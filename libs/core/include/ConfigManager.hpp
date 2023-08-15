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

  void GetExtraEventCollections(std::map<std::string, ExtraCollection> &extraEventCollections) {
    PyObject *pythonDict = GetPythonDict("extraEventCollections");

    PyObject *collectionName, *collectionSettings;
    Py_ssize_t pos = 0;

    while (PyDict_Next(pythonDict, &pos, &collectionName, &collectionSettings)) {
      if (!PyUnicode_Check(collectionName)) {
        error() << "Failed retriving python collection name (string)\n";
        continue;
      }
      PyObject *pyKey = nullptr;
      PyObject *pyValue = nullptr;
      Py_ssize_t pos2 = 0;
      ExtraCollection extraCollection;

      while (PyDict_Next(collectionSettings, &pos2, &pyKey, &pyValue)) {
        std::string keyStr = PyUnicode_AsUTF8(pyKey);
        if (keyStr == "inputCollections") {
          for (Py_ssize_t i = 0; i < GetCollectionSize(pyValue); ++i) {
            PyObject *item = GetItem(pyValue, i);
            extraCollection.inputCollections.push_back(PyUnicode_AsUTF8(item));
          }
        } else {
          PyObject *min = GetItem(pyValue, 0);
          PyObject *max = GetItem(pyValue, 1);
          extraCollection.selections[keyStr] = {PyFloat_AsDouble(min), PyFloat_AsDouble(max)};
        }
      }

      extraEventCollections[PyUnicode_AsUTF8(collectionName)] = extraCollection;
    }
  }

 private:
  FILE *pythonFile;
  PyObject *pythonModule;
  PyObject *config;

  PyObject *GetPythonValue(std::string name);
  PyObject *GetPythonList(std::string name);
  PyObject *GetPythonDict(std::string name);

  int GetCollectionSize(PyObject *collection);
  PyObject *GetItem(PyObject *collection, int index);
};

#endif /* ConfigManager_hpp */
