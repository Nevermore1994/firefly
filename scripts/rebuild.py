import os
import shutil
import sys


def delete_cmake_cache(file_path):
    for root_path, dirs, files in os.walk(file_path):
        for file in files:
            if file.find("CMakeCache") >= 0 or \
                file.find("cmake_install") >= 0 or \
                file.find(".a") >= 0 or \
                file.find("Makefile") >= 0 or \
                file.find(".dylib") >= 0:
                print("remove files:" + os.path.join(root_path, file))
                os.remove(os.path.join(root_path, file))

        for path in dirs:
            if path.find("CMakeFiles") >= 0:
                print("remove dirs:" + os.path.join(root_path, path))
                shutil.rmtree(os.path.join(root_path, path))


def get_all_source_file(dir_path):
    header_files = []
    source_files = []
    for _, dirs, files in os.walk(dir_path):
        for file in files:
            if file.find(".hpp") >= 0 or file.find(".h") >= 0:
                header_files.append(os.path.join(dir_path, file) + " ")
            elif file.find(".cpp") >= 0 or file.find(".cc") >= 0 or file.find(".c"):
                source_files.append(os.path.join(dir_path, file) + " ")
        for path in dirs:
            sub_files = get_all_source_file(os.path.join(dir_path, path))
            header_files.extend(sub_files[0])
            source_files.extend(sub_files[1])

    return tuple((header_files, source_files))


def get_target_path():
    path = os.getcwd() + "/bin/"
    target_paths = []
    for _, dirs, files in os.walk(path):
        for file in files:
            if file.find(".a") >= 0:
                target_paths.append(os.path.join(path, file))
    return target_paths


os.system('cd ..')
root_path = os.getcwd()
print(root_path)
delete_cmake_cache(root_path)

res = 0
exec_path = "./bin/" + os.path.dirname(__file__)
if len(sys.argv) >= 2:
    param = sys.argv[1]
    if param != "-r" and param != "-b" and param != "-i":
        print("error arguments")
        exit(0)
    res = os.system("cmake .")
    res = os.system("cmake --build .")
    if res != 0:
        print("build error!!!!")
        exit(-1)
    if param == "-r":
        res = os.popen("chmod 777 " + exec_path)
        res = os.system(exec_path)
    elif param == "-i":
        res = os.system("sudo make install")
