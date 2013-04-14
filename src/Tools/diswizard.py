#!/usr/bin/env python

from PySide import QtGui  
from PySide import QtCore
from PySide import QtUiTools

from diswizard_library import LibraryWidget
from Library import Library

import sys
import os
import re

# Absolute path of directory DisCODe was installed in
DISCODE_PATH="@CMAKE_INSTALL_PREFIX@"
DISCODE_DCL_DIR = os.environ["DISCODE_DCL_DIR"]

class Handler:
	def __init__(self):
		self.name = "<New Handler>"
		self.type = 0 #0 - active, 1 - passive, 2 - triggered
		self.deps = []


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
		self.resize(500, 500)

		ICON_PATH=DISCODE_PATH+"/share/DisCODe/resources/icons/10/"

		self.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"028.png"))
		self.ui.lblInfo.setText("Info about current focus")

		widgets = self.findChildren(QtGui.QWidget);
		for w in widgets:
			w.installEventFilter(self)
			
		self.lastText = ""
		self.lastIcon = None
		self.infoVisible = False
		self.setWindowTitle("DisCODe Wizard")

			
	def eventFilter(self, object, event):
		ICON_PATH=DISCODE_PATH+"/share/DisCODe/resources/icons/10/"
		
		if (event.type() == QtCore.QEvent.Leave) and self.infoVisible:
			self.ui.lblInfo.setText(self.lastText)
			self.ui.lblInfoIcon.setPixmap(self.lastIcon)
			self.infoVisible = False
			
		if (event.type() == QtCore.QEvent.Enter) and (object.toolTip() != ""):
			if not self.infoVisible:
				self.lastText = self.ui.lblInfo.text() 
				self.lastIcon = self.ui.lblInfoIcon.pixmap().copy()
			self.infoVisible = True
			self.ui.lblInfo.setText(object.toolTip())
			self.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"028.png"))

		return QtGui.QWidget.eventFilter(self, object, event)

    
