#include "formula.h"
#include "../eqtypes.h"
#include "../common.h"
#include "../mul.h"
#include "../transform.h"
#include "../starmath.h"
#include "../debug/debug.h"


#ifdef OS_WINDOWS
  #define swprintf _snwprintf
#endif

// release memory 
static void Formula_dealloc(FormulaObject *self)
{
    if( self->equation != 0)
        eq_delete(self->equation);
    
    self->ob_type->tp_free((PyObject*)self);
}

//alloc memory for Formula
static PyObject * Formula_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    FormulaObject *self;
    self = (FormulaObject *)type->tp_alloc(type, 0);
    return (PyObject *)self;
}

// init Formula after creation
static int Formula_init(FormulaObject *self, PyObject *args, PyObject *kwds)
{
    int type = EQ_SUMM;
    static char *kwlist[] = {"type", NULL};
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|i", kwlist, &type))
        return -1;

    if(self != NULL) {
        self->equation = eq_node_new(type, 1);
        ((struct eq_node *)(self->equation))->first_child = eq_leaf_new(EQ_NUMBER, 1, NULL, 0);
    }
    
    return 0;
}

/*
 * Formula functions
 */
static PyObject *
Formula_toStarMath(FormulaObject *self) 
{
    wchar_t star_math[1000];
    PyObject *res = NULL;
    
    if(self != NULL) {
        *star_math = 0;
        sm_to_string((struct eq_node *)(self->equation), SM_DEFAULT, star_math);

        res = PyUnicode_FromUnicode(star_math, (Py_ssize_t)wcslen(star_math));
        
        if(res == NULL)
            Py_RETURN_NONE;
        else
            return res;
    }
    
    return res;
}


static PyObject *Formula_mulout(FormulaObject *self)
{
    if(self != NULL)
        eq_move_multipliers_out((struct eq_node **)&self->equation);
    
    return 0;
}


static PyObject *Formula_mulin(FormulaObject *self)
{
    return 0;
}


static PyObject *Formula_transform(FormulaObject *self)
{
    return 0;
}


static PyObject *Formula_calculate(FormulaObject *self)
{
    return 0;
}

/*
 * Number protocol functions
 */

struct eq_node *getEquation(PyObject *param)
{
    wchar_t tmp[100];
    double num = 0.0;
    char sign = 1;
    char is_num = 0;
    *tmp = 0;
    
    if(PyObject_TypeCheck(param, &FormulaType)) {
      struct eq_node *node = ((FormulaObject *)param)->equation;
      
      return (struct eq_node *)eq_clone(node);
    }
    
    if(PyUnicode_Check(param)) {
        if (PyUnicode_AsWideChar((PyUnicodeObject *)param, tmp, 100) < 0) 
            return NULL;
        
        return (struct eq_node *)eq_leaf_new(EQ_SYMBOL, 1, tmp, 0);
    }
    
    if(PyString_CheckExact(param)) {
        char *str = PyString_AsString(param);
        swprintf(tmp, 100, L"%hs", str);
        
        return (struct eq_node *)eq_leaf_new(EQ_SYMBOL, 1, tmp, 0);
    }
    
    if(PyFloat_Check(param)) {
        num = PyFloat_AsDouble(param);
        if (num < 0) sign = -1;
        is_num = 1;
    }
    
    if(PyInt_Check(param) || PyLong_Check(param)) {
        num = (double)PyInt_AS_LONG(param);
        if (num < 0) sign = -1;
        is_num = 1;
    }
    
    if(is_num) 
        return (struct eq_node *)eq_leaf_new(EQ_NUMBER, sign, NULL, sign*num );

    return NULL;
}


