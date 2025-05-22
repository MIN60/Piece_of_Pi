#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "web_server.h"

#define BUF_SIZE 1024

static void* handle_http_client(void* arg);
static void send_file(FILE* fp, const char* filename);
static void send_response(FILE* fp, const char* status, const char* content_type);
static void send_error(FILE* fp);

void start_web_server(int port) {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("[WEB] socket");
        exit(1);
    }

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("[WEB] bind");
        exit(1);
    }

    if (listen(server_sock, 10) == -1) {
        perror("[WEB] listen");
        exit(1);
    }

    printf("[WEB] 웹 서버가 포트 %d에서 실행 중입니다...\n", port);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t addrlen = sizeof(cli_addr);
        int* client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr*)&cli_addr, &addrlen);
        if (*client_sock < 0) {
            perror("[WEB] accept");
            free(client_sock);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_http_client, client_sock);
        pthread_detach(tid);
    }
}

static void* handle_http_client(void* arg) {
    int client_sock = *(int*)arg;
    free(arg);

    FILE* read_fp = fdopen(client_sock, "r");
    FILE* write_fp = fdopen(dup(client_sock), "w");
    if (!read_fp || !write_fp) {
        close(client_sock);
        return NULL;
    }

    char line[BUF_SIZE];
    char method[16], path[256], query[64] = {0}, value[32] = {0};
    fgets(line, sizeof(line), read_fp);

    printf("[DEBUG] 전체 요청 라인: %s", line);
    sscanf(line, "%s %s", method, path);
    printf("[DEBUG] 파싱된 메서드: %s\n", method);
    printf("[DEBUG] 파싱된 경로: %s\n", path);

    if (strcmp(method, "GET") != 0) {
        send_error(write_fp);
    } else if (strcmp(path, "/PoP.html") == 0) {
        send_response(write_fp, "200 OK", "text/html");
        send_file(write_fp, "../../PoP.html");
    } else if (strcmp(path, "/style.css") == 0) {
        send_response(write_fp, "200 OK", "text/css");
        send_file(write_fp, "../../style.css");
    } else {
        // 경로와 쿼리 분리
        char* qmark = strchr(path, '?');
        if (qmark) {
            *qmark = '\0';                      // path 절단
            strncpy(query, qmark + 1, 63);      // 쿼리 복사
        }

        int cat_id = 0, act_id = 0;

        if (strcmp(path, "/ledon") == 0) {
            cat_id = 1; act_id = 1;
        } else if (strcmp(path, "/ledoff") == 0) {
            cat_id = 1; act_id = 2;
        } else if (strcmp(path, "/ledctrl") == 0) {
            cat_id = 1; act_id = 3;
            sscanf(query, "value=%31s", value);
        } else if (strcmp(path, "/buzzeron") == 0) {
            cat_id = 2; act_id = 1;
        } else if (strcmp(path, "/buzzeroff") == 0) {
            cat_id = 2; act_id = 2;
        } else if (strcmp(path, "/cdsvalue") == 0) {
            cat_id = 3; act_id = 1;
        } else if (strcmp(path, "/cds_with_led") == 0) {
            cat_id = 3; act_id = 2;
        } else if (strcmp(path, "/segment") == 0) {
            cat_id = 4; act_id = 1;
            sscanf(query, "num=%31s", value);
        } else if (strcmp(path, "/pi_game_start") == 0) {
            cat_id = 5; act_id = 1;
        } else if (strcmp(path, "/pi_game_check") == 0) {
            cat_id = 5; act_id = 2;
            sscanf(query, "ans=%31s", value);
        }

        if (cat_id > 0 && act_id > 0) {
            printf("[DEBUG] menu_command(cat=%d, act=%d, val=%s)\n", cat_id, act_id, value);
            const char* result = menu_command(cat_id, act_id, strlen(value) > 0 ? value : NULL);
            send_response(write_fp, "200 OK", "text/plain");
            fprintf(write_fp, "%s\n", result ? result : "명령 처리 실패");
        } else {
            send_error(write_fp);
        }
    }

    fflush(write_fp);
    fclose(read_fp);
    fclose(write_fp);
    return NULL;
}




static void send_response(FILE* fp, const char* status, const char* content_type) {
    fprintf(fp, "HTTP/1.1 %s\r\n", status);
    fprintf(fp, "Content-Type: %s\r\n", content_type);
    fprintf(fp, "Connection: close\r\n\r\n");
}

static void send_file(FILE* fp, const char* filename) {
    char buf[BUF_SIZE];
    FILE* file = fopen(filename, "rb"); 
    if (!file) {
        fprintf(fp, "file open error: %s\n", filename);
        return;
    }

    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
        fwrite(buf, 1, len, fp); 
    }
    fclose(file);
}

static void send_error(FILE* fp) {
    send_response(fp, "404 Not Found", "text/html");
    fprintf(fp, "<html><body><h1>404 - Not Found</h1></body></html>");
}
