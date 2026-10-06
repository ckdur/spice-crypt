#include <Python.h>
#include <string.h>

#include "rand.h"

#define RANDSIZB (RANDSIZ * sizeof(ub4))

static randctx rctx;

static const char randuint32_docstring[] =
    "Return a random integer in [0, 0xFFFFFFFF].\n\n"
    ">>> pyisaac.randuint32()\n"
    "3297083183";

static const char random_docstring[] =
    "Return a random float in [0, 1].\n\n"
    ">>> pyisaac.random()\n"
    "0.3417196273803711";

static void seed_rng(const char *data, Py_ssize_t length);

static PyObject *pyisaac_random(PyObject *self, PyObject *unused)
{
    (void)self;
    (void)unused;
    return PyFloat_FromDouble(rand(&rctx) / (double)0xFFFFFFFFu);
}

static PyObject *pyisaac_randuint32(PyObject *self, PyObject *unused)
{
    (void)self;
    (void)unused;
    return PyLong_FromUnsignedLong((unsigned long)rand(&rctx));
}

static PyObject *pyisaac_seed(PyObject *self, PyObject *args)
{
    PyObject *seed_object;
    const char *seed_data;
    Py_ssize_t seed_length;

    (void)self;

    if (!PyArg_ParseTuple(args, "O:seed", &seed_object))
        return NULL;

    if (PyUnicode_Check(seed_object)) {
        seed_data = PyUnicode_AsUTF8AndSize(seed_object, &seed_length);
        if (seed_data == NULL)
            return NULL;
    } else if (PyBytes_Check(seed_object)) {
        char *bytes_data;
        if (PyBytes_AsStringAndSize(seed_object, &bytes_data, &seed_length) < 0)
            return NULL;
        seed_data = bytes_data;
    } else {
        PyErr_SetString(PyExc_TypeError, "seed must be str or bytes");
        return NULL;
    }

    if (seed_length == 0) {
        PyErr_SetString(PyExc_ValueError, "seed must not be empty");
        return NULL;
    }

    seed_rng(seed_data, seed_length);
    Py_RETURN_NONE;
}

static void seed_rng(const char *data, Py_ssize_t length)
{
    Py_ssize_t i;
    Py_ssize_t quotient = RANDSIZB / length;
    Py_ssize_t remainder = RANDSIZB % length;

    for (i = 0; i < quotient; i++)
        memcpy((char *)rctx.randrsl + i * length, data, (size_t)length);

    if (remainder > 0)
        memcpy((char *)rctx.randrsl + quotient * length, data, (size_t)remainder);

    randinit(&rctx, TRUE);
    isaac(&rctx);
}

static PyMethodDef module_methods[] = {
    {"random", pyisaac_random, METH_NOARGS, random_docstring},
    {"randuint32", pyisaac_randuint32, METH_NOARGS, randuint32_docstring},
    {"seed", pyisaac_seed, METH_VARARGS, "Seed the ISAAC generator with text or bytes."},
    {NULL, NULL, 0, NULL},
};

static struct PyModuleDef pyisaac_module = {
    PyModuleDef_HEAD_INIT,
    "_pyisaac",
    "ISAAC random number generator.",
    -1,
    module_methods,
    NULL,
    NULL,
    NULL,
    NULL,
};

PyMODINIT_FUNC PyInit__pyisaac(void)
{
    PyObject *module = PyModule_Create(&pyisaac_module);
    if (module == NULL)
        return NULL;

    if (PyModule_AddIntConstant(module, "RANDSIZB", RANDSIZB) < 0) {
        Py_DECREF(module);
        return NULL;
    }

    return module;
}