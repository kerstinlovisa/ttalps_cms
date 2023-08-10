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
    fatal() << "Could not parse python config\n";
    Py_Finalize();
    exit(1);
  }

  PyRun_SimpleFile(pythonFile, configPath.c_str());
  fclose(pythonFile);

  pythonModule = PyImport_ImportModule("__main__");
  if (!pythonModule) {
    fatal() << "Couldn't import __main__ from the python module\n";
    Py_Finalize();
    exit(1);
  }

  config = PyModule_GetDict(pythonModule);
}

ConfigManager::~ConfigManager() { Py_Finalize(); }

int ConfigManager::GetCollectionSize(PyObject *collection) {
  int size = -1;
  if (PyList_Check(collection))
    size = PyList_Size(collection);
  else if (PyTuple_Check(collection))
    size = PyTuple_Size(collection);
  return size;
}

PyObject* ConfigManager::GetItem(PyObject *collection, int index) {
  PyObject *item;

  if (PyList_Check(collection))
    item = PyList_GetItem(collection, index);
  else if (PyTuple_Check(collection))
    item = PyTuple_GetItem(collection, index);

  return item;
}

//-------------------------------------------------------------------------------------------------
// Methods to retrieve a value/list/dict from the python file
//-------------------------------------------------------------------------------------------------

PyObject *ConfigManager::GetPythonValue(string name) {
  PyObject *pythonValue = PyDict_GetItemString(config, name.c_str());
  if (!pythonValue) {
    fatal() << "Could not find a value in python config file: " << name << "\n";
    exit(1);
  }
  return pythonValue;
}

PyObject *ConfigManager::GetPythonList(string name) {
  PyObject *pythonList = PyDict_GetItemString(config, name.c_str());
  if (!pythonList || !PyList_Check(pythonList)) {
    fatal() << "Could not find a list in python config file: " << name << "\n";
    exit(1);
  }
  return pythonList;
}

PyObject *ConfigManager::GetPythonDict(string name) {
  PyObject *pythonDict = PyDict_GetItemString(config, name.c_str());
  if (!pythonDict || !PyDict_Check(pythonDict)) {
    fatal() << "Could not find a dict in python config file: " << name << "\n";
    fatal() << "dict address: " << pythonDict << "\n";
    exit(1);
  }
  return pythonDict;
}

//-------------------------------------------------------------------------------------------------
// Template specializations to extract a value from the python file
//-------------------------------------------------------------------------------------------------

template <>
void ConfigManager::GetValue<string>(std::string name, string &outputValue) {
  PyObject *pythonValue = GetPythonValue(name);
  if (!pythonValue || !PyUnicode_Check(pythonValue)) {
    error() << "Failed retriving python value (string)\n";
    return;
  }
  outputValue = PyUnicode_AsUTF8(pythonValue);
}

template <>
void ConfigManager::GetValue<int>(std::string name, int &outputValue) {
  PyObject *pythonValue = GetPythonValue(name);
  if (!pythonValue || (!PyUnicode_Check(pythonValue) && !PyLong_Check(pythonValue))) {
    error() << "Failed retriving python value (int)\n";
    return;
  }
  outputValue = PyLong_AsLong(pythonValue);
}

template <>
void ConfigManager::GetValue<float>(std::string name, float &outputValue) {
  PyObject *pythonValue = GetPythonValue(name);
  if (!pythonValue || !PyFloat_Check(pythonValue)) {
    error() << "Failed retriving python value (float)\n";
    return;
  }
  outputValue = PyFloat_AsDouble(pythonValue);
}

//-------------------------------------------------------------------------------------------------
// Template specializations to extract a vector from the python file
//-------------------------------------------------------------------------------------------------

template <>
void ConfigManager::GetVector<std::string>(std::string name, std::vector<std::string> &outputVector) {
  PyObject *pythonList = GetPythonList(name);

  for (Py_ssize_t i = 0; i < GetCollectionSize(pythonList); ++i) {
    PyObject *item = GetItem(pythonList, i);
    
    if (!item || !PyUnicode_Check(item)) {
      error() << "Failed retriving python vector<string>\n";
      continue;
    }
    std::string value = PyUnicode_AsUTF8(item);
    outputVector.push_back(value);
  }
}

//-------------------------------------------------------------------------------------------------
// Template specializations to extract a map from the python file
//-------------------------------------------------------------------------------------------------

template <>
void ConfigManager::GetMap<std::string, std::string>(std::string name, std::map<std::string, std::string> &outputMap) {
  PyObject *pythonDict = GetPythonDict(name);

  PyObject *pKey, *pValue;
  Py_ssize_t pos = 0;

  while (PyDict_Next(pythonDict, &pos, &pKey, &pValue)) {
    if (!PyUnicode_Check(pKey) || !PyUnicode_Check(pValue)) {
      error() << "Failed retriving python key-value pair (string-string)\n";
      continue;
    }
    outputMap[PyUnicode_AsUTF8(pKey)] = PyUnicode_AsUTF8(pValue);
  }
}

template <>
void ConfigManager::GetMap<std::string, int>(std::string name, std::map<std::string, int> &outputMap) {
  PyObject *pythonDict = GetPythonDict(name);

  PyObject *pKey, *pValue;
  Py_ssize_t pos = 0;

  while (PyDict_Next(pythonDict, &pos, &pKey, &pValue)) {
    if (!PyUnicode_Check(pKey) || !PyLong_Check(pValue)) {
      error() << "Failed retriving python key-value pair (string-int)\n";
      continue;
    }
    outputMap[PyUnicode_AsUTF8(pKey)] = PyLong_AsLong(pValue);
  }
}

template <>
void ConfigManager::GetMap<std::string, float>(std::string name, std::map<std::string, float> &outputMap) {
  PyObject *pythonDict = GetPythonDict(name);

  PyObject *pKey, *pValue;
  Py_ssize_t pos = 0;

  while (PyDict_Next(pythonDict, &pos, &pKey, &pValue)) {
    if (!PyUnicode_Check(pKey) || (!PyFloat_Check(pValue) && !PyLong_Check(pValue))) {
      error() << "Failed retriving python key-value pair (string-float)\n";
      continue;
    }
    outputMap[PyUnicode_AsUTF8(pKey)] = PyFloat_AsDouble(pValue);
  }
}

template <>
void ConfigManager::GetMap<string, vector<string>>(string name, map<string, vector<string>> &outputMap) {
  PyObject *pythonDict = GetPythonDict(name);

  PyObject *pKey, *pValue;
  Py_ssize_t pos = 0;

  while (PyDict_Next(pythonDict, &pos, &pKey, &pValue)) {
    if (!PyUnicode_Check(pKey) || (!PyList_Check(pValue) && !PyTuple_Check(pValue))) {
      error() << "Failed retriving python key-value pair (string-vector<string>)\n";
      continue;
    }
    vector<string> outputVector;
    for (Py_ssize_t i = 0; i < GetCollectionSize(pValue); ++i) {
      PyObject *item = GetItem(pValue, i);
      outputVector.push_back(PyUnicode_AsUTF8(item));
    }
    outputMap[PyUnicode_AsUTF8(pKey)] = outputVector;
  }
}