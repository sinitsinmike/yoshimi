#!/usr/bin/env python

def increment(line):
    numbertext = line[len(endstring):]
    if numbertext > " ":
        return str(int(numbertext) + 1)
    else:
        return "0"


f = open("Misc/ConfBuild.cpp","r+")
line = "start"
found = 0
endstring = "#define BUILD_NUMBER"
text = "0"
while line != "" and found == 0:
    mark = f.tell()
    line = f.readline()
    if endstring in line:
        found = mark
        text = increment(line)

if found == 0:
    found = mark
f.seek(found)
f.write("#define BUILD_NUMBER " + text)
f.truncate()
f.close()
