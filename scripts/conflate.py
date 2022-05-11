import shutil
import os
import sys

def get_all_path_name(file_path) -> list:
    list = []
    for p in os.listdir(file_path):
        if os.path.isdir(p):
            list.append(p)
    return list

def splicing_commond(paths, name) -> str:
    s = "xcrun xcodebuild -create-xcframework \\"
    for path in paths:
        s += "-framework ./{0}/{1}.framework \\".format(path, name)
    s += "-output {0}.xcframework".format(name)
    return s


def processing():
    #if len(sys.argv) < 2:
    #   exit()
    path = os.getcwd()
    name = "firefly"
    output_path = "{0}/{1}.xcframework".format(path, name)
    if os.path.exists(output_path):
        shutil.rmtree(output_path)
    
    sub_paths = get_all_path_name(path)
    commond = splicing_commond(sub_paths, name)
    res = os.system(commond)
    if res == 0:
        print("The following frameworks were merged:")
        for p in sub_paths:
            print("{0}".format(p))
    else:
        print("error!!!")

processing()




