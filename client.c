#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libwebsockets.h>

static struct lws *global_wsi = NULL;
static int connection_established = 0;

static int callback_websocket(struct lws *wsi, enum lws_callback_reasons reason,
                              void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_CLIENT_ESTABLISHED:
            printf("WebSocket connection established\n");
            global_wsi = wsi;
            connection_established = 1; // 标记连接已建立
            break;
        case LWS_CALLBACK_CLIENT_RECEIVE:
            printf("Received: %s\n", (char *)in);
            break;
        case LWS_CALLBACK_CLIENT_WRITEABLE:
            // 不在这里处理写操作，写操作在主循环中处理
            break;
        case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            printf("WebSocket connection error\n");
            global_wsi = NULL;
            connection_established = -1; // 标记连接出错
            break;
        case LWS_CALLBACK_CLOSED:
            printf("WebSocket connection closed\n");
            global_wsi = NULL;
            connection_established = -1; // 标记连接关闭
            break;
        default:
            break;
    }
    return 0;
}

static struct lws_protocols protocols[] = {
    {
        "example-protocol",
        callback_websocket,
        0,
        128,
    },
    { NULL, NULL, 0, 0 } // 终止符
};

int main(void) {
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof(info));
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = protocols;

    struct lws_context *context = lws_create_context(&info);
    if (context == NULL) {
        fprintf(stderr, "lws_create_context failed\n");
        return -1;
    }

    struct lws_client_connect_info ccinfo = {0};
    ccinfo.context = context;
    ccinfo.address = "192.168.1.244"; // 确保这是正确的服务器 IP 地址
    ccinfo.port = 8000;               // 确保这是正确的端口
    ccinfo.path = "/";
    ccinfo.host = lws_canonical_hostname(context);
    ccinfo.origin = "origin";
    ccinfo.protocol = protocols[0].name;
    ccinfo.ietf_version_or_minus_one = -1;

    struct lws *wsi = lws_client_connect_via_info(&ccinfo);
    if (wsi == NULL) {
        fprintf(stderr, "lws_client_connect_via_info failed\n");
        lws_context_destroy(context);
        return -1;
    }

    char buffer[128];

    // 等待 WebSocket 连接建立
    while (!connection_established) {
        lws_service(context, 1000);
    }

    // 如果连接出错，退出
    if (connection_established == -1) {
        lws_context_destroy(context);
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        size_t n = strlen(buffer);
        if (n > 0 && buffer[n - 1] == '\n') {
            buffer[n - 1] = '\0';
            n--;
        }
		
		if (global_wsi != NULL) {
            // 構建 JSON 格式字串
            char json_msg[256]; // 假設最大長度為 256
            snprintf(json_msg, sizeof(json_msg), "{\"radar_value\": \"%s\"}", buffer);

            unsigned char msg[LWS_SEND_BUFFER_PRE_PADDING + 256 + LWS_SEND_BUFFER_POST_PADDING];
            size_t json_len = strlen(json_msg);
            memcpy(&msg[LWS_SEND_BUFFER_PRE_PADDING], json_msg, json_len);
            lws_write(global_wsi, &msg[LWS_SEND_BUFFER_PRE_PADDING], json_len, LWS_WRITE_TEXT);
            lws_callback_on_writable(global_wsi); // 觸發寫事件
        }

        lws_service(context, 1000);
    }

    lws_context_destroy(context);
    return 0;
}
