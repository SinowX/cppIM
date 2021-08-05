#ifndef DEFINE_H
#define DEFINE_H

#include<stdint.h>


const int MAX_SQL_LENGTH = 4096;
const int MAX_BUFFER_SIZE = 4096;
const int MAX_LISTEN_QUEUE=1024;
const int MAX_ID_LENGTH=16;
const int MAX_PWD_LENGTH=32;
const int MAX_MESSAGE_LENGTH=128;
const int MAX_USERNAME_LENGTH=10;


const int LISTEN_PORT = 5000;
const char LISTEN_ADDR[]="127.0.0.1";

enum rpl_status{
    SUCCESS,
    NAME_OCCUPIED,
    INSERT_FAILED,
    NOTSIGNED,
    PWDNOTMATCH,
    NOPRIVILEGE,
    BROKEN_PKG,
    MULTILOGIN
};

enum rqs_type{
    LOGIN, 
    P2PMSG,
    P2GMSG,
    SYNC,
    UNKNOWN
};


typedef struct Msg
{
    int sender;
    int receiver;
    char receiver_name[MAX_USERNAME_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
}Msg;


#endif