#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>

#define SERVER_PORT 60000
#define BUF_SIZE 128

int sock;

void ignore_signal(int signo) {
    printf("\n[CLIENT] 강제 종료는 차단되어 있습니다.\n");
}

void send_command(int category, int action, const char* value) {
    char msg[BUF_SIZE];
    if (value && strlen(value) > 0)
        snprintf(msg, sizeof(msg), "%d %d %s", category, action, value);
    else
        snprintf(msg, sizeof(msg), "%d %d", category, action);

    if (send(sock, msg, strlen(msg), 0) < 0) {
        perror("send 실패");
        return;
    }

    char resp[BUF_SIZE] = {0};
    int len = recv(sock, resp, BUF_SIZE - 1, 0);
    if (len > 0) {
        resp[len] = '\0';
        printf("[서버 응답] %s\n", resp);
    }
}

void led_menu() {
    int choice;
    char level[16];
    while (1) {
        printf("\n[LED 제어 메뉴]\n");
        printf("0. 돌아가기\n1. 켜기\n2. 끄기\n3. 밝기 설정 (HIGH/MID/LOW)\n선택: ");
        scanf("%d", &choice);
        if (choice == 0) break;

        if (choice == 3) {
            printf("밝기 입력: ");
            scanf("%s", level);
            send_command(1, choice, level);
        } else {
            send_command(1, choice, NULL);
        }
    }
}

void buzzer_menu() {
    int choice;
    while (1) {
        printf("\n[BUZZER 제어 메뉴]\n0. 돌아가기\n1. 부저 ON\n2. 부저 OFF\n선택: ");
        scanf("%d", &choice);
        if (choice == 0) break;
        send_command(2, choice, NULL);
    }
}

void cds_menu() {
    int choice;
    while (1) {
        printf("\n[CDS 제어 메뉴]\n0. 돌아가기\n1. 조도값 확인\n2. 밝기값 LED 제어\n선택: ");
        scanf("%d", &choice);
        if (choice == 0) break;
        send_command(3, choice, NULL);
    }
}

void segment_menu() {
    int num;
    while (1) {
        printf("\n[7세그먼트 제어 메뉴]\n0. 돌아가기\n1. 카운트다운 (0~9)\n선택: ");
        scanf("%d", &num);
        if (num == 0) break;
        if (num != 1) continue;

        int input;
        printf("카운트다운 시작 숫자 입력 (0~9): ");
        scanf("%d", &input);

        char val[16];
        snprintf(val, sizeof(val), "%d", input);
        send_command(4, 1, val);
    }
}

void game_menu() {
    int choice;
    char answer[16];
    while (1) {
        printf("\n[GUESS Pi 게임 메뉴]\n0. 돌아가기\n1. 게임 시작\n2. 정답 입력\n선택: ");
        scanf("%d", &choice);
        if (choice == 0) break;

        if (choice == 2) {
            printf("정답 입력: ");
            scanf("%s", answer);
            send_command(5, choice, answer);
        } else {
            send_command(5, choice, NULL);
        }
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, ignore_signal);
    signal(SIGTERM, ignore_signal);

    if (argc != 2) {
        fprintf(stderr, "사용법: %s <서버 IP 주소>\n", argv[0]);
        return 1;
    }

    const char* server_ip = argv[1];

    struct sockaddr_in serv_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket 생성 실패");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, server_ip, &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("서버 연결 실패");
        exit(1);
    }

    int choice;
    while (1) {
        printf("\n===== Piece of Pi 클라이언트 입니다~~ =====\n");
        printf("1. LED 제어\n2. 부저 제어\n3. CDS 센서\n4. 7세그먼트\n5. GUESS Pi 게임\n0. 종료\n선택: ");
        scanf("%d", &choice);
        switch (choice) {
            case 0:
                printf("클라이언트 종료\n");
                close(sock);
                return 0;
            case 1: led_menu(); break;
            case 2: buzzer_menu(); break;
            case 3: cds_menu(); break;
            case 4: segment_menu(); break;
            case 5: game_menu(); break;
            default: printf("잘못된 입력\n");
        }
    }
}
