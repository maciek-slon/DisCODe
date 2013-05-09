import re
import logging
import os
import pwd
import tempfile
import shutil
import sys

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

########################################################################

def getEmptyDic():
	return {
		# class name
		'TemplateComponent'    : "",
		# header guard
		'TEMPLATECOMPONENT'    : "",
		# name of component
		'%TMPLName%'           : "",
		# current user name
		'%TMPLAuthor%'         : "", 
		# components description
		'%TMPLDescription%'    : "",
		# dependencies
		'%TMPLDeps%'           : "",
		# additional fields
		'%TMPLFields%'         : "",
		# methods declaration
		'%TMPLMethodsHeaders%' : "",
		# initializer list
		'%TMPLInitializer%'    : "",
		# constructor body
		'%TMPLConstructor%'    : "",
		# interface preparation
		'%TMPLPrepInterface%'  : "",
		# component initialization
		'%TMPLInit%'           : "",
		# methods definition
		'%TMPLMethodsCode%'    : "", 
		# CMake packages to find
		'%TMPLCMakePkg%'       : "",
		# CMake libraries to link
		'%TMPLCMakeLibs%'      : "",
	}

########################################################################

def prepareDicFromFile(file_name):
	stream = open(file_name, 'r')
	s = yaml.load(stream)
	
	dic = getEmptyDic()
	
	dic['TemplateComponent'] = s['name']
	dic['TEMPLATECOMPONENT'] = s['name'].upper()
	dic['%TMPLName%'] = s['name']
	dic['%TMPLAuthor%'] = s['author']
	dic['%TMPLDescription%'] = s['description']
	
	dic = prepareStreams(dic, s['inputs'], s['outputs'])
	dic = prepareHandlers(dic, s['handlers'], s['name'])
	
	return dic

########################################################################

def prepareDefaultDic(cmp_name):
	dic = getEmptyDic()
	
	dic['TemplateComponent'] = cmp_name
	dic['TEMPLATECOMPONENT'] = cmp_name.upper()
	dic['%TMPLName%'] = cmp_name
	dic['%TMPLAuthor%'] = pwd.getpwuid( os.getuid() ).pw_gecos
	dic['%TMPLDescription%'] = cmp_name + " processor."
	
	return dic

########################################################################

def prepareStreams(dic, inputs, outputs):
	TMPLFields = "\t// Data streams\n"
	TMPLPrepInterface = "\t// Register data streams\n"
	
	if inputs:
		classname = "Base::DataStreamIn"
		for s in inputs:
			TMPLFields += "\t{}< {} > {};\n".format(classname, s['type'], s['name'])
			TMPLPrepInterface += '\tregisterStream("{0}", &{0});\n'.format(s['name'])
		
	if outputs:
		classname = "Base::DataStreamOut"
		for s in outputs:
			TMPLFields += "\t{}< {} > {};\n".format(classname, s['type'], s['name'])
			TMPLPrepInterface += '\tregisterStream("{0}", &{0});\n'.format(s['name'])
		
	dic['%TMPLFields%'] = dic['%TMPLFields%'] + TMPLFields + "\n"
	dic['%TMPLPrepInterface%'] = dic['%TMPLPrepInterface%'] + TMPLPrepInterface + "\n"
	
	return dic

########################################################################

def prepareHandlers(dic, handlers, cmp_name):
	TMPLFields = "\t// Handlers\n"
	TMPLPrepInterface = "\t// Register handlers\n"
	TMPLMethodsHeaders = "\t// Handlers\n"
	TMPLMethodsCode = ""
	
	for h in handlers:
		TMPLFields += "\tBase::EventHandler2 h_{};\n".format(h['name'])
		TMPLMethodsHeaders += "\tvoid {}();\n".format(h['name'])
		TMPLPrepInterface += '\th_{0}.setup(boost::bind(&{1}::{0}, this));\n'.format(h['name'], cmp_name)
		TMPLPrepInterface += '\tregisterHandler("{0}", &h_{0});\n'.format(h['name'])
		TMPLMethodsCode += "void {}::{}()".format(cmp_name, h['name']) + " {\n}\n\n"
		if 'triggeredBy' in h:
			for d in h['triggeredBy']:
				TMPLPrepInterface += '\taddDependency("{}", &{});\n'.format(h['name'], d)
		else:
			TMPLPrepInterface += '\taddDependency("{}", NULL);\n'.format(h['name'])
		
	dic['%TMPLFields%'] = dic['%TMPLFields%'] + TMPLFields
	dic['%TMPLPrepInterface%'] = dic['%TMPLPrepInterface%'] + TMPLPrepInterface
	dic['%TMPLMethodsHeaders%'] = dic['%TMPLMethodsHeaders%'] + TMPLMethodsHeaders
	dic['%TMPLMethodsCode%'] = dic['%TMPLMethodsCode%'] + TMPLMethodsCode
	
	return dic

########################################################################

def createComponent(cmp_name, dcl_name, dcl_path, dic):
	dir = os.path.join(dcl_path, 'src/Components/', cmp_name)
	if os.path.exists(dir):
		logging.error("Folder already exists! Choose different name of component.")
		sys.exit()
	else:
		logging.debug("Creating [{}] in [{}]".format(cmp_name, dir))
	
		
	

	tmp_dir = ""
	
	try:
		tmp_dir = tempfile.mkdtemp()
		src_dir = DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component'
		configure_file(src_dir+'/TemplateComponent.hpp.tmpl', tmp_dir+'/'+cmp_name+'.hpp', dic)
		configure_file(src_dir+'/TemplateComponent.cpp.tmpl', tmp_dir+'/'+cmp_name+'.cpp', dic)
		configure_file(src_dir+'/CMakeLists.txt.tmpl', tmp_dir+'/CMakeLists.txt', dic)

		# Adding new component to DCL CMakeLists file
		cmakefile = open(dcl_path+'/src/Components/CMakeLists.txt', "a")
		cmakefile.write("\nADD_COMPONENT("+cmp_name+")\n")
		cmakefile.close()
		
		os.makedirs(dir)
	
		shutil.copy(os.path.join(tmp_dir, cmp_name+'.hpp'), os.path.join(dir, cmp_name+'.hpp'))
		shutil.copy(os.path.join(tmp_dir, cmp_name+'.cpp'), os.path.join(dir, cmp_name+'.cpp'))
		shutil.copy(os.path.join(tmp_dir, 'CMakeLists.txt'), os.path.join(dir, 'CMakeLists.txt'))
		shutil.rmtree(tmp_dir)
	
		logging.debug("Component created.")
	
	except Exception, err:
		if (tmp_dir != ""):
			try:
				shutil.rmtree(tmp_dir)
			except Exception, err2:
				pass
				
		if (os.path.exists(dir)):
			try:
				shutil.rmtree(dir)
			except Exception, err2:
				pass
		
		logging.exception("Can't create component [{}]".format(cmp_name))
		sys.exit()

########################################################################