class DisCODeWizard(object):
	def __init__(self):
		self.app = QtGui.QApplication(sys.argv)  
		self.win = MyWidget()
		
		QtCore.QObject.connect(self.win.ui.btnGenerate, QtCore.SIGNAL('clicked()'), self.generate)
		
		QtCore.QObject.connect(self.win.ui.btnAddDep, QtCore.SIGNAL('clicked()'), self.addDep)
		
		QtCore.QObject.connect(self.win.ui.btnAddStreamIn, QtCore.SIGNAL('clicked()'), lambda: self.addStream(self.win.ui.tblStreamIn))
		QtCore.QObject.connect(self.win.ui.btnRemoveStreamIn, QtCore.SIGNAL('clicked()'), lambda: self.remStream(self.win.ui.tblStreamIn))
		QtCore.QObject.connect(self.win.ui.btnAddStreamOut, QtCore.SIGNAL('clicked()'), lambda: self.addStream(self.win.ui.tblStreamOut))
		QtCore.QObject.connect(self.win.ui.btnRemoveStreamOut, QtCore.SIGNAL('clicked()'), lambda: self.remStream(self.win.ui.tblStreamOut))
		
		QtCore.QObject.connect(self.win.ui.btnAddProp, QtCore.SIGNAL('clicked()'), self.addProp)
		QtCore.QObject.connect(self.win.ui.btnRemoveProp, QtCore.SIGNAL('clicked()'), self.remProp)
		
		self.app.connect(self.app, QtCore.SIGNAL("lastWindowClosed()"), self.app, QtCore.SLOT("quit()"))
		self.app.connect(self.win.ui.btnCancel, QtCore.SIGNAL('clicked()'), self.app, QtCore.SLOT("quit()"))

		self.win.ui.btnBrowseDCL.hide()

		self.win.ui.btnAddHandler.clicked.connect(self.addHandler)
		
		self.win.ui.tblStreamIn.itemChanged.connect(self.processStreams)
		
		self.win.ui.lstHandlers.currentItemChanged.connect(self.getCurrentHandler)
		self.win.ui.lstHandlers.itemChanged.connect(self.changeHandler)
		
		self.win.ui.rdAct.toggled.connect(self.changeHandlerProps)
		self.win.ui.rdPas.toggled.connect(self.changeHandlerProps)
		self.win.ui.rdTri.toggled.connect(self.changeHandlerProps)
		self.win.ui.lstHandDeps.itemChanged.connect(self.changeHandlerProps)
		
		self.currentHandler = None
		self.processHandlers()
		
		self.libs = LibraryWidget(None)
		libs = []
		lib = Library()
		lib.name="OpenCV"
		lib.desc="Open source compute vision library"
		lib.pckg="OpenCV"
		lib.libs="${OpenCV_LIBS}"
		lib.incl="opencv2/opencv.hpp"
		libs.append(lib)
		lib = Library()
		lib.name="Pointcloud"
		libs.append(lib)
		self.libs.setLibraries(libs)

		self.loadDCL()
		
		self.handlers=[]
		
		#self.showMessage("Error!", "E")
		self.showMessage("", "")
		
	def showMessage(self, minfo, mtype):
		ICON_PATH=DISCODE_PATH+"/share/DisCODe/resources/icons/10/"
		self.messageInfo = minfo
		self.messageType = mtype
		
		self.win.ui.lblInfo.setText(minfo)
		if mtype == "E":
			self.win.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"150.png"))
		if mtype == "W":
			self.win.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"050.png"))
		if mtype == "":
			self.win.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"000.png"))
		
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
			stream_type = tbl.cellWidget(row,2).currentText()
			if (stream_type == "Input"):
				stream_type = "DataStreamIn"
			else:
				stream_type = "DataStreamOut"
				
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
			print "Creating component", cmp_name, "in", dir
		
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
		 
		#os.makedirs(dir)
		 
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
		
		print dic

		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/Component.hpp', dir+'/'+cmp_name+'.hpp', dic)
		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/Component.cpp', dir+'/'+cmp_name+'.cpp', dic)
		#configure_file(DISCODE_PATH+'/share/DisCODe/Templates/src/Components/Component/CMakeLists.txt', dir+'/CMakeLists.txt', dic)

		#===============================================================================
		# Adding new component to DCL CMakeLists file
		#===============================================================================
		#cmakefile = open(fullpath+'/src/Components/CMakeLists.txt', "a")
		#cmakefile.write("\nADD_COMPONENT("+cmp_name+")\n")
		#cmakefile.close()
		
		
		
	def addDep(self):
		self.libs.show()
		
	def remDep(self):
		pass
		
		
	def addProp(self):
		self.win.ui.tblProps.insertRow(self.win.ui.tblProps.rowCount())
		
	def remProp(self):
		self.win.ui.tblProps.removeRow(self.win.ui.tblProps.currentRow())

	def addStream(self, table):
		rc = table.rowCount()
		table.insertRow(rc)
		table.setItem(rc, 0, QtGui.QTableWidgetItem("stream_%d"%(rc)))
		#self.processStreams()
		
	def remStream(self, table):
		table.removeRow(table.currentRow())
		self.processStreams()
	
	def addHandler(self):
		handler = Handler()
		self.handlers.append(handler)
		item = QtGui.QListWidgetItem( handler.name );
		item.setFlags( item.flags() | QtCore.Qt.ItemIsEditable );
		self.win.ui.lstHandlers.addItem(item)
		self.win.ui.lstHandlers.setCurrentItem(item)
		self.win.ui.lstHandlers.editItem(item)
		pass
		
	def remHandler(self):
		pass
		
	def processStreams(self):
		self.win.ui.lstHandDeps.clear()
		for i in range(self.win.ui.tblStreamIn.rowCount()):
			txt = self.win.ui.tblStreamIn.item(i, 0).text()
			item = QtGui.QListWidgetItem(txt, self.win.ui.lstHandDeps)
			item.setFlags(item.flags() | QtCore.Qt.ItemIsUserCheckable)
			item.setCheckState(QtCore.Qt.Unchecked)

	def checkHandDeps(self):
		for i in range(self.win.ui.lstHandDeps.count()):
			item = self.win.ui.lstHandDeps.item(i)
			if item.text() in self.currentHandler.deps:
				item.setCheckState(QtCore.Qt.Checked)
			else:
				item.setCheckState(QtCore.Qt.Unchecked)

	def getHandler(self, name):
		for hand in self.handlers:
			if hand.name == name:
				return hand
		
		return None

	def getCurrentHandler(self):
		self.currentHandler = self.getHandler(self.win.ui.lstHandlers.currentItem().text())
		if self.currentHandler != None:
			print "Cur hand:", self.currentHandler.name, self.currentHandler.type, self.currentHandler.deps
		self.processHandlers()
		self.checkHandDeps()

	def changeHandlerProps(self):
		if self.currentHandler == None:
			return
		if self.win.ui.rdAct.isChecked():
			self.currentHandler.type = 0
			self.win.ui.lstHandDeps.setEnabled(False)
		if self.win.ui.rdPas.isChecked():
			self.currentHandler.type = 1
			self.win.ui.lstHandDeps.setEnabled(False)
		if self.win.ui.rdTri.isChecked():
			self.currentHandler.type = 2
			self.win.ui.lstHandDeps.setEnabled(True)
		for i in range(self.win.ui.lstHandDeps.count()):
			item = self.win.ui.lstHandDeps.item(i);
			text = item.text()
			if (item.checkState() == QtCore.Qt.Checked) and not (text in self.currentHandler.deps):
				self.currentHandler.deps.append(text)
			if (item.checkState() == QtCore.Qt.Unchecked) and (text in self.currentHandler.deps):
				self.currentHandler.deps.remove(text)

	def processHandlers(self):
		if self.currentHandler == None:
			self.currentHandler = None
			self.win.ui.rdAct.setEnabled(False)
			self.win.ui.rdAct.setChecked(False)
			self.win.ui.rdPas.setEnabled(False)
			self.win.ui.rdPas.setChecked(False)
			self.win.ui.rdTri.setEnabled(False)
			self.win.ui.rdTri.setChecked(False)
			self.win.ui.lstHandDeps.setEnabled(False)
		else:		
			self.win.ui.rdAct.setEnabled(True)
			self.win.ui.rdAct.setChecked(self.currentHandler.type == 0)
			self.win.ui.rdPas.setEnabled(True)
			self.win.ui.rdPas.setChecked(self.currentHandler.type == 1)
			self.win.ui.rdTri.setEnabled(True)
			self.win.ui.rdTri.setChecked(self.currentHandler.type == 2)
			self.win.ui.lstHandDeps.setEnabled(self.currentHandler.type == 2)

	def changeHandler(self, item):
		self.currentHandler.name = item.text()
		
		print "Hands:"
		for h in self.handlers:
			print h.name, h.type, h.deps



if __name__ == '__main__':  
	import sys  
	import os
	print("Running in " + os.getcwd() + " .\n")
	app = DisCODeWizard()
	app.run()

