# IM 说明文档

## 开发环境

- 系统：Linux Arch 5.13.7-arch1-1 #1 SMP PREEMPT Sat, 31 Jul 2021 13:18:52 +0000 x86_64 GNU/Linux
- 后端开发语言：C++
  - 外部共享库：
    - mysqlclient-dev, jsoncpp, pthread
- 后端开发工具：
  - visual-studio-code-bin 1.58.2-1
  - g++ (GCC) 11.1.0
  - cmake version 3.21.1
- 前端开发语言：Python
  - 库：
    - PyQt5, ctypes, socket, json, sys, time
- 前端开发工具：
  - pycharm-community-edition 2021.1.3-1



## 后端逻辑说明

### 类 Class

- Diverter

  - ```cc
    class Diverter
    {
    public:
        Diverter();
        ~Diverter();
    private:
        void run_once();	// accept() 尝试登陆的客户端
        void initListenFd();	// 初始化服务器监听套接字
        void initDB();	// 连接 TDSQL 数据库
        userList *u_list;	// 维护在线成员列表
        int listen_fd;	// 监听套接字fd
        DBMYSQL * m_db;	// 数据库句柄
    };
    ```

- Worker

  - ```cc
    class Worker
    {
        public:
            Worker(const char* username,DBMYSQL *m_db, int client_fd,userList *u_list);	// 连接TDSQL
            void start();	// 创建子线程
        private:
            const char *username;	// 该线程服务的用户名
            int client_fd;	// 用户套接字
            userList *u_list;	// 用户列表指针，便于在线程中查询用户在线状态
            DBMYSQL * m_db;	// 数据库句柄
            pthread_t thread; // 线程id 
            static void *run(void* arg);	// 线程运行函数
    };
    ```

- userList

  - ```cc
    class userList{
        public:
            userList();
            void addUser(const char *name,int client_fd);	// 上线用户
            void dropUser(const char *name);	// 下线用户
            userInfo *getUser(const char *name);	// 查询用户
        private:
            userInfo *head;	// 头指针
    };
    ```

- DBMYSQL

  - ```cc
    class DBMYSQL
    {
    public:
        DBMYSQL(const char*host,int port ,const char* user,const char* passwd,const char* dbname);
        ~DBMYSQL();
        //if failed return -1 else return 0
        int insert(const char* sql); // 增
        //if none return nullptr !!!!
        MYSQL_RES* select(const char* sql);	// 查
    private:
        MYSQL handle;
    };
    ```



### 流程

- main 函数调用 Diverter()，Dirverter 循环调用 Diverter->run_once() 来与客户端建立 TCP 套接字 连接，验证密码通过则调用 Worker() 建立新线程处理客户请求，Worker 可处理 对某用户发信息、查询历史消息 功能



