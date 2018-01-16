//#include "stdafx.h"
#include <iostream>
#include <Python.h>

int main() {
	Py_Initialize();
	PyObject
		*pName,
		*pModule,
		*pDict,
		*pFunc,
		*pArgs,
		*pValue;
	// get filename as a pystring
	pName = PyString_FromString("ttest");
	std::cout << std::endl << pName;
	// Import module from filename
	pModule = PyImport_Import(pName);
	std::cout << std::endl << pModule;
	// build the module's dict
	pDict = PyModule_GetDict(pModule);
	std::cout << std::endl << pDict << " " << PyDict_Size(pDict);
	PyObject* keys = PyDict_Keys(pDict);
	int s = PyList_Size(keys);
	for (int i = 0; i < s; ++i) {
		PyObject* item = PyList_GetItem(keys, i);
		printf("\n");
		printf(PyString_AsString(item));
	}

	PyObject* testvar = PyDict_GetItemString(pDict, "qa");
	printf("\n");
	printf(PyString_AsString(testvar));

	// get a function from the dict
	pFunc = PyDict_GetItemString(pDict, "test");
	std::cout << std::endl << pFunc;
	// build the arg tuple
	pArgs = PyTuple_New(1);
	// create an argument
	pValue = PyString_FromString("cee programme");
	// set an argument
	PyTuple_SetItem(pArgs, 0, pValue);
	// call the function with the func and the args
	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
	// error checking
	if (pResult == NULL) {
		printf("\nis broek");
	}

	char* res = PyString_AsString(pResult);
	// "destroy the interpreter"
	Py_Finalize();
	printf(res);

	std::cout << "\nPress Enter to Continue...";
	std::cin.ignore(1);

	return 0;
}
