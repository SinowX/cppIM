#ifndef DIVERTER_H
#define DIVERTER_H

#include<arpa/inet.h>
#include<errno.h>
#include<netinet/in.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>

#include<stdio.h>
#include<string.h>

#include<string>

#include"database.h"
#include"define.h"
// #include"limits.h"
// #include"request.h"
// #include"reply.h"
#include"user.h"
#include"utils.h"
#include"worker.h"


class Diverter
{
public:
    Diverter();
    ~Diverter();
private:

    void run_once();
    void initListenFd();
    void initDB();
    userList *u_list;
    int listen_fd;
    DBMYSQL * m_db;
};





#endif