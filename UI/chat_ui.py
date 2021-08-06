import sys
from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout, \
    QLabel, QLineEdit, QPushButton, QTextBrowser, QPlainTextEdit
from PyQt5 import QtGui
from PyQt5.QtCore import Qt






class Chat_UI(QWidget):
    history_dialog: QTextBrowser
    editor_dialog:QPlainTextEdit
    btn_send: QPushButton
    name_info: QLabel
    status_info: QLabel
    info: QVBoxLayout
    switch: QHBoxLayout
    line_switch: QLineEdit
    btn_switch: QPushButton
    btn_history: QPushButton
    name_myinfo = QLabel
    status_myinfo = QLabel
    btn_exit: QPushButton
    myinfo: QVBoxLayout
    def __init__(self):
        super().__init__()
        self.history_dialog = QTextBrowser()
        self.history_dialog.setFont(QtGui.QFont(None,12))
        self.history_dialog.setPlainText('')
        self.editor_dialog = QPlainTextEdit()
        self.editor_dialog.setFont(QtGui.QFont(None,12))
        self.editor_dialog.setPlainText('')
        self.btn_send = QPushButton()
        self.btn_send.setText('Send')
        self.btn_send.setFont(QtGui.QFont(None, 14))

        self.name_info = QLabel()
        self.name_info.setText('-')
        self.name_info.setFont(QtGui.QFont(None, 16))
        self.name_info.setAlignment(Qt.AlignHCenter|Qt.AlignVCenter)
        self.status_info = QLabel()
        self.status_info.setText('Online')
        self.status_info.setFont(QtGui.QFont(None,14))
        self.info = QVBoxLayout()
        self.info.addWidget(self.name_info)
        self.info.addWidget(self.status_info)

        self.switch = QHBoxLayout()
        self.line_switch = QLineEdit()
        self.line_switch.setFont(QtGui.QFont(None,16))
        self.btn_switch = QPushButton()
        self.btn_switch.setText('Switch')
        self.btn_switch.setFont(QtGui.QFont(None,14))
        self.switch.addWidget(self.line_switch)
        self.switch.addWidget(self.btn_switch)

        self.btn_history = QPushButton()
        self.btn_history.setText('History')
        self.btn_history.setFont(QtGui.QFont(None, 14))


        self.name_myinfo = QLabel()
        self.name_myinfo.setText('-')
        self.name_myinfo.setAlignment(Qt.AlignCenter)
        self.name_myinfo.setFont(QtGui.QFont(None,16))

        self.status_myinfo = QLabel()
        self.status_myinfo.setText('Connected')
        self.status_myinfo.setFont(QtGui.QFont(None,14))

        self.btn_exit = QPushButton()
        self.btn_exit.setText('Exit')
        self.btn_exit.setFont(QtGui.QFont(None, 14))


        self.myinfo = QVBoxLayout()
        self.myinfo.addWidget(self.name_myinfo)
        self.myinfo.addWidget(self.status_myinfo)
        self.myinfo.addWidget(self.btn_exit)

        self.initUI()
        pass

    def initUI(self):


        btm_dialog = QHBoxLayout()
        btm_dialog.addStretch(1)
        btm_dialog.addWidget(self.btn_send)
        btm_dialog.setStretch(0,4)
        btm_dialog.setStretch(1,2)

        dialogBox = QVBoxLayout()
        dialogBox.addWidget(self.history_dialog)
        dialogBox.addWidget(self.editor_dialog)
        dialogBox.addLayout(btm_dialog)

        dialogBox.setStretch(0, 2)
        dialogBox.setStretch(1, 1)

        control = QVBoxLayout()
        control.addLayout(self.switch)
        control.addWidget(self.btn_history)

        panelBox = QVBoxLayout()
        panelBox.addLayout(self.info)
        panelBox.addLayout(control)
        # panelBox.addWidget(self.history)
        panelBox.addLayout(self.myinfo)

        # panelBox.setStretch(0, 1)
        # panelBox.setStretch(1, 1)
        # panelBox.setStretch(2, 1)
        # panelBox.setStretch(3, 1)
        # panelBox.addStretch(1)
        # panelBox.setSpacing(50)


        hBox = QHBoxLayout()
        hBox.addLayout(dialogBox)
        hBox.addLayout(panelBox)
        hBox.setStretch(0, 2)
        hBox.setStretch(1, 1)



        self.setLayout(hBox)
        self.setGeometry(300, 300, 1080, 800)
        self.setWindowTitle('Absolute')
        self.show()
        pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    exam = Chat_UI()
    sys.exit(app.exec_())

