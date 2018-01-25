#include "stdafx.h"
#include "ecpy.h"
#include <Python.h>

namespace ECPy {

	int Functions::predictions(char* dir)	{
		double result = 0;
		// startup
		Py_Initialize();
		PyObject
			*Module,
			*Dict,
			*Func,
			*Args;
		// import the module
		Module = PyImport_Import(PyString_FromString("predictions"));
		// get dictionary from module
		Dict = PyModule_GetDict(Module);
		// get function from dictionary
		Func = PyDict_GetItemString(Dict, "predictions");
		// make args tuple
		Args = PyTuple_New(1);
		// set args for function
		PyTuple_SetItem(Args, 0, PyString_FromString(dir));

		PyObject* pRes = PyObject_CallObject(Func, Args);

		if (pRes == NULL) {
			result = -1;
		}
		else {
			result = PyInt_AsLong(pRes);
		}

		// cleanup
		Py_Finalize();
		return result;
	}

	int Functions::image_partition(char* dir) {
		double result = 0;
		// startup
		Py_Initialize();
		PyObject
			*Module,
			*Dict,
			*Func,
			*Args;
		// import the module
		Module = PyImport_Import(PyString_FromString("image_partition"));
		// get dictionary from module
		Dict = PyModule_GetDict(Module);
		// get function from dictionary
		Func = PyDict_GetItemString(Dict, "image_partition");
		// make args tuple
		Args = PyTuple_New(1);
		// set args for function
		PyTuple_SetItem(Args, 0, PyString_FromString(dir));

		PyObject* pRes = PyObject_CallObject(Func, Args);

		if (pRes == NULL) {
			result = -1;
		}
		else {
			result = PyInt_AsLong(pRes);
		}

		// cleanup
		Py_Finalize();
		return result;
	}

	int Functions::train(char* dir, int iters) {
		double result = 0;
		// startup
		Py_Initialize();
		PyObject
			*Module,
			*Dict,
			*Func,
			*Args;
		// import the module
		Module = PyImport_Import(PyString_FromString("predictions"));
		// get dictionary from module
		Dict = PyModule_GetDict(Module);
		// get function from dictionary
		Func = PyDict_GetItemString(Dict, "predictions");
		// make args tuple
		Args = PyTuple_New(2);
		// set directory
		PyTuple_SetItem(Args, 0, PyString_FromString(dir));
		// set iterations
		PyTuple_SetItem(Args, 1, PyLong_FromLong(iters));

		PyObject* pRes = PyObject_CallObject(Func, Args);

		if (pRes == NULL) {
			result = -1;
		}
		else {
			result = PyInt_AsLong(pRes);
		}

		// cleanup
		Py_Finalize();
		return result;
	}

}