import sys

def CreateHeader(className):
    f = open(className + ".h", "w")
    mark = "_" + className + "_H"
    f.write("#ifndef " + mark + "\n")
    f.write("#define " + mark + "\n")
    f.write("\n")
    f.write("class " + className + "{\n")
    f.write("public:\n")
    f.write("  " + className + "();\n")
    f.write("  ~" + className + "();\n")
    f.write("\n")
    f.write("private:\n")
    f.write("};\n")
    f.write("\n")
    
    f.write(className + "::" + className + "()\n")
    f.write("{\n")
    f.write("}\n")
    f.write("\n")
    f.write(className + "::~" + className + "()\n")
    f.write("{\n")
    f.write("}\n")
    
    f.write("#endif //" + mark + "\n")

    f.close()

CreateHeader(sys.argv[1])
