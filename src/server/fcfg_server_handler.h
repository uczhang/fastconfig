//fcfg_server_handler.h

#ifndef FCFG_SERVER_HANDLER_H
#define FCFG_SERVER_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fastcommon/fast_task_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

int fcfg_server_handler_init();
int fcfg_server_handler_destroy();
int fcfg_server_deal_task(struct fast_task_info *task);
void fcfg_server_task_finish_cleanup(struct fast_task_info *task);
void *fcfg_server_alloc_thread_extra_data(const int thread_index);

#ifdef __cplusplus
}
#endif

#endif
