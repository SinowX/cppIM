#ifndef WORKER_H
#define WORKER_H

#include<pthread.h>
#include<unistd.h>
#include<stdio.h>

#include"database.h"
// #include"limits.h"
// #include"reply.h"
// #include"request.h"
#include"user.h"
#include"utils.h"
// #include"msg.h"

class Worker
{
    public:
        Worker(const char* username,DBMYSQL *m_db, int client_fd,userList *u_list);
        void start();
    private:
        static const char *username;
        static int client_fd;
        static userList *u_list;
        pthread_t thread;
        static DBMYSQL * m_db;
        static void *run(void* arg);
};



#endif