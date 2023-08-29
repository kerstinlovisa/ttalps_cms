import argparse
import shutil

args = argparse.ArgumentParser()
args.add_argument("--name", help="Name of the class/app to add", required=True)
args.add_argument("--type", help="Type of the extension to add: PhysicsObject (po), app", required=True)
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
  new_string += f"  return std::static_pointer_cast<{class_name}>(physicsObject);\n"
  new_string += "}\n\n"
  new_string += "#endif /* ExtensionsHelpers_hpp */\n"
  
  replace_string_in_file(file_path, old_string, new_string)
  
  
def main():
    
    class_name = args.name
    class_type = args.type
    
    if class_type == "PhysicsObject" or class_type == "po":
      print(f"Adding a PhysicsObject class: {class_name}")
      
      source_file = "templates/PhysicsObject.template.cpp"
      destination_file = f"libs/extensions/src/{class_name}.cpp"
      shutil.copy(source_file, destination_file)
      replace_string_in_file(destination_file, "TemplateName", class_name)
      
      source_file = "templates/PhysicsObject.template.hpp"
      destination_file = f"libs/extensions/include/{class_name}.hpp"
      shutil.copy(source_file, destination_file)
      replace_string_in_file(destination_file, "TemplateName", class_name)
      
      insert_cast(class_name)
      

if __name__ == "__main__":
    main()
