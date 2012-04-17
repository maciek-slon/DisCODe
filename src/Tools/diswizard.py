#!/usr/bin/python

# Import PySide classes
import sys
from PySide.QtCore import *
from PySide.QtGui import *



class MyWidget(QWidget):
    def __init__(self, parent):
        super(QWidget, self).__init__(parent)
        loader = QUiLoader()
        file = QFile("diswizard.ui")
        file.open(QFile.ReadOnly)
        myWidget = loader.load(file, self)
        file.close()

        layout = QVBoxLayout()
        layout.addWidget(myWidget)
        self.setLayout(layout)


# Create a Qt application
app = QApplication(sys.argv)
# Create a Label and show it
#widget=MyWidget(null)
#widget.show()
# Enter Qt application main loop
app.exec_()
sys.exit()
