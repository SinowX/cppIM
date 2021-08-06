import json
import sys
import time

from PyQt5.QtCore import QThread, QMutex, QWaitCondition, pyqtSignal
from ctypes import c_uint32
from UI.login_ui import Login_UI
from UI.chat_ui import Chat_UI
from Definition import *
from Connector import Connector

login_mutex = QMutex()
login_condi = QWaitCondition()
isLogin = False

send_mutex = QMutex()
send_condi = QWaitCondition()
history_mutex = QMutex()
history_condi = QWaitCondition()
switch_mutex = QMutex()
switch_condi = QWaitCondition()
exit_mutex = QMutex()
exit_condi = QWaitCondition()

update_mutex = QMutex()
update_condi = QWaitCondition()


username:str = ''
theother:str = ''
history:dict ={}


# chatui = Chat_UI()

# class EventProc:
#     def __init__(self,TcpKeeper:Connector):
#         self.TcpKeeper = TcpKeeper
#
#         pass
#
#
#         pass


class SendThread(QThread):
    sendSignal = pyqtSignal(str)
    def __init__(self, msg:dict, ui: Chat_UI, TcpKeeper: Connector):
        print("SendThread init")
        super(SendThread, self).__init__()
        self.msg = msg
        self.ui = ui
        self.TcpKeeper = TcpKeeper
        print("SendThread init over")
        pass
    def run(self):
        send_msg = (json.dumps(self.msg)+"\0").encode('utf-8')
        print("SendThread send_msg = ")
        print(send_msg)
        send_msg_size = c_uint32(len(send_msg))

        self.TcpKeeper.sendMsg(send_msg_size)
        self.TcpKeeper.sendMsg(send_msg)
        print("SendThread Msg has Sent")

        if theother not in history:
            history[theother] = ''
        # history[theother] = history[theother] + json.dumps(msg) + "\0"

        newhistory:str = self.msg['sender']+" "+time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())+"\n"+"  "+self.msg['content']
        history[theother] = history[theother] + "\n" +newhistory
        self.sendSignal.emit(newhistory)
        # self.ui.history_dialog.append(newhistory)
        # global chatui
        # chatui.history_dialog.setPlainText(newhistory)
        # self.ui.history_dialog.setPlainText(newhistory)
        # self.ui.history_dialog.
        # self.ui.history_dialog.repaint()

        global send_condi
        send_condi.wakeAll()
        pass


class HistoryThread(QThread):
    def __init__(self,msg:dict, ui: Chat_UI , TcpKeeper: Connector):
        super(HistoryThread, self).__init__()
        self.msg = msg
        self.ui = ui
        self.TcpKeeper = TcpKeeper
        pass
    def run(self):
        send_msg = (json.dumps(self.msg) + "\0").encode('utf-8')
        send_msg_size = c_uint32(len(send_msg))
        self.TcpKeeper.sendMsg(send_msg_size)
        self.TcpKeeper.sendMsg(send_msg)
        history_condi.wakeAll()
        pass



class LoginThread(QThread):
    def __init__(self, name: str, pwd: str, TcpKeeper:Connector):
        print("Login Thread init")
        super(LoginThread, self).__init__()
        self.name = name
        self.pwd = pwd
        self.TcpKeeper = TcpKeeper
        print("Login Thread init over")
        pass
    def run(self):
        print("Login Thread running")
        info = {'username': self.name, 'password': self.pwd}
        login_info = (json.dumps(info)+"\0").encode('utf-8')
        print("Login Thread login_info(bytes):")
        print(login_info)
        login_info_size = c_uint32(len(login_info))
        print("Login Thread login_info_size:", login_info_size)
        self.TcpKeeper.sendMsg(login_info_size)
        self.TcpKeeper.sendMsg(login_info)
        print("Login Thread Sent size and info")
        buffersize:bytes = self.TcpKeeper.recvMsg(cuint_32size)
        print("Login Thread Get buffersize", buffersize)
        # print(int.from_bytes(buffersize,'little'))
        buffer: bytes = self.TcpKeeper.recvMsg(int.from_bytes(buffersize,'little'))
        print("Login Thread recv buffersize :", int.from_bytes(buffersize,'little'))
        print(buffer)
        res = buffer.decode('utf-8')
        print("Login Thread get res:")
        print(res)
        res2: dict = json.loads(res)
        print("Login Thread get res2: ")
        print(res2)
        global isLogin
        if (res2['type']==LOGIN and res2['status']==SUCCESS):
            print("Login Thread Success")
            isLogin = True
            pass
        else:
            print("Login Thread Failed")
            isLogin = False
            pass
        global login_condi
        login_condi.wakeAll()
        print("Login Thread Wake login_condi")
        pass


