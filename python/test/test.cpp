

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

FAILED: python/libiRadio_python.dll python/libiRadio_python.dll.a 
C:\windows\system32\cmd.exe /C "cd . && "D:\soft\clion\CLion 2024.1.4\bin\mingw\bin\g++.exe" -O3 -DNDEBUG   -shared -o python\libiRadio_python.dll -Wl,--out-implib,python\libiRadio_python.dll.a -Wl,--major-image-version,0,--minor-image-version,0 python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj python/CMakeFiles/iRadio_python.dir/base/PyAgent.cpp.obj python/CMakeFiles/iRadio_python.dir/base/PyInterpreter.cpp.obj python/CMakeFiles/iRadio_python.dir/base/PythonModuleAPI.cpp.obj python/CMakeFiles/iRadio_python.dir/test.cpp.obj  D:/soft/qt_13/5.13.0/mingw73_64/lib/libQt5Core.a  -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32 && cd ."
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj:mocs_compilation.cpp:(.text+0x33): undefined reference to `__imp__ZN2Py11PythonAgent16staticMetaObjectE'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj:mocs_compilation.cpp:(.text+0x93): undefined reference to `__imp__ZN2Py11PythonAgent16staticMetaObjectE'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj:mocs_compilation.cpp:(.text+0xcb): undefined reference to `__imp__ZN2Py11PythonAgent16staticMetaObjectE'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj:mocs_compilation.cpp:(.text+0x1e8): undefined reference to `__imp__ZN2Py11PythonAgent16staticMetaObjectE'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/iRadio_python_autogen/mocs_compilation.cpp.obj:mocs_compilation.cpp:(.text.startup+0xa): undefined reference to `__imp__ZN2Py11PythonAgent16staticMetaObjectE'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/base/PyAgent.cpp.obj:PyAgent.cpp:(.text+0x84): undefined reference to `__imp_PyRun_SimpleFileExFlags'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/base/PyAgent.cpp.obj:PyAgent.cpp:(.text+0x2df): undefined reference to `__imp_PyRun_SimpleStringFlags'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/base/PyAgent.cpp.obj:PyAgent.cpp:(.text+0x4cb): undefined reference to `__imp_Py_IsInitialized'
D:\soft\clion\CLion 2024.1.4\bin\mingw\bin/ld.exe: python/CMakeFiles/iRadio_python.dir/base/PyAgent.cpp.obj:PyAgent.cpp:(.text+0x504): undefined reference to `__imp_PyErr_PrintEx'
collect2.exe: error: ld returned 1 exit status