static PyObject *Formula_NAdd(PyObject *o1, PyObject *o2)
{
    struct eq_node *obj1;
    struct eq_node *obj2;
    FormulaObject *result;
    
    obj1 = getEquation(o1);
    obj2 = getEquation(o2);
    
    if(obj1 == NULL || obj2 == NULL) {
        if(obj1 != NULL)
            eq_delete(obj1);
        if(obj2 != NULL)
            eq_delete(obj2);
      
        PyErr_SetString(PyExc_ValueError, "Invalid arguments.\nArguments must be <int>, <float>, <string> or <Formula>.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_node_new(EQ_SUMM, 1);
    
    obj1->next = obj2;
    ((struct eq_node *)(result->equation))->first_child = obj1;
    eq_transform(&result->equation);
    eq_transform(&result->equation);
    
    return (PyObject *)result;
}


static PyObject *Formula_NSub(PyObject *o1, PyObject *o2)
{
    struct eq_node *obj1;
    struct eq_node *obj2;
    FormulaObject *result;
    
    obj1 = getEquation(o1);
    obj2 = getEquation(o2);
    
    if(obj1 == NULL || obj2 == NULL) {
        if(obj1 != NULL)
            eq_delete(obj1);
        if(obj2 != NULL)
            eq_delete(obj2);
      
        PyErr_SetString(PyExc_ValueError, "Invalid arguments.\nArguments must be <int>, <float>, <string> or <Formula>.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_node_new(EQ_SUMM, 1);
    
    obj2->sign *= -1;
    obj1->next = obj2;
    ((struct eq_node *)(result->equation))->first_child = obj1;
    eq_transform(&result->equation);
    eq_transform(&result->equation);
    
    return (PyObject *)result;
}


static PyObject *Formula_NMul(PyObject *o1, PyObject *o2)
{
    struct eq_node *obj1;
    struct eq_node *obj2;
    FormulaObject *result;
    
    obj1 = getEquation(o1);
    obj2 = getEquation(o2);
    
    if(obj1 == NULL || obj2 == NULL) {
        if(obj1 != NULL)
            eq_delete(obj1);
        if(obj2 != NULL)
            eq_delete(obj2);
      
        PyErr_SetString(PyExc_ValueError, "Invalid arguments.\nArguments must be <int>, <float>, <string> or <Formula>.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_node_new(EQ_MUL, 1);
    
    obj1->next = obj2;
    ((struct eq_node *)(result->equation))->first_child = obj1;
    eq_transform(&result->equation);
    eq_transform(&result->equation);
    
    return (PyObject *)result;
}


static PyObject *Formula_NDiv(PyObject *o1, PyObject *o2)
{
    struct eq_node *obj1;
    struct eq_node *obj2;
    struct eq_node *div;
    FormulaObject *result;
    
    obj1 = getEquation(o1);
    obj2 = getEquation(o2);
    
    if(obj1 == NULL || obj2 == NULL) {
        if(obj1 != NULL)
            eq_delete(obj1);
        if(obj2 != NULL)
            eq_delete(obj2);
      
        PyErr_SetString(PyExc_ValueError, "Invalid arguments.\nArguments must be <int>, <float>, <string> or <Formula>.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_node_new(EQ_MUL, 1);
    
    div = eq_node_new(EQ_RECIPROCAL, 1);
    div->first_child = obj2;
    
    obj1->next = div;
    ((struct eq_node *)(result->equation))->first_child = obj1;
    eq_transform(&result->equation);
    eq_transform(&result->equation);
    
    return (PyObject *)result;
}


/*
 * Module functions
 */
static PyObject * Formula_sin(PyObject *self, PyObject *param)
{
    wchar_t variable[1000];
    char is_num = 0;
    char sign   = 1;
    double num  = 0.0;
    
    FormulaObject *result;
    
    memset(variable, 0, 1000 * sizeof(wchar_t));
    
    if(PyUnicode_Check(param)) {
        if (PyUnicode_AsWideChar((PyUnicodeObject *)param, variable, 100) < 0) {
            PyErr_SetString(PyExc_ValueError, "Error in copy unicode string.");
            return NULL;
        }
    }
    
    if(PyString_CheckExact(param)) {
        char *str = PyString_AsString(param);
        swprintf(variable, 100, L"%hs", str);
    }
    
    if(*variable != 0) {
        result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
        result->equation = eq_node_new(EQ_SIN, 1);
        ((struct eq_node *)(result->equation))->first_child = eq_leaf_new(EQ_SYMBOL, 1, variable, 0);
        
        return (PyObject *)result;
    }
    
    if(PyFloat_Check(param)) {
        num = PyFloat_AsDouble(param);
        
        if (num < 0) sign = -1;
        is_num = 1;
        
    }
    
    if(PyInt_Check(param) || PyLong_Check(param)) {
        num = (double)PyInt_AS_LONG(param);
        if (num < 0) sign = -1;
        is_num = 1;
    }
    
    if(is_num) {
        result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
        result->equation = eq_node_new(EQ_SIN, 1);
        ((struct eq_node *)(result->equation))->first_child = eq_leaf_new(EQ_NUMBER, sign, NULL, sign*num );
        
        return (PyObject *)result;
    }
    
    if(PyObject_TypeCheck(param, &FormulaType)) {
        result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
        result->equation = eq_node_new(EQ_SIN, 1);
        
        struct eq_node *node = ((FormulaObject *)param)->equation;
        ((struct eq_node *)(result->equation))->first_child = eq_clone(node);
        
        return (PyObject *)result;
    }
    
    PyErr_SetString(PyExc_ValueError, "Invalid argument.");
    return NULL;
}


static PyObject * Formula_cos(PyObject *self, PyObject *param)
{
    FormulaObject *res = (FormulaObject *)Formula_sin(self, param);
    if(res != NULL)
      ((struct eq_node *)(res->equation))->type = EQ_COS;
    
    return (PyObject *)res;
}


static PyObject * Formula_asin(PyObject *self, PyObject *param)
{
    FormulaObject *res = (FormulaObject *)Formula_sin(self, param);
    if(res != NULL)
      ((struct eq_node *)(res->equation))->type = EQ_ASIN;
    
    return (PyObject *)res;
}


static PyObject * Formula_acos(PyObject *self, PyObject *param)
{
    FormulaObject *res = (FormulaObject *)Formula_sin(self, param);
    if(res != NULL)
      ((struct eq_node *)(res->equation))->type = EQ_ACOS;
    
    return (PyObject *)res;
}


static PyObject * Formula_symbol(PyObject *self, PyObject *args, PyObject *kwargs)
{
    PyObject *name;
    int sign = 1;
    wchar_t sym_name[1000];
    *sym_name = 0;
    
    FormulaObject *result;
    
    static char *kwlist[]={"name", "sign", NULL};
    
    if(!PyArg_ParseTupleAndKeywords(args, kwargs, "O|i", kwlist, &name, &sign)) {
        PyErr_SetString(PyExc_ValueError, "Invalid arguments. \"name\"=<string>, \"sign\" = optional <int>.");
        return NULL;
    }
    
    if(PyUnicode_Check(name)) {
        if (PyUnicode_AsWideChar((PyUnicodeObject *)name, sym_name, 100) < 0) {
            PyErr_SetString(PyExc_ValueError, "Invalid unicode string.");
            return NULL;
        }
        
    } else if(PyString_CheckExact(name)) {
        char *str = PyString_AsString(name);
        swprintf(sym_name, 100, L"%hs", str);
        
    } else {
        PyErr_SetString(PyExc_ValueError, "Invalid arguments. \"name\"=<string>, \"sign\" = optional <int>.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_leaf_new(EQ_SYMBOL, sign, sym_name, 0);
    
    return (PyObject *)result;
}


static PyObject * Formula_number(PyObject *self, PyObject *param)
{
    double num = 0.0;
    char sign = 1;
    FormulaObject *result;
    
    if(PyFloat_Check(param)) {
        num = PyFloat_AsDouble(param);
        if (num < 0) sign = -1;
    } else if(PyInt_Check(param) || PyLong_Check(param)) {
        num = (double)PyInt_AS_LONG(param);
        if (num < 0) sign = -1;
    } else {
        PyErr_SetString(PyExc_ValueError, "Invalid argument.");
        return NULL;
    }
    
    result = (FormulaObject *)Formula_new(&FormulaType, Py_None, Py_None);
    result->equation = eq_leaf_new(EQ_NUMBER, sign, NULL, sign*num );
    
    return (PyObject *)result;
}


#ifndef PyMODINIT_FUNC /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif
PyMODINIT_FUNC
initformula(void)
{
    PyObject* m;

    FormulaType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&FormulaType) < 0)
        return;

    m = Py_InitModule3("formula", module_methods, "Init formula type.");

    Py_INCREF(&FormulaType);
    PyModule_AddObject(m, "Formula", (PyObject *)&FormulaType);

    PyModule_AddIntConstant(m, "EQ_SYMBOL", EQ_SYMBOL);
    PyModule_AddIntConstant(m, "EQ_NUMBER", EQ_NUMBER);
    PyModule_AddIntConstant(m, "EQ_SUMM", EQ_SUMM);
    PyModule_AddIntConstant(m, "EQ_MUL", EQ_MUL);
    PyModule_AddIntConstant(m, "EQ_RECIPROCAL", EQ_RECIPROCAL);
    PyModule_AddIntConstant(m, "EQ_SIN", EQ_SIN);
    PyModule_AddIntConstant(m, "EQ_COS", EQ_COS);
    PyModule_AddIntConstant(m, "EQ_ASIN", EQ_ASIN);
    PyModule_AddIntConstant(m, "EQ_ACOS", EQ_ACOS);
    PyModule_AddIntConstant(m, "EQ_POW", EQ_POW);
}
