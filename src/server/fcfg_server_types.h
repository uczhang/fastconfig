#ifndef _FCFG_SERVER_TYPES_H
#define _FCFG_SERVER_TYPES_H

#include "fastcommon/common_define.h"

typedef struct fcfg_server_context {
    char data_path[MAX_PATH_SIZE];
    char error_info[FCFG_ERROR_INFO_SIZE];
} FCFGServerContext;

#endif