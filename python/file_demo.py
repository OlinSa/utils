import os
import sys
import shutil

if __name__ == "__main__":
    print("===========================")
    print("filename:{}".format(sys.argv[0]))
    print("workDir:{}".format(os.getcwd()))
    print("listfile:{}".format(os.listdir(".")))
    print("path:{}".format(os.path.dirname(sys.argv[0])))
    print("basename:{}".format(os.path.basename(sys.argv[0])))
    print("extension:{}".format(os.path.splitext(sys.argv[0])))
    print("===========================")
    
    dir_demo = "dir_demo"
    file_demo = "file_demo"

    if not os.path.exists(file_demo):
        f= open(file_demo, "w", encoding="utf-8")
        f.write("file_demo")
        f.close()

    print("state({}):{}".format(file_demo, os.stat(file_demo)))
    print("fileSize({}):{}".format(file_demo, os.path.getsize(file_demo)))
    
    print("copy({}) to {}".format(file_demo, file_demo+".bak"))
    shutil.copyfile(file_demo, file_demo+".bak")
    
    print("set env:{}".format("ENV_DEMO"))
    os.putenv("TEST_ENV", "ENV_DEMO")
    print("env:{}".format(os.getenv("TEST_ENV")))