#ifndef _FCFG_TYPES_H
#define _FCFG_TYPES_H

#include "fastcommon/common_define.h"

#define FCFG_SERVER_DEFAULT_INNER_PORT  20000
#define FCFG_SERVER_DEFAULT_OUTER_PORT  20000

#define FCFG_ERROR_INFO_SIZE             256

typedef struct {
    unsigned char cmd;  //response command
    int body_len;       //response body length
} FCFGRequestInfo;

typedef struct {
    struct {
        char message[FCFG_ERROR_INFO_SIZE];
        int length;
    } error;
    int status;
    int body_len;    //response body length
    bool response_done;
    unsigned char cmd;   //response command
} FCFGResponseInfo;

#endif