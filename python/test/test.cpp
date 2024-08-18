

#include "test.h"
#include <iostream>
#include <Python.h>

void MyPythonTest::TestPython() {
// 初始化 Python 解释器
	Py_Initialize();

	// 打印 Python 环境信息
	PyRun_SimpleString("print('Python environment initialized.')");

	// 导入 Python 的 math 模块
	PyObject *pName = PyUnicode_DecodeFSDefault("math");
	PyObject *pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != nullptr) {
		// 获取 math.sqrt 函数
		PyObject *pFunc = PyObject_GetAttrString(pModule, "sqrt");
		if (pFunc && PyCallable_Check(pFunc)) {
			// 创建参数 (16) 调用 sqrt 函数
			PyObject *pArgs = PyTuple_Pack(1, PyFloat_FromDouble(16.0));

			// 调用 sqrt 函数
			PyObject *pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if (pValue != nullptr) {
				double result = PyFloat_AsDouble(pValue);
				std::cout << "Result of sqrt(16): " << result << std::endl;
				Py_DECREF(pValue);
			} else {
				PyErr_Print();
				std::cerr << "Call to sqrt() failed" << std::endl;
			}
		} else {
			PyErr_Print();
			std::cerr << "Cannot find function sqrt" << std::endl;
		}
		// 释放函数对象
		Py_XDECREF(pFunc);
		// 释放模块对象
		Py_DECREF(pModule);
	} else {
		PyErr_Print();
		std::cerr << "Failed to load \"math\" module" << std::endl;
	}

	// 关闭 Python 解释器
	Py_Finalize();
}