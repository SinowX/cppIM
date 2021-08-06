import sys
import time

from PyQt5.QtWidgets import QApplication, QWidget, QHBoxLayout, QVBoxLayout, \
    QLabel, QLineEdit, QPushButton






class Login_UI(QWidget):
    def __init__(self):
        super().__init__()

        self.name_line = QLineEdit(self)
        self.pwd_line = QLineEdit(self)
        self.login_btn = QPushButton('Login', self)
        # self.login_btn.clicked.connect(self.change)
        self.initUI()
        pass

    def initUI(self):

        name_label = QLabel(self)
        name_label.setText('UserName:')
        name_hBox = QHBoxLayout()
        name_hBox.addWidget(name_label)
        name_hBox.addWidget(self.name_line)

        pwd_label = QLabel(self)
        pwd_label.setText(' Password:')
        pwd_hBox = QHBoxLayout()
        pwd_hBox.addWidget(pwd_label)
        pwd_hBox.addWidget(self.pwd_line)






        vbox = QVBoxLayout()
        # vbox.addStretch(1)
        vbox.addLayout(name_hBox)
        vbox.addLayout(pwd_hBox)
        vbox.addWidget(self.login_btn)

        self.setLayout(vbox)
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('Absolute')
        # self.show()
        pass


    def change(self):
        for i in range(10):
            time.sleep(1)
            self.name_line.setText(str(i))
        pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    exam = Login_UI()

    app.exec_()
    # sys.exit(app.exec_())

