#include <Python.h>
#include "../eqtypes.h"

#include "formula.h"

// release memory 
static void Formula_dealloc(FormulaObject *self)
{
    if( self->formula != 0)
        eq_delete(self->formula);
    
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
static int Formula_init(DatablockObject *self, PyObject *args, PyObject *kwds)
{
    if(self != NULL)
        self->formula = new DataBlock();
    
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
}