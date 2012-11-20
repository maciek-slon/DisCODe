#! /usr/bin/env python

# Copyright (c) 2012 Warsaw Univeristy of Technology
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

import sys
import os
import logging
import yaml
import pwd
import tempfile
import shutil

logging.basicConfig(level=logging.DEBUG)

from discode_helper import *

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
	
	classname = "Base::DataStreamIn"
	for s in inputs:
		TMPLFields += "\t{}<{}> {};\n".format(classname, s['type'], s['name'])
		TMPLPrepInterface += '\tregisterStream("{0}", &{0});\n'.format(s['name'])
		
	classname = "Base::DataStreamOut"
	for s in outputs:
		TMPLFields += "\t{}<{}> {};\n".format(classname, s['type'], s['name'])
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
		TMPLPrepInterface += '\th_{0}->setup(boost::bind(&{1}::{0}, this));\n'.format(h['name'], cmp_name)
		TMPLPrepInterface += '\tregisterHandler("{0}", &h_{0});\n'.format(h['name'])
		TMPLMethodsCode += "void {}::{}()".format(cmp_name, h['name']) + " {\n}\n\n"
		if 'triggeredBy' in h:
			for d in h['triggeredBy']:
				TMPLPrepInterface += '\taddDependency("{}", &{})\n'.format(h['name'], d)
		else:
			TMPLPrepInterface += '\taddDependency("{}", NULL)\n'.format(h['name'])
		
	dic['%TMPLFields%'] = dic['%TMPLFields%'] + TMPLFields
	dic['%TMPLPrepInterface%'] = dic['%TMPLPrepInterface%'] + TMPLPrepInterface
	dic['%TMPLMethodsHeaders%'] = dic['%TMPLMethodsHeaders%'] + TMPLMethodsHeaders
	dic['%TMPLMethodsCode%'] = dic['%TMPLMethodsCode%'] + TMPLMethodsCode
	
	return dic

########################################################################

def createComponent(cmp_name, dcl_name, dcl_path):
	dir = os.path.join(dcl_path, 'src/Components/', cmp_name)
	if os.path.exists(dir):
		logging.error("Folder already exists! Choose different name of component.")
		sys.exit()
	else:
		logging.debug("Creating [{}] in [{}]".format(cmp_name, dir))
	
		
	dic = {}
	if (os.path.exists(cmp_name+".yml")):
		dic = prepareDicFromFile(cmp_name+".yml")
	else:
		dic = prepareDefaultDic(cmp_name)

	tmp_dir = ""
	
	try:
		tmp_dir = tempfile.mkdtemp()
		src_dir = DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component'
		configure_file(src_dir+'/TemplateComponent.hpp.tmpl', tmp_dir+'/'+cmp_name+'.hpp', dic)
		configure_file(src_dir+'/TemplateComponent.cpp.tmpl', tmp_dir+'/'+cmp_name+'.cpp', dic)
		configure_file(src_dir+'/CMakeLists.txt', tmp_dir+'/CMakeLists.txt', dic)

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

if __name__ == '__main__':  
	if (len(sys.argv) != 3) or (sys.argv[1] == "--help"):
		# stop the program and print an error message
		sys.exit("Usage: " + os.path.basename(sys.argv[0]) + " DCL_NAME COMPONENT_NAME")

	dcl_name = sys.argv[1]
	dcl_path = getDclDir(dcl_name)
	if (dcl_path == ""):
		logging.error("DCL [{}] doesn'n exist!".format(dcl_name))
		sys.exit()
	dcl_name = os.path.basename(dcl_path)
	cmp_name = sys.argv[2]

	createComponent(cmp_name, dcl_name, dcl_path)
