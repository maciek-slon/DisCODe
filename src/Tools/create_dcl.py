#! /usr/bin/env python

import sys
import os
import re


def replace_words(text, word_dic):
    """
    take a text and <strong class="highlight">replace</strong> words that match 
    a key <strong class="highlight">in</strong> a dictionary with the associated
    value, return the changed text
    """
    rc = re.compile('|'.join(map(re.escape, word_dic)))
    def translate(match):
        return word_dic[match.group(0)]
    return rc.sub(translate, text)


def configure_file(in_name, out_name, word_dic):
    """
    take template file, replace words according to given dictionary and write 
    result to another file
    """
    # Read template file
    fin = open(in_name, "r")
    str = fin.read()
    fin.close()
    
    # call the function and get the changed text
    str = replace_words(str, word_dic)
    
    # write changed text back out
    fout = open(out_name, "w")
    fout.write(str)
    fout.close()


# Absolute path of directory DisCODe was installed in
DISCODE_PATH="@CMAKE_INSTALL_PREFIX@"

if len(sys.argv) != 2:
    # stop the program and print an error message
    sys.exit("Must provide path for new DCL!")

fullpath = os.path.abspath(sys.argv[1])
dcl_name = os.path.basename(fullpath)

# Create directory if it doesn't exist
if not os.path.exists(fullpath):
    os.makedirs(fullpath)

# Check if provided directory contains CMakeLists.txt file
# which means, that it is probably already an DCL
if os.path.exists(fullpath + "/CMakeLists.txt"):
    sys.exit("There is already DCL named " + dcl_name + " in current location!")


print "Creating DCL:" , dcl_name , "in" , fullpath 


#===============================================================================
# Preparing README file
#===============================================================================
readme_header = dcl_name + " - DisCODe Component Library"
readme_header += "\n" + "=" * len(readme_header)
 
readme_dic = {
'TEMPLATE_ARG_DCL_NAME' : readme_header
}

configure_file(DISCODE_PATH+'/share/DisCODe/Templates/README.md', fullpath+'/README.md', readme_dic)

#===============================================================================
# Preparing CMakeLists.txt file
#===============================================================================
cmakelists_dic = {
'TEMPLATE_ARG_DCL_NAME' : dcl_name
}

configure_file(DISCODE_PATH+'/share/DisCODe/Templates/CMakeLists.txt', fullpath+'/CMakeLists.txt', cmakelists_dic)



print dcl_name , "DCL created."
print "Please edit README file and provide all necessary information."
print "You can create new components by executing create_component command."

#===============================================================================
# Roll through existing directories and try to convert and add found components
#===============================================================================
cmakelists_dic = {
'INSTALL_PROCESSOR' : 'INSTALL_COMPONENT',
'INSTALL_SOURCE' : 'INSTALL_COMPONENT',
'INSTALL_SINK' : 'INSTALL_COMPONENT',
'INSTALL_PROXY' : 'INSTALL_COMPONENT'
}

cmakefile = open(fullpath+'/CMakeLists.txt', "a")

for f in os.listdir(fullpath):
    newpath = os.path.join(fullpath, f)
    if os.path.isdir(newpath):
        cmakepath = os.path.join(newpath, "CMakeLists.txt")
        if os.path.exists(cmakepath):
            print "Converting component: " + f
            configure_file(cmakepath, cmakepath, cmakelists_dic)
            cmakefile.write("\nADD_COMPONENT("+f+")\n")
            
cmakefile.close()