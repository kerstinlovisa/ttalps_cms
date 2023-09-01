import argparse
import shutil
import os

args = argparse.ArgumentParser()
args.add_argument("--name", help="Name of the class/app to add", required=True)
args.add_argument("--type", help="Type of the extension to add: PhysicsObject (po), HistogramFiller (hf), app", required=True)
args.add_argument("--path", help="In case type is app, specify directory name in which to put it", required=False, default="")
args = args.parse_args()

def replace_string_in_file(file_path, old_string, new_string):
  # Read the contents of the file
  with open(file_path, 'r') as file:
      content = file.read()

  # Replace all occurrences of the old string with the new string
  new_content = content.replace(old_string, new_string)

  # Write the modified content back to the file
  with open(file_path, 'w') as file:
      file.write(new_content)

def insert_cast(class_name):
  
  file_path = "libs/extensions/include/ExtensionsHelpers.hpp"
  
  old_string = "#include \"PhysicsObject.hpp\""
  new_string = f"#include \"{class_name}.hpp\"\n"
  new_string += "#include \"PhysicsObject.hpp\""
  replace_string_in_file(file_path, old_string, new_string)
  
  old_string = "#endif /* ExtensionsHelpers_hpp */"
  new_string = f"inline std::shared_ptr<{class_name}> as{class_name}(const std::shared_ptr<PhysicsObject> physicsObject) {{\n"
  new_string += f"  return std::make_shared<{class_name}>(physicsObject);\n"
  new_string += "}\n\n"
  new_string += "#endif /* ExtensionsHelpers_hpp */\n"
  
  replace_string_in_file(file_path, old_string, new_string)
  
def create_parent_directories(file_path):
  parent_dir = os.path.dirname(file_path)

  if not os.path.exists(parent_dir):
    os.makedirs(parent_dir)


def remove_path(path):
  if os.path.isdir(path):
    shutil.rmtree(path)
  elif os.path.isfile(path):
    os.remove(path)


def main():
    
  class_name = args.name
  class_type = args.type
  
  if class_type == "PhysicsObject" or class_type == "po":
    print(f"Adding a PhysicsObject class: {class_name}")
    
    source_file = "templates/PhysicsObject.template.cpp"
    destination_file = f"libs/extensions/src/{class_name}.cpp"
    shutil.copy(source_file, destination_file)
    replace_string_in_file(destination_file, "TemplateName", class_name)
    print(f"Added file: {destination_file}")
    
    source_file = "templates/PhysicsObject.template.hpp"
    destination_file = f"libs/extensions/include/{class_name}.hpp"
    shutil.copy(source_file, destination_file)
    replace_string_in_file(destination_file, "TemplateName", class_name)
    print(f"Added file: {destination_file}")
    
    insert_cast(class_name)
    print(f"Added conversion from PhysicsObject to {class_name} in libs/extensions/include/ExtensionsHelpers.hpp")
  elif class_type == "HistogramFiller" or class_type == "hf":
    print(f"Adding a HistogramFiller class: {class_name}")
    
    source_file = "templates/HistogramFiller.template.cpp"
    destination_file = f"libs/extensions/src/{class_name}.cpp"
    shutil.copy(source_file, destination_file)
    replace_string_in_file(destination_file, "TemplateName", class_name)
    print(f"Added file: {destination_file}")
    
    source_file = "templates/HistogramFiller.template.hpp"
    destination_file = f"libs/extensions/include/{class_name}.hpp"
    shutil.copy(source_file, destination_file)
    replace_string_in_file(destination_file, "TemplateName", class_name)
    print(f"Added file: {destination_file}")
  elif class_type == "app":
    print(f"Adding an app called: {class_name}")
    
    source_file = "templates/app.template.cpp"
    destination_file = f"apps/{args.path}/{class_name}.cpp"
    create_parent_directories(destination_file)
    shutil.copy(source_file, destination_file)
    print(f"Added file: {destination_file}")
    
    source_file = "templates/config.template.py"
    destination_file = f"configs/{args.path}/{class_name}.py"
    create_parent_directories(destination_file)
    shutil.copy(source_file, destination_file)
    print(f"Added file: {destination_file}")
  
  remove_path("build/CMakeFiles/")
  remove_path("build/CMakeCache.txt")
      
      
if __name__ == "__main__":
    main()
