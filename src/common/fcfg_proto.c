
#include <errno.h>
#include "fastcommon/shared_func.h"
#include "fastcommon/connection_pool.h"
#include "fastcommon/ini_file_reader.h"
#include "fcfg_proto.h"
#include "fcfg_types.h"
#include "fastcommon/sockopt.h"

void fcfg_proto_init()
{
}

int fcfg_proto_set_body_length(struct fast_task_info *task)
{
    task->length = buff2int(((FCFGProtoHeader *)task->data)->body_len);
    return 0;
}

int fcfg_proto_deal_actvie_test(struct fast_task_info *task,
        const FCFGRequestInfo *request, FCFGResponseInfo *response)
{
    return FCFG_PROTO_EXPECT_BODY_LEN(task, request, response, 0);
}

void fcfg_proto_response_extract (FCFGProtoHeader *header_pro,
        FCFGResponseInfo *resp_info)
{
    resp_info->cmd      = header_pro->cmd;
    resp_info->body_len = buff2int(header_pro->body_len);
    resp_info->status   = header_pro->status;
}

void fcfg_set_admin_header (FCFGProtoHeader *fcfg_header_proto,
        unsigned char cmd, int body_len)
{
    fcfg_header_proto->cmd = cmd;
    int2buff(body_len, fcfg_header_proto->body_len);
}

void fcfg_free_config_array(FCFGConfigArray *array)
{
    FCFGConfigEntry *current;
    FCFGConfigEntry *end;

    if (array->rows == NULL) {
        return;
    }

    end = array->rows + array->count;
    for (current=array->rows; current<end; current++) {
        if (current->name.str != NULL) {
            free(current->name.str);
        }
    }

    free(array->rows);
    array->rows = NULL;
    array->count = 0;
}

void fcfg_free_env_array(FCFGEnvArray *array)
{
    FCFGEnvEntry *current;
    FCFGEnvEntry *end;

    if (array->rows == NULL) {
        return;
    }

    end = array->rows + array->count;
    for (current=array->rows; current<end; current++) {
        if (current->env.str != NULL) {
            free(current->env.str);
        }
    }

    free(array->rows);
    array->rows = NULL;
    array->count = 0;
}

void _get_conn_config (ConnectionInfo *conn, const char *config_server)
{
    char *buff[2];
    char tmp_value[32];

    strncpy(tmp_value, config_server, sizeof(tmp_value) - 1);
    tmp_value[sizeof(tmp_value) - 1] = '\0';
    splitEx(tmp_value, ':', buff, 2);
    strncpy(conn->ip_addr, buff[0], sizeof(conn->ip_addr));
    conn->port = atoi(buff[1]);
    conn->sock = -1;
}

int send_and_recv_response_header(ConnectionInfo *conn, char *data, int len,
        FCFGResponseInfo *resp_info, int network_timeout, int connect_timeout)
{
    int ret;
    FCFGProtoHeader fcfg_header_resp_pro;

    if ((ret = tcpsenddata_nb(conn->sock, data,
            len, network_timeout)) != 0) {
        return ret;
    }
    if ((ret = tcprecvdata_nb_ex(conn->sock, &fcfg_header_resp_pro,
            sizeof(FCFGProtoHeader), network_timeout, NULL)) != 0) {
        return ret;
    }
    fcfg_proto_response_extract(&fcfg_header_resp_pro, resp_info);
    return 0;
}

