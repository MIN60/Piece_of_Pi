#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#include "command.h"
#include "web_server.h"


#define PORT 60000
#define BACKLOG 10
#define BUF_SIZE 128

int server_sock;

void sigint_handler(int signo) {
    printf("\n[SYSTEM] SIGINT 수신됨, 서버 종료 중...\n");
    close(server_sock);
    exit(0);
}

void ignore_signal(int signo) {
    // 아무 동작 안 함
}

void* client_handler(void* arg) {
    int client_sock = *(int*)arg;
    free(arg);

    char buf[BUF_SIZE];
    int n;

    while ((n = recv(client_sock, buf, BUF_SIZE - 1, 0)) > 0) {
        buf[n] = '\0';
        printf("[RECV] %s\n", buf);

        int category, action;
        char value[32] = {0};

        // "1 3 MID" 형식 파싱
        int parsed = sscanf(buf, "%d %d %31s", &category, &action, value);
        const char* val_ptr = (parsed == 3) ? value : NULL;

        const char* response = menu_command(category, action, val_ptr);
        if (!response) response = "ERROR\n";

        send(client_sock, response, strlen(response), 0);
    }

    printf("[INFO] 클라이언트 연결 종료\n");
    close(client_sock);
    return NULL;
}

void daemonize(const char* work_dir) {
    pid_t pid = fork();
    if (pid < 0) exit(1);
    if (pid > 0) exit(0);  // 부모 프로세스 종료

    if (setsid() < 0) exit(1);
    umask(0);

    if (chdir(work_dir) < 0) {
        perror("[DAEMON] 작업 디렉토리 변경 실패");
        exit(1);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int fd = open("/tmp/pop_server.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
    close(fd); 


}

int main() {

    signal(SIGPIPE, SIG_IGN);

    char cwd[256];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd 실패");
        exit(1);
    }

    // 시그널 등록
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, ignore_signal);
    signal(SIGQUIT, ignore_signal);

    //  데몬화
    daemonize(cwd); 

    // 웹 서버 스레드 시작
    pthread_t web_tid;
    if (pthread_create(&web_tid, NULL, (void*(*)(void*))start_web_server, (void*)(intptr_t)8080) != 0) {
        perror("[WEB] 웹 서버 스레드 생성 실패");
        exit(1);
    }

    // 소켓 생성
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket 실패");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind 실패");
        exit(1);
    }

    if (listen(server_sock, BACKLOG) < 0) {
        perror("listen 실패");
        exit(1);
    }

    printf("[SYSTEM] 서버가 포트 %d에서 실행 중입니다...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int* client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addrlen);
        if (*client_sock < 0) {
            perror("accept 실패");
            free(client_sock);
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void*)client_sock) != 0) {
            perror("pthread_create 실패");
            close(*client_sock);
            free(client_sock);
        } else {
            pthread_detach(tid);
        }
    }

    return 0;
}
