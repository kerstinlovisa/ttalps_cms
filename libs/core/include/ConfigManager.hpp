//  ConfigManager.hpp
//
//  Created by Jeremi Niedziela on 09/08/2023.

#ifndef ConfigManager_hpp
#define ConfigManager_hpp

#include <Python.h>

#include "Helpers.hpp"

class ConfigManager {
 public:
  ConfigManager(std::string configPath);
  ~ConfigManager();

  template <typename T>
  void GetValue(std::string name, T &outputValue);

  template <typename T>
  void GetVector(std::string name, std::vector<T> &outputVector);

  template <typename T, typename U>
  void GetMap(std::string name, std::map<T, U> &outputMap);

 private:
  FILE *pythonFile;
  PyObject *pythonModule;
  PyObject *config;

  PyObject *GetPythonValue(std::string name);
  PyObject *GetPythonList(std::string name);
  PyObject *GetPythonDict(std::string name);
};

template <typename T>
void ConfigManager::GetVector(std::string name, std::vector<T> &outputVector) {
  PyObject *pythonList = GetPythonList(name);

  for (Py_ssize_t i = 0; i < PyList_Size(pythonList); ++i) {
    PyObject *item = PyList_GetItem(pythonList, i);

    if (std::is_same<T, std::string>::value) {
      if (!item || !PyUnicode_Check(item)) {
        PyErr_Print();
        continue;
      }
      std::string value = PyUnicode_AsUTF8(item);
      outputVector.push_back(value);
    } else {
      error() << "Unknown type encountered in ConfigManager::GetVector()\n";
    }
  }
}

#endif /* ConfigManager_hpp */
