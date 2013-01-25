import sys
import os
import re

def getDclDir(dcl_name):
	if dcl_name == "":
		return ""

	dcl_dir = os.getenv("DISCODE_DCL_DIR", "")
	if (dcl_dir == ""):
		print >> sys.stderr, "Warning: DISCODE_DCL_DIR not set. Searching only in current directory."
		dcl_dir = "./"

	dcl_dir = os.path.abspath(dcl_dir)

	#print "Looking for DCL [" , dcl_name , "] in" , dcl_dir

	# Check if provided directory contains CMakeLists.txt file
	# which means, that it is probably an DCL
	if os.path.exists(os.path.join(dcl_dir, dcl_name, "CMakeLists.txt")):
		#print os.path.join(dcl_dir, dcl_name)
		return os.path.join(dcl_dir, dcl_name)
	else:
		return ""


def getDependencies(dcl_dir):
	fpath = os.path.join(dcl_dir, "src", "CMakeLists.txt")

	# Read CMakeLists file contents
	fin = open(fpath, "r")
	str = fin.read()
	fin.close()

	return re.findall(r'FIND_PACKAGE[\s]*\([\s]*([^\s]*)', str)


def generateDocumentation(dcl_name, dcl_dir):
	print "Generating documentation for [", dcl_name , "] in" , dcl_dir
	print "DCL:", dcl_name
	print "Depends on:"
	deps = getDependencies(dcl_dir)
	for d in deps:
		print "\t" + d


def printUsage():
	s = """
Usage: 

discode_doc [DCL_NAME]

If DCL_NAME is specified, then documentation is generated for this library,
otherwise documentation is created for DCL in current directory.
"""
	print s



if __name__ == '__main__': 
	# check if proper argument is given
	dcl_dir = ""
	dcl_name = ""
	if len(sys.argv) == 2:
		# show usage information
		if (sys.argv[1] == '--help'):
			printUsage()
			sys.exit()
		# check if specified DCL exists
		dcl_name = sys.argv[1]
		dcl_dir = getDclDir(dcl_name)
		if (dcl_dir == ""):
			print "DCL [" , dcl_name , "] doesn't exist."
			sys.exit()

	
	generateDocumentation(dcl_name, dcl_dir)		

