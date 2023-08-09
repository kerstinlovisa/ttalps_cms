//  ConfigManager.cpp
//
//  Created by Jeremi Niedziela on 09/08/2023.

#include "ConfigManager.hpp"

#include <type_traits>

#include "Logger.hpp"

using namespace std;

ConfigManager::ConfigManager(string configPath) {
  Py_Initialize();

  pythonFile = fopen(configPath.c_str(), "r");

  if (!pythonFile) {
    PyErr_Print();
    Py_Finalize();
    fatal() << "Could not parse python config\n";
    exit(0);
  }

  PyRun_SimpleFile(pythonFile, configPath.c_str());
  fclose(pythonFile);

  pythonModule = PyImport_ImportModule("__main__");
  if (!pythonModule) PyErr_Print();

  pythonDict = PyModule_GetDict(pythonModule);
}

ConfigManager::~ConfigManager() {
  Py_Finalize();
  delete pythonFile;
  delete pythonModule;
  delete pythonDict;
}

PyObject* ConfigManager::GetPythonList(string name) {
  PyObject* pythonList = PyDict_GetItemString(pythonDict, name.c_str());
  if (!pythonList || !PyList_Check(pythonList)) {
    PyErr_Print();
    fatal() << "Could not find object in python config file: " << name << "\n";
    exit(1);
  }
  return pythonList;
}