class ReceiveThread(QThread):
    receiveSignal = pyqtSignal(str)
    def __init__(self, TcpKeeper:Connector, ui: Chat_UI):
        super(ReceiveThread, self).__init__()
        print("Receiver Thread init")
        self.TcpKeeper = TcpKeeper
        self.ui = ui
        print("Receiver Thread init over")
        pass
    def run(self):
        print("ReceiveThread start")
        m_theother: str =''
        while True:
            try:
                update_mutex.tryLock()
            except IOError:
                pass

            buffersize: bytes = self.TcpKeeper.recvMsg(cuint_32size)
            print("ReceiveThread buffersize = ", int.from_bytes(buffersize, 'little'))
            buffer: bytes = self.TcpKeeper.recvMsg(int.from_bytes(buffersize, 'little'))
            res: dict = json.loads(buffer.decode('utf-8'))
            # time.sleep(3)
            # res:dict = {'content':"asdnauid",'receiver':"laopo",'sender':'zhaoqifan','status':0,'timestamp':'2021-08-06 15:24:41','type':3}
            print("ReceiveThread get buffer res:")
            print(res)
            if res['type'] == P2PMSG:
                print("ReceiveThread get a P2PMSG")
                print("ReceiveThread P2PMSG from ",res['sender'])
                if res['receiver'] == username:
                    m_theother = res['sender']
                else:
                    continue
            elif res['type'] == SYNC:
                print("ReceiveThread get a SYNC")
                print("ReceiveThread SYNC sender = ", res['sender'])
                print("ReceiveThread SYNC receiver = ", res['receiver'])
                if res['sender'] == username:
                    m_theother = res['receiver']
                else:
                    m_theother = res['sender']

            if m_theother not in history:
                history[m_theother]=''
            if 'timestamp' in res:
                newhistory = res['sender'] + " " \
                             + res['timestamp'] \
                             + "\n" + "  " + res['content']
            else:
                newhistory = res['sender']+" "\
                            +time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())\
                            +"\n"+"  "+res['content']
            print("ReceiveThread get newhistory :")
            print(newhistory)
            history[m_theother]= history[m_theother] + "\n" + newhistory
            print("ReceiveThread Updated history")
            if m_theother == theother:
                self.receiveSignal.emit(newhistory)
                # self.ui.history_dialog.append(newhistory)
                # self.ui.history_dialog.repaint()
                update_condi.wait(update_mutex)
                print("ReceiveThread receiveSignal Emited")
            update_mutex.unlock()
        pass

class MainThread(QThread):
    def __init__(self):
        super(MainThread, self).__init__()
        self.TcpKeeper = Connector()
        # self.EPUnit = EventProc(self.TcpKeeper)


        self.LUI = Login_UI()
        self.LUI.login_btn.clicked.connect(
            lambda:self.LoginRqs(
                self.LUI.name_line.text(),
                self.LUI.pwd_line.text()
            ))
        # global chatui
        self.CUI = Chat_UI()
        self.CUI.btn_send.clicked.connect(self.SendMsg)
        self.CUI.btn_switch.clicked.connect(self.SwitchPerson)
        self.CUI.btn_history.clicked.connect(self.GetHistory)
        self.CUI.btn_exit.clicked.connect(self.Exit)
        self.CUI.hide()
        self.receiveKeeper = ReceiveThread(self.TcpKeeper,self.CUI)
        self.receiveKeeper.receiveSignal.connect(self.UpdateHisDialog)
        print("MainThread init over")
        pass

    def run(self):
        self.LUI.show()
        print("MainThread running")
        pass

    def LoginRqs(self, name:str, pwd:str):
        print("Login Btn Triggered")
        global login_mutex
        try:
            print("trying lock login_mutex")
            login_mutex.tryLock()
        except IOError:
            return

        self.TcpKeeper.connectServer()
        print("Login tcp connectServer")
        login_thread = LoginThread(name,pwd, self.TcpKeeper)
        login_thread.start()

        login_condi.wait(login_mutex)
        print("Login condi awaked")
        if(isLogin):
            print("Login Success")
            self.LUI.login_btn.disconnect()
            print("Lui login btn disconnect()")
            self.LUI.hide()
            print("Lui hide()")
            global username
            username = name
            print("username = ",name)
            self.CUI.name_myinfo.setText(username)
            self.CUI.show()
            print("CUI show()")
            self.receiveKeeper.start()

        else:
            print("Login Failed")
            self.TcpKeeper.disconnect()
        login_mutex.unlock()
        print("login mutex unlock()")
        pass

    def UpdateHisDialog(self,newhistory:str):

        global update_condi
        # try:
        #     update_mutex.tryLock()
        #     print("UpdateHisDialog trylock()")
        # except IOError:
        #     pass
        print("UpdateHisDialog running")
        # self.CUI.history_dialog.setPlainText(newhistory)
        self.CUI.history_dialog.append(newhistory+"\n")
        update_condi.wakeAll()
        print("UpdateHisDialog wakeAll()")
        pass

    def SendMsg(self):
        print("MainThread Send Btn Triggered")
        if theother=='':
            return
        global send_mutex
        try:
            send_mutex.tryLock()
            print("MainTread Send mutex trylock()")
        except IOError:
            return
        # global theother
        # global username
        msg = {'type':P2PMSG,'sender':username,'receiver':theother,'content':self.CUI.editor_dialog.toPlainText()}
        print("MainTread SendMsg pack msg :")
        print(msg)
        send_thread = SendThread(msg, self.CUI, self.TcpKeeper)
        send_thread.sendSignal.connect(self.UpdateHisDialog)
        send_thread.start()

        send_condi.wait(send_mutex)


        send_mutex.unlock()
        pass

    def GetHistory(self):
        print("History Btn Triggered")
        global history_mutex
        try:
            history_mutex.tryLock()
        except IOError:
            pass


        history[theother]=''
        self.CUI.history_dialog.setPlainText('')
        msg = {'type':SYNC,'person1':username,'person2':theother}

        history_thread = HistoryThread(msg,self.CUI,self.TcpKeeper)
        history_thread.start()
        history_condi.wait(history_mutex)
        history_mutex.unlock()
        pass

    def SwitchPerson(self):
        global switch_mutex
        try:
            switch_mutex.tryLock()
        except IOError:
            return
        global theother
        theother = self.CUI.line_switch.text()
        self.CUI.name_info.setText(theother)
        global history
        if theother not in history:
            history[theother] = ''
        self.CUI.history_dialog.setText(history[theother])
        switch_mutex.unlock()
        pass

    def Exit(self):
        self.TcpKeeper.disconnect()
        exit(0)
        pass

    pass