﻿#include <Python.h>
#include "structmember.h"

typedef struct {
    PyObject_HEAD
    void *equation; /* eq_node or eq_leaf */
} FormulaObject;

static void Formula_dealloc(FormulaObject *self);
static PyObject * Formula_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int Formula_init(FormulaObject *self, PyObject *args, PyObject *kwds);

/*
 * module functions
 */
static PyObject * Formula_sin(PyObject *self, PyObject *param);
static PyObject * Formula_cos(PyObject *self, PyObject *param);

/*
 * Formula functions
 */
 
static PyObject *Formula_toStarMath(FormulaObject *self);

static PyMethodDef module_methods[] = {
    {"sin", (PyCFunction)Formula_sin, METH_O, "Creates sinus formula type."},
    {"cos", (PyCFunction)Formula_cos, METH_O, "Creates cosinus formula type."},
    {NULL}  /* Sentinel */
};

static PyMethodDef Formula_memthods[] = {
    {"toStarMath", (PyCFunction)Formula_toStarMath, METH_NOARGS, "Return StarMath view of the formula"},
    {NULL}  /* Sentinel */
};

static PyMemberDef Formula_members[] = {
    {NULL}  /* Sentinel */
};


static PyTypeObject FormulaType = {
    PyObject_HEAD_INIT(NULL)
    0,                           /*ob_size*/
    "formula.Formula",           /*tp_name*/
    sizeof(FormulaObject),       /*tp_basicsize*/
    0,                           /*tp_itemsize*/
    (destructor)Formula_dealloc, /*tp_dealloc*/
    0,                           /*tp_print*/
    0,                           /*tp_getattr*/
    0,                           /*tp_setattr*/
    0,                           /*tp_compare*/
    0,                           /*tp_repr*/
    0,                           /*tp_as_number*/
    0,                           /*tp_as_sequence*/
    0,                           /*tp_as_mapping*/
    0,                           /*tp_hash */
    0,                           /*tp_call*/
    0,                           /*tp_str*/
    0,                           /*tp_getattro*/
    0,                           /*tp_setattro*/
    0,                           /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,  /*tp_flags*/
    "formula objects",           /* tp_doc */
    0,                           /* tp_traverse */
    0,                           /* tp_clear */
    0,                           /* tp_richcompare */
    0,                           /* tp_weaklistoffset */
    0,                           /* tp_iter */
    0,                           /* tp_iternext */
    Formula_memthods,            /* tp_methods */
    Formula_members,             /* tp_members */
    0,                           /* tp_getset */
    0,                           /* tp_base */
    0,                           /* tp_dict */
    0,                           /* tp_descr_get */
    0,                           /* tp_descr_set */
    0,                           /* tp_dictoffset */
    (initproc)Formula_init,      /* tp_init */
    0,                           /* tp_alloc */
    Formula_new                  /* tp_new */
};
