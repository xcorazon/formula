#include "formula.h"
#include "structmember.h"
#include "../eqtypes.h"
#include "../common.h"


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

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "i", kwlist, &type))
        return NULL;

    if(self != NULL)
        self->equation = eq_node_new(type, 1);
    
    return 0;
}



static PyMethodDef module_methods[] = {
    {NULL}  /* Sentinel */
};

static PyMemberDef Formula_members[] = {
    {NULL}  /* Sentinel */
};


#ifndef PyMODINIT_FUNC	/* declarations for DLL import/export */
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
