#ifndef REQUEST_H
#define REQUEST_H

#include<stdint.h>

#include"limits.h"

enum rqs_type{
    LOGIN, 
    P2PMSG,
    P2GMSG,
    SYNC,
    UNKNOWN
};



// enum pkg_type{
//     SIGIN,
//     LOGIN, 
//     P2PMSG,
//     P2GMSG,
//     SYNC // 同步请求
// };

// // 每发一个请求，服务端都会受到一个 pkg_header 以确定请求内容类型和大小
// // 根据 pkg_size 来 read 得到字符串后进行处理
// typedef struct pkg_header
// {
//     // pkg_type 内取值
//     uint8_t pkg_type;
//     uint32_t pkg_size;
// }pkg_header;


// typedef struct pkg_sigin
// {
//     char username[MAX_USERNAME_LENGTH];
//     char password[MAX_PWD_LENGTH];
// }pkg_sigin;


// typedef struct pkg_login
// {
//     char username[MAX_USERNAME_LENGTH];
//     char password[MAX_PWD_LENGTH];
// }pkg_login;

// typedef struct pkg_p2pmsg
// {
//     char sender[MAX_ID_LENGTH];
//     char receiver[MAX_ID_LENGTH];
//     char content[MAX_MESSAGE_LENGTH];
// }pkg_p2pmsg;

// typedef struct pkg_p2gmsg
// {
//     char sender[MAX_ID_LENGTH];
//     char group[MAX_ID_LENGTH];
//     char content[MAX_MESSAGE_LENGTH];
// }pkg_p2gmsg;

// typedef struct pkg_sync
// {
//     // 涉及的对象时群组还是个人
//     bool isGroup;
//     char relateObjName[MAX_ID_LENGTH];
// }pkg_sync;



#endif