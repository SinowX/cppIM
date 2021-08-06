from building import *
from PyQt5.QtWidgets import QApplication
import sys

# def main():
#     app = QApplication(sys.argv)
#     L_UI = Login_UI()
#     athread = LoginThread(L_UI)
#     athread.login_done.connect(zhi)
#     athread.start()
#
#     app.exec_()
#     pass




if __name__ == '__main__':
    app = QApplication(sys.argv)

    theThread = MainThread()
    theThread.start()

    app.exec_()
    pass