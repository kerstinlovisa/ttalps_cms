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
  void GetVector(std::string name, std::vector<T> &outputVector);

 private:
  FILE *pythonFile;
  PyObject *pythonModule;
  PyObject *pythonDict;

  PyObject *GetPythonList(std::string name);
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
      outputVector.push_back(PyUnicode_AsUTF8(item));
    } else {
      error() << "Unknown type encountered in ConfigManager::GetVector()\n";
    }
  }
}

#endif /* ConfigManager_hpp */
