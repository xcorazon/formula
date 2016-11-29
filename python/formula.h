#include <Python.h>
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
static PyObject * Formula_asin(PyObject *self, PyObject *param);
static PyObject * Formula_acos(PyObject *self, PyObject *param);

/*
 * Formula functions
 */
 
static PyObject *Formula_toStarMath(FormulaObject *self);
static PyObject *Formula_mulout(FormulaObject *self);
static PyObject *Formula_mulin(FormulaObject *self);
static PyObject *Formula_transform(FormulaObject *self);
static PyObject *Formula_calculate(FormulaObject *self);

/*
 * Number protocol functions
 */
static PyObject *Formula_NAdd(PyObject *o1, PyObject *o2);
static PyObject *Formula_NSub(PyObject *o1, PyObject *o2);
static PyObject *Formula_NMul(PyObject *o1, PyObject *o2);

static PyMethodDef module_methods[] = {
    {"sin", (PyCFunction)Formula_sin, METH_O, "Creates sinus formula type."},
    {"cos", (PyCFunction)Formula_cos, METH_O, "Creates cosinus formula type."},
    {"asin", (PyCFunction)Formula_asin, METH_O, "Creates arcsinus formula type."},
    {"acos", (PyCFunction)Formula_acos, METH_O, "Creates arccosinus formula type."},
    {NULL}  /* Sentinel */
};

static PyMethodDef Formula_methods[] = {
    {"toStarMath", (PyCFunction)Formula_toStarMath, METH_NOARGS, "Return StarMath view of the formula"},
    {"multipliersOut", (PyCFunction)Formula_mulout, METH_NOARGS, "Moves multipliers out the brackets."},
    {"multipliersIn", (PyCFunction)Formula_mulin, METH_NOARGS, "Moves multipliers out the brackets."},
    {"transform", (PyCFunction)Formula_transform, METH_NOARGS, "Transforms equation if it is possible."},
    {"calculate", (PyCFunction)Formula_calculate, METH_NOARGS, "Calculate number expressions if it is possible."},
    {NULL}  /* Sentinel */
};

static PyNumberMethods Formula_NProtocol = {
    Formula_NAdd,       /* binaryfunc nb_add; */
    Formula_NSub,       /* binaryfunc nb_subtract; */
    Formula_NMul,       /* binaryfunc nb_multiply; */
    0,                  /* binaryfunc nb_divide; */
    0,                  /* binaryfunc nb_remainder; */
    0,                  /* binaryfunc nb_divmod; */
    0,                  /* ternaryfunc nb_power; */
    0,                  /* unaryfunc nb_negative; */
    0,                  /* unaryfunc nb_positive; */
    0,                  /* unaryfunc nb_absolute; */
    0,                  /* inquiry nb_nonzero; */
    0,                  /* unaryfunc nb_invert; */
    0,                  /* binaryfunc nb_lshift; */
    0,                  /* binaryfunc nb_rshift; */
    0,                  /* binaryfunc nb_and; */
    0,                  /* binaryfunc nb_xor; */
    0,                  /* binaryfunc nb_or; */
    0,                  /* coercion nb_coerce; */
    0,                  /* unaryfunc nb_int; */
    0,                  /* unaryfunc nb_long; */
    0,                  /* unaryfunc nb_float; */
    0,                  /* unaryfunc nb_oct; */
    0,                  /* unaryfunc nb_hex; */

    /* Added in release 2.0 */
    0,                  /* binaryfunc nb_inplace_add; */
    0,                  /* binaryfunc nb_inplace_subtract; */
    0,                  /* binaryfunc nb_inplace_multiply; */
    0,                  /* binaryfunc nb_inplace_divide; */
    0,                  /* binaryfunc nb_inplace_remainder; */
    0,                  /* ternaryfunc nb_inplace_power; */
    0,                  /* binaryfunc nb_inplace_lshift; */
    0,                  /* binaryfunc nb_inplace_rshift; */
    0,                  /* binaryfunc nb_inplace_and; */
    0,                  /* binaryfunc nb_inplace_xor; */
    0,                  /* binaryfunc nb_inplace_or; */

    /* Added in release 2.2 */
    0,                  /* binaryfunc nb_floor_divide; */
    0,                  /* binaryfunc nb_true_divide; */
    0,                  /* binaryfunc nb_inplace_floor_divide; */
    0,                  /* binaryfunc nb_inplace_true_divide; */

    /* Added in release 2.5 */
    0                   /* unaryfunc nb_index; */
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
    &Formula_NProtocol,          /*tp_as_number*/
    0,                           /*tp_as_sequence*/
    0,                           /*tp_as_mapping*/
    0,                           /*tp_hash */
    0,                           /*tp_call*/
    0,                           /*tp_str*/
    0,                           /*tp_getattro*/
    0,                           /*tp_setattro*/
    0,                           /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_INPLACEOPS | Py_TPFLAGS_CHECKTYPES,  /*tp_flags*/
    "formula objects",           /* tp_doc */
    0,                           /* tp_traverse */
    0,                           /* tp_clear */
    0,                           /* tp_richcompare */
    0,                           /* tp_weaklistoffset */
    0,                           /* tp_iter */
    0,                           /* tp_iternext */
    Formula_methods,             /* tp_methods */
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
