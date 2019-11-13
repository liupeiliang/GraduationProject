import sys

def CreateTest(className):
    f = open(className + "Test.cpp", "w")
    f.write("#include <gtest/gtest.h>\n")
    f.write("#include <bits/stdc++.h>\n")
    f.write("#include \"../art/" + className + ".h\"\n")
    f.write("\n");
    f.write("using namespace std;\n")
    

    f.write("class " + className + "Test : public ::testing::Test\n")
    f.write("{\n")
    f.write("protected:\n")
    f.write("void SetUp() override {}\n")
    f.write("void TearDown() override {}\n")
    f.write("};\n")
    f.write("\n")
    
    f.write("TEST_F(" + className + "Test, Test1)\n")
    f.write("{\n")
    f.write("}\n")

    f.close()

CreateTest(sys.argv[1])
