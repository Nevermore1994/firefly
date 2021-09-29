import os
import shutil
 
def delete_cmake_cache(file_path):
    for root_path, dirs, files in os.walk(file_path):
        for file in files:
            if file.find("CMakeCache") >= 0 or file.find("cmake_install") >= 0 or file.find(".a") >= 0:
                print ("remove files:" + os.path.join(root_path, file))
                os.remove(os.path.join(root_path, file))


        for path in dirs :
            if path.find("CMakeFiles") >= 0:
                print ("remove dirs:" + os.path.join(root_path, path))
                shutil.rmtree(os.path.join(root_path, path))
                
root_path = os.getcwd()
delete_cmake_cache(root_path)
