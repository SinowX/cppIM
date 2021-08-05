#ifndef REPLY_H
#define REPLY_H

#include<stdint.h>

#include"limits.h"

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

// typedef struct rpl_header
// {
//     bool valid;
//     uint8_t rpl_type;
//     uint32_t rpl_num;
// }rpl_header;

// typedef struct rpl_sigin
// {
//     uint8_t status;
// };

// typedef struct rpl_login
// {
//     uint8_t status;
// }rpl_login;

// typedef struct rpl_p2pmsg
// {
//     uint8_t status;
// }rpl_p2pmsg;


// typedef struct rpl_p2gmsg
// {
//     uint8_t status;
// }rpl_p2gmsg;


// typedef struct rpl_sync
// {
//     uint8_t status;
//     uint32_t msg_num;
//     //其后跟 size 个 msg struct
// }rpl_sync;


#endif