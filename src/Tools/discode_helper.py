import re
import logging
import os

# Absolute path of directory DisCODe was installed in
DISCODE_PATH="@CMAKE_INSTALL_PREFIX@"

########################################################################

def replace_words(text, word_dic):
    """
    take a text and replace words that match a key in a dictionary with
    the associated value, return the changed text
    """
    rc = re.compile('|'.join(map(re.escape, word_dic)))
    def translate(match):
        return word_dic[match.group(0)]
    return rc.sub(translate, text)

########################################################################

def configure_file(in_name, out_name, word_dic):
    """
    take template file, replace words according to given dictionary and write result to another file
    """
    # Read template file
    fin = open(in_name, "r")
    str = fin.read()
    fin.close()
    
    # call the function and get the changed text
    str = replace_words(str, word_dic)
    
    # write changed text back out
    fout = open(out_name, "w")
    fout.write(str.encode('UTF-8'))
    fout.close()

########################################################################

def getDclDir(dcl_name):
	if dcl_name == "":
		return ""

	dcl_dir = os.getenv("DISCODE_DCL_DIR", "")
	if (dcl_dir == ""):
		logging.warning("Warning: DISCODE_DCL_DIR not set. Searching only in current directory.")
		dcl_dir = "./"

	dcl_dir = os.path.abspath(dcl_dir)

	logging.debug("Searching for [{}] in [{}]".format(dcl_name, dcl_dir));

	# Check if provided directory contains CMakeLists.txt file
	# which means, that it is probably an DCL
	if os.path.exists(os.path.join(dcl_dir, dcl_name, "CMakeLists.txt")):
		return os.path.join(dcl_dir, dcl_name)
	else:
		return ""
