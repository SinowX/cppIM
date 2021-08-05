#ifndef USER_H
#define USER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "define.h"
// #include "worker.h"

typedef struct loginInfo
{
    char id[MAX_ID_LENGTH];
    char pwd[MAX_PWD_LENGTH];
}loginInfo;

typedef struct userInfo
{
    // char id[MAX_ID_LENGTH];
    char name[MAX_USERNAME_LENGTH];
    int client_fd;
    // Worker *thread;
    struct userInfo* next;
}userInfo;

class userList{
    public:
        userList();
        void addUser(const char *name,int client_fd);
        void dropUser(const char *name);
        userInfo *getUser(const char *name);
        // int getLength();
    private:
        userInfo *head;
};

#endif