```python
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
        super(SendThread, self).__init__()
        self.msg = msg
        self.ui = ui
        self.TcpKeeper = TcpKeeper
        pass
    def run(self):
        send_msg = (json.dumps(self.msg)+"\0").encode('utf-8')
        send_msg_size = c_uint32(len(send_msg))

        self.TcpKeeper.sendMsg(send_msg_size)
        self.TcpKeeper.sendMsg(send_msg)

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
        super(LoginThread, self).__init__()
        self.name = name
        self.pwd = pwd
        self.TcpKeeper = TcpKeeper
        pass
    def run(self):
        info = {'username': self.name, 'password': self.pwd}
        login_info = (json.dumps(info)+"\0").encode('utf-8')
        login_info_size = c_uint32(len(login_info))
        self.TcpKeeper.sendMsg(login_info_size)
        self.TcpKeeper.sendMsg(login_info)
        buffersize:bytes = self.TcpKeeper.recvMsg(cuint_32size)
        buffer: bytes = self.TcpKeeper.recvMsg(int.from_bytes(buffersize,'little'))
        res = buffer.decode('utf-8')
        res2: dict = json.loads(res)
        global isLogin
        if (res2['type']==LOGIN and res2['status']==SUCCESS):
            isLogin = True
            pass
        else:
            isLogin = False
            pass
        global login_condi
        login_condi.wakeAll()
        pass


class ReceiveThread(QThread):
    receiveSignal = pyqtSignal(str)
    def __init__(self, TcpKeeper:Connector, ui: Chat_UI):
        super(ReceiveThread, self).__init__()
        self.TcpKeeper = TcpKeeper
        self.ui = ui
        pass
    def run(self):
        m_theother: str =''
        while True:
            try:
                update_mutex.tryLock()
            except IOError:
                pass

            buffersize: bytes = self.TcpKeeper.recvMsg(cuint_32size)
            buffer: bytes = self.TcpKeeper.recvMsg(int.from_bytes(buffersize, 'little'))
            res: dict = json.loads(buffer.decode('utf-8'))
            # time.sleep(3)
            # res:dict = {'content':"asdnauid",'receiver':"laopo",'sender':'zhaoqifan','status':0,'timestamp':'2021-08-06 15:24:41','type':3}
            if res['type'] == P2PMSG:
                if res['receiver'] == username:
                    m_theother = res['sender']
                else:
                    continue
            elif res['type'] == SYNC:
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
            history[m_theother]= history[m_theother] + "\n" + newhistory
            if m_theother == theother:
                self.receiveSignal.emit(newhistory)
                # self.ui.history_dialog.append(newhistory)
                # self.ui.history_dialog.repaint()
                update_condi.wait(update_mutex)
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
        pass

    def run(self):
        self.LUI.show()
        pass

    def LoginRqs(self, name:str, pwd:str):
        global login_mutex
        try:
            login_mutex.tryLock()
        except IOError:
            return

        self.TcpKeeper.connectServer()
        login_thread = LoginThread(name,pwd, self.TcpKeeper)
        login_thread.start()

        login_condi.wait(login_mutex)
        if(isLogin):
            self.LUI.login_btn.disconnect()
            self.LUI.hide()
            global username
            username = name
            self.CUI.name_myinfo.setText(username)
            self.CUI.show()
            self.receiveKeeper.start()

        else:
            self.TcpKeeper.disconnect()
        login_mutex.unlock()
        pass

    def UpdateHisDialog(self,newhistory:str):

        global update_condi
        # try:
        #     update_mutex.tryLock()
        # except IOError:
        #     pass
        # self.CUI.history_dialog.setPlainText(newhistory)
        self.CUI.history_dialog.append(newhistory+"\n")
        update_condi.wakeAll()
        pass

    def SendMsg(self):
        if theother=='':
            return
        global send_mutex
        try:
            send_mutex.tryLock()
        except IOError:
            return
        # global theother
        # global username
        msg = {'type':P2PMSG,'sender':username,'receiver':theother,'content':self.CUI.editor_dialog.toPlainText()}
        send_thread = SendThread(msg, self.CUI, self.TcpKeeper)
        send_thread.sendSignal.connect(self.UpdateHisDialog)
        send_thread.start()

        send_condi.wait(send_mutex)


        send_mutex.unlock()
        pass

    def GetHistory(self):
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
```









## 数据库

```sql
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user`(
    `index` int not null AUTO_INCREMENT,
    `id` varchar(64) not null,
    `nickname` varchar(64) not null,
    `createtime` timestamp DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`index`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

DROP TABLE IF EXISTS `message_log`;
CREATE TABLE `message_log`(
    `index` int not null AUTO_INCREMENT,
    `sender_id` varchar(64) not null,
    `receiver_id` varchar(64) not null,
    `content` varchar(255) not null,
    `timestamp` timestamp DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`index`)
)ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```





## 前段逻辑说明

### 普通类

- Connector 封装与后端的 TCP 的交互

### UI 类

- Login_UI 绘制 Login 登录界面
- Chat_UI 绘制 Chat 聊天界面

### 线程类

- MainThread(QThread) 调用 UI 类，处理信号
- LoginThread(QThread) 尝试登录

- SendThread(QThread) 发送文本
- HistoryThread(QThread) 发送查询历史请求
- ReceiveThread(QThread) 接收服务器发送的信息，分流处理（常驻线程）



## 使用方法

1. 测试账户

<img src="imgs/2021-08-06_16-46.png" alt="2021-08-06_16-46" style="zoom:75%;" />

<img src="imgs/2021-08-06_16-45_1.png" alt="2021-08-06_16-45_1" style="zoom:75%;" />



2. 登录

![2021-08-06_16-47](imgs/2021-08-06_16-47.png)

3. 选择聊天对象

![2021-08-06_16-47_1](imgs/2021-08-06_16-47_1.png)

4. 进行聊天对话

![2021-08-06_16-48](imgs/2021-08-06_16-48.png)

![2021-08-06_16-48_1](imgs/2021-08-06_16-48_1.png)

5. 查询历史记录

![2021-08-06_16-48_2](imgs/2021-08-06_16-48_2.png)

![2021-08-06_16-49](imgs/2021-08-06_16-49.png)



## 日志文件

- 为了便于调试，对代码进行了较多的输出信息，运行时默认输出到 stdout，可自行重定向
- 演示输出 log 文件
  - log_backend 后端日志
  - log_zhaoqifan 用户zhaoqifan前端日志
  - log_laopo 用户laopo前端日志







## 参考代码

https://github.com/qinshixiao/IM.git