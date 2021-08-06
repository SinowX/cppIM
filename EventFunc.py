from PyQt5.QtCore import *
from PyQt5.Qt import *
from PyQt5.QtGui import *
import sys
import cgitb
sys.excepthook = cgitb.Hook(1, None, 5, sys.stderr, 'text')
class mythread(QThread):  # 步骤1.创建一个线程实例
    mysignal = pyqtSignal(tuple)  # 创建一个自定义信号，元组参数

    def __init__(self):
        super(mythread, self).__init__()


    def run(self):
        a = (1, 2)
        self.mysignal.emit(a)  # 发射自定义信号

class Window(QWidget):
    def __init__(self):
        super(Window, self).__init__()
        self.setWindowTitle('信号传输')
        self.resize(500,500)
        self.move(50,50)
        self.setup_ui()

        self.my_thread = mythread()  # 步骤2. 主线程连接子线
        self.my_thread.mysignal.connect(self.zhi)  # 自定义信号连接
        self.my_thread.start()   #  步骤3 子线程开始执行run函数

    def setup_ui(self):
        self.line1 = QLineEdit(self)
        self.line1.move(0,0)
        self.line1.resize(50,50)

        self.line2 = QLineEdit(self)
        self.line2.move(50, 50)
        self.line2.resize(50, 50)


    def zhi(self, zhi):
        a, b = zhi
        self.line1.setText(str(a))
        self.line2.setText(str(b))
if __name__ == '__main__':

    app = QApplication(sys.argv)

    window = Window()
    window.show()
    sys.exit(app.exec_())
QThread