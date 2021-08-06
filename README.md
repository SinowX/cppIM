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

<img src="../imgs/2021-08-06_16-46.png" alt="2021-08-06_16-46" style="zoom:75%;" />

<img src="../imgs/2021-08-06_16-45_1.png" alt="2021-08-06_16-45_1" style="zoom:75%;" />



2. 登录

![2021-08-06_16-47](../imgs/2021-08-06_16-47.png)

3. 选择聊天对象

![2021-08-06_16-47_1](../imgs/2021-08-06_16-47_1.png)

4. 进行聊天对话

![2021-08-06_16-48](../imgs/2021-08-06_16-48.png)

![2021-08-06_16-48_1](../imgs/2021-08-06_16-48_1.png)

5. 查询历史记录

![2021-08-06_16-48_2](../imgs/2021-08-06_16-48_2.png)

![2021-08-06_16-49](../imgs/2021-08-06_16-49.png)



## 日志文件

- 为了便于调试，对代码进行了较多的输出信息，运行时默认输出到 stdout，可自行重定向
- 演示输出 log 文件
  - log_backend 后端日志
  - log_zhaoqifan 用户zhaoqifan前端日志
  - log_laopo 用户laopo前端日志







## 参考代码

https://github.com/qinshixiao/IM.git