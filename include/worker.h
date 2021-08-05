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
        const char *username;
        int client_fd;
        userList *u_list;
        DBMYSQL * m_db;
        pthread_t thread;
        static void *run(void* arg);
};



#endif