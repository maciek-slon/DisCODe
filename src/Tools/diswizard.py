#!/usr/bin/env python

from PySide import QtGui  
from PySide import QtCore
from PySide import QtUiTools

import sys
import os
import re

# Absolute path of directory DisCODe was installed in
DISCODE_PATH="@CMAKE_INSTALL_PREFIX@"
DISCODE_DCL_DIR = os.environ["DISCODE_DCL_DIR"]

def replace_words(text, word_dic):
	"""
	take a text and replace words that match a key in a dictionary with
	the associated value, return the changed text
	"""
	rc = re.compile('|'.join(map(re.escape, word_dic)))
	def translate(match):
		return word_dic[match.group(0)]
	return rc.sub(translate, text)


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
	fout.write(str)
	fout.close()




class MyWidget(QtGui.QMainWindow):
    def __init__(self, *args):  
       apply(QtGui.QMainWindow.__init__, (self,) + args)

       loader = QtUiTools.QUiLoader()
       file = QtCore.QFile(DISCODE_PATH+"/bin/diswizard.ui")
       file.open(QtCore.QFile.ReadOnly)
       self.ui = loader.load(file, self)
       file.close()

       self.setCentralWidget(self.ui)
       self.resize(640, 720)

    
class DisCODeWizard(object):
	def __init__(self):
		self.app = QtGui.QApplication(sys.argv)  
		self.win = MyWidget()
		
		QtCore.QObject.connect(self.win.ui.btnGenerate, QtCore.SIGNAL('clicked()'), self.generate)
		
		QtCore.QObject.connect(self.win.ui.btnAddStream, QtCore.SIGNAL('clicked()'), self.addStream)
		QtCore.QObject.connect(self.win.ui.btnRemoveStream, QtCore.SIGNAL('clicked()'), self.remStream)
		
		QtCore.QObject.connect(self.win.ui.btnAddProp, QtCore.SIGNAL('clicked()'), self.addProp)
		QtCore.QObject.connect(self.win.ui.btnRemoveProp, QtCore.SIGNAL('clicked()'), self.remProp)
		
		self.app.connect(self.app, QtCore.SIGNAL("lastWindowClosed()"), self.app, QtCore.SLOT("quit()"))
		self.app.connect(self.win.ui.btnCancel, QtCore.SIGNAL('clicked()'), self.app, QtCore.SLOT("quit()"))

		widgets = self.win.findChildren(QtGui.QWidget);
		for w in widgets:
			print w.objectName()

		self.loadDCL()
		
	def loadDCL(self):
		for o in os.listdir(DISCODE_DCL_DIR):
			print o
			if os.path.exists(DISCODE_DCL_DIR+"/"+o+"/src/Components/CMakeLists.txt"):
				self.win.ui.cbDCL.addItem(os.path.basename(o))
		
	def run(self):
		self.win.show()
		self.app.exec_()
		
		
	def generateStreams(self):
		tbl = self.win.ui.tblStreams
		allRows = tbl.rowCount()
		self.TMPLFields += "// Data streams\n\n"
		for row in xrange(0,allRows):
			stream_name = tbl.item(row,0).text()
			stream_data = tbl.item(row,1).text()
			stream_type = tbl.item(row,2).currentText()
			if (stream_type == "Input"):
				stream_type = "Base::DataStreamIn"
			else:
				stream_type = "Base::DataStreamOut"
				
			self.TMPLFields += "\t\tBase::%s<%s> %s;\n"%(stream_type, stream_data, stream_name)
			self.TMPLInit += "registerStream(\"%s\", &%s);"%(stream_name, stream_name)
	
	def generateEvents(self):
		pass
		
	def generateProperties(self):
		print "Properties:\n"
		props_header = ""
		props_initializer = ""
		props_constructor = ""
		
		tbl = self.win.ui.tblProps
		allRows = tbl.rowCount()
		for row in xrange(0,allRows):
			prop_name = tbl.item(row,0)
			prop_type = tbl.item(row,1)
			prop_default = tbl.item(row,2)
			prop_display = tbl.item(row,3)
			if (prop_display):
				prop_display = ", \"" + prop_display.text() + "\""
			else:
				prop_display = ""
			self.TMPLFields += "\t\tBase::Property<" + prop_type.text() + "> " + prop_name.text() + ";\n";
			self.TMPLInitializer += ", \n\t\t" + prop_name.text() + "(\"" + prop_name.text() + "\", " + prop_default.text() + prop_display + ")"
			self.TMPLConstructor += "\t\tregisterProperty(" + prop_name.text() + ");\n"
		
	def generate(self):

		fullpath = os.path.abspath(os.getcwd())
		dcl_name = self.win.ui.cbDCL.currentText()
		cmp_name = self.win.ui.edName.text()
		dcl_dir = os.environ["DISCODE_DCL_DIR"]
		print dcl_dir
		
		if dcl_name=="":
			print "DCL name can't be empty!"
			return
			
		if cmp_name=="":
			print "Component name can't be empty!"
			return
		
		dir = dcl_dir+'/'+dcl_name+'/src/Components/'+cmp_name
		print dir
		if not os.path.exists(dcl_dir+'/'+dcl_name):
			print "DCL %s doesn't exist!"%(dcl_name)
			return
		if os.path.exists(dir):
			print "Component already exists! Choose different name of component."
			return
		else:
			print "Creating component", cmp_name
		
		self.TMPLFields = ""
		self.TMPLMethodsHeaders = ""
		self.TMPLInitializer = ""
		self.TMPLConstructor = ""
		self.TMPLInit = ""
		self.TMPLMethodsCode = ""
		
		self.generateStreams()
		self.generateEvents()
		self.generateProperties()
				
		#===============================================================================
		# Preparing component source files
		#===============================================================================
		 
		os.makedirs(dir)
		 
		dic = {
		'TemplateComponent'  : cmp_name,
		'EXAMPLE'            : cmp_name.upper(),
		'TMPLFields'         : self.TMPLFields,
		'TMPLMethodsHeaders' : self.TMPLMethodsHeaders,
		'TMPLInitializer'    : self.TMPLInitializer,
		'TMPLConstructor'    : self.TMPLConstructor,
		'TMPLInit'           : self.TMPLInit,
		'TMPLMethodsCode'    : self.TMPLMethodsCode
		}

		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/Component.hpp', dir+'/'+cmp_name+'.hpp', dic)
		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/Component.cpp', dir+'/'+cmp_name+'.cpp', dic)
		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/CMakeLists.txt', dir+'/CMakeLists.txt', dic)

		#===============================================================================
		# Adding new component to DCL CMakeLists file
		#===============================================================================
		#cmakefile = open(fullpath+'/src/Components/CMakeLists.txt', "a")
		#cmakefile.write("\nADD_COMPONENT("+cmp_name+")\n")
		#cmakefile.close()
		
		
		
		
		
		
	def addProp(self):
		self.win.ui.tblProps.insertRow(self.win.ui.tblProps.rowCount())
		
	def remProp(self):
		self.win.ui.tblProps.removeRow(self.win.ui.tblProps.currentRow())

	def addStream(self):
		rc = self.win.ui.tblStreams.rowCount()
		self.win.ui.tblStreams.insertRow(rc)
		
		self.win.ui.tblStreams.setItem(rc, 0, QtGui.QTableWidgetItem("stream_%d"%(rc)))
		streamType = QtGui.QComboBox()
		streamType.addItem("Input")
		streamType.addItem("Output")
		self.win.ui.tblStreams.setCellWidget(rc, 2, streamType)
		
	def remStream(self):
		self.win.ui.tblStreams.removeRow(self.win.ui.tblStreams.currentRow())










if __name__ == '__main__':  
	import sys  
	import os
	print("Running in " + os.getcwd() + " .\n")
	app = DisCODeWizard()
	app.run()

