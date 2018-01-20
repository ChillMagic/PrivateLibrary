import os

file = open("prilib.h", "w")

file.write("// prilib.h\n")
file.write("// * PrivateLibrary\n")
file.write("// * This file is automatically generated.\n\n")
file.write("#pragma once\n#ifndef _PRILIB_H_\n#define _PRILIB_H_\n\n")
for f in os.listdir("include"):
    file.write("#include \"%s\"\n" % f)
file.write("\n#endif\n")
