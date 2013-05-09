#!/usr/bin/env python

from PySide import QtGui  
from PySide import QtCore
from PySide import QtUiTools

from Library import Library

import sys
import os
import re

# Absolute path of directory DisCODe was installed in
DISCODE_PATH="@CMAKE_INSTALL_PREFIX@"


class LibraryWidget(QtGui.QWidget):
	def __init__(self, parent):  
		super(LibraryWidget, self).__init__(parent)

		loader = QtUiTools.QUiLoader()
		file = QtCore.QFile(DISCODE_PATH+"/bin/diswizard_library.ui")
		file.open(QtCore.QFile.ReadOnly)
		self.ui = loader.load(file, self)
		file.close()

		layout = QtGui.QVBoxLayout()
		layout.addWidget(self.ui)
		layout.setContentsMargins (0, 9, 0, 9)
		self.setLayout(layout)
		
		pal = self.palette()
		pal.setColor(QtGui.QPalette.Window, QtGui.QColor(255, 255, 255))
		pal.setColor(QtGui.QPalette.Button, QtGui.QColor(255, 255, 255))
		self.setPalette(pal)
		
		ICON_PATH=DISCODE_PATH+"/share/DisCODe/resources/icons/10/"

		self.ui.lstLibs.currentItemChanged.connect(self.libSelected)

		self.ui.lblInfoIcon.setPixmap(QtGui.QPixmap(ICON_PATH+"000.png"))
		self.ui.lblInfo.setText("")

	
		QtCore.QObject.connect(self.ui.btnGenerate, QtCore.SIGNAL('clicked()'), self.selected)

		widgets = self.findChildren(QtGui.QWidget);
		for w in widgets:
			w.installEventFilter(self)
			
		self.lastText = ""
		self.lastIcon = None
		self.infoVisible = False
		
		self.resize(500, 500)
		
		self.libs=None
		
		self.setWindowTitle("DisCODe Wizard: Choose Library")
		
		self.fun_cb = None
		
			
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

	def setLibraries(self, libraries):
		self.libs=libraries
		self.reloadLibs()
		
	def reloadLibs(self):
		self.ui.lstLibs.clear()
		if self.libs == None:
			return
		for lib in self.libs:
			self.ui.lstLibs.addItem(lib.name)
	
	def getLib(self, name):
		if self.libs == None:
			return None
			
		for lib in self.libs:
			if lib.name == name:
				return lib
				
		return None
	
	def libSelected(self, _, __):
		n = ""
		try:
			n=self.ui.lstLibs.currentItem().text()
		finally:
			self.selected_lib = self.getLib(n)
			lib = self.selected_lib
			if lib == None:
				return
			self.ui.edName.setText(lib.name)
			self.ui.edDesc.setPlainText(lib.desc)
			self.ui.edPckg.setText(lib.pckg)
			self.ui.edLibs.setText(lib.libs)
			self.ui.edAdds.setPlainText(lib.adds)
			self.ui.edIncl.setPlainText(lib.incl)

	def getSelectedLib(self):
		return self.selected_lib
		
	def setSelectedCb(self, fun):
		self.fun_cb = fun
		
	def selected(self):
		self.fun_cb(self.selected_lib)
		self.close()
	
