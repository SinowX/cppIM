#ifndef MSG_H
#define MSG_H

#include "limits.h"
typedef struct Msg
{
    int sender;
    int receiver;
    char receiver_name[MAX_USERNAME_LENGTH];
    char message[MAX_MESSAGE_LENGTH];
}Msg;



#endif