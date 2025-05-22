#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "command.h"
#include "../segment/segment.h"


typedef struct {
    int category;
    int action;
    char value[16];
} CommandArgs;

void* command_thread(void* arg) {
    CommandArgs* cmd = (CommandArgs*)arg;
    menu_command(cmd->category, cmd->action, strlen(cmd->value) > 0 ? cmd->value : NULL);
    free(cmd);
    return NULL;
}

void led_menu() {
    int choice;
    char level[16];

    while (1) {     
        printf("\n[LED 제어 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 켜기\n");
        printf("2. 끄기\n");
        printf("3. 밝기 설정 (HIGH/MID/LOW)\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 0) break;

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 1;
        cmd->action = choice;
        cmd->value[0] = '\0';

        switch (choice) {
            case 1:
                printf("LED 켜기 선택됨\n");
                break;
            case 2:
                printf("LED 끄기 선택됨\n");
                break;
            case 3:
                printf("밝기 설정 선택됨\n");
                printf("밝기 입력 (HIGH/MID/LOW): ");
                fflush(stdout);
                scanf("%s", cmd->value);
                break;
            default:
                printf("잘못된 입력\n");
                free(cmd);
                continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_detach(tid);
        }
    }
}

void buzzer_menu() {
    int choice;

    while (1) {
        printf("\n[BUZZER 제어 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 부저 ON\n");
        printf("2. 부저 OFF\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 0) break;

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 2;
        cmd->action = choice;
        cmd->value[0] = '\0';

        switch (choice) {
            case 1:
                printf("부저 ON 선택됨\n");
                break;
            case 2:
                printf("부저 OFF 선택됨\n");
                break;
            default:
                printf("잘못된 입력\n");
                free(cmd);
                continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_detach(tid);
        }
    }
}

void cds_menu() {
    int choice;

    while (1) {
        printf("\n[CDS 제어 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 조도값 확인\n");
        printf("2. 밝기값 LED 제어\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 0) break;

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 3;
        cmd->action = choice;
        cmd->value[0] = '\0';

        switch (choice) {
            case 1:
                printf("조도값 확인 선택됨\n");
                break;
            case 2:
                printf("밝기값 기반 LED 제어 선택됨\n");
                break;
            default:
                printf("잘못된 입력\n");
                free(cmd);
                continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_detach(tid);
        }
    }
}

void segment_menu() {
    int num;

    while (1) {
        printf("\n[7세그먼트 제어 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 카운트다운 (0~9)\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &num);

        if (num == 0) break;
        if (num != 1) {
            printf("잘못된 입력입니다.\n");
            continue;
        }

        int input;
        printf("카운트다운 시작 숫자 입력 (0~9): ");
        fflush(stdout);
        scanf("%d", &input);

        if (input < 0 || input > 9) {
            printf("0~9 사이 숫자만 입력하세요.\n");
            continue;
        }

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 4;
        cmd->action = 1;
        snprintf(cmd->value, sizeof(cmd->value), "%d", input);

        printf("카운트다운 %d 시작 선택됨\n", input);

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_detach(tid);
        }
    }
}



void game_menu() {
    int choice;
    char answer[16];

    while (1) {
        printf("\n[GUESS Pi 게임 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 게임 시작\n");
        printf("2. 정답 입력 (예: 314)\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &choice);

        if (choice == 0) break;

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 5;
        cmd->action = choice;
        cmd->value[0] = '\0';

        switch (choice) {
            case 1:
                printf("게임을 시작합니다!\n");
                break;
            case 2:
                printf("정답을 입력하세요 (예: 512): ");
                scanf("%s", cmd->value);
                break;
            default:
                printf("잘못된 입력입니다.\n");
                free(cmd);
                continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_detach(tid);
        }
    }
}



int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    int choice;

    while (1) {
        printf("\n===== Piece of Pi =====\n");
        printf("1. LED 제어\n");
        printf("2. 부저 제어\n");
        printf("3. 조도센서 확인\n");
        printf("4. 7세그먼트 제어\n");
        printf("5. GUESS Pi 게임\n");
        printf("0. 종료\n");
        printf("선택: ");
        fflush(stdout);
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printf("프로그램을 종료합니다.\n");
                //segment_clean();
                return 0;
            case 1:
                led_menu();
                break;
            case 2:
                buzzer_menu();
                break;
            case 3:
                cds_menu();
                break;
            case 4:
                segment_menu();
                break;
            case 5:
                game_menu();
                break;
            default:
                printf("다시 선택하세욤\n");
        }
    }

    
    return 0;
}



