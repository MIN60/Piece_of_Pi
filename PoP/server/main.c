#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "command.h"

//쓰레드에서 menu_command 호출
typedef struct {
    int category;
    int action;
    char value[16];
} CommandArgs;

void* command_thread(void* arg) {
    CommandArgs* cmd = (CommandArgs*)arg;
    fflush(stdout); 
    menu_command(cmd->category, cmd->action, strlen(cmd->value) > 0 ? cmd->value : NULL);
    free(cmd);
    return NULL;
}

// LED 메뉴
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
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("LED 켜기\n");
                break;
            case 2:
                printf("LED 끄기\n");
                break;
            case 3:
                printf("밝기 설정\n");
                break;
            default:
                printf("잘못된 입력\n");
                continue;
        }

        // 명령 구조체 생성
        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 1;
        cmd->action = choice;
        cmd->value[0] = '\0';

        if (choice == 3) {
            printf("밝기 입력 (HIGH/MID/LOW): ");
            scanf("%s", cmd->value);
        }

        // 스레드 실행
        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_join(tid, NULL);  // join으로 안정화
        }
    }
}

// 부저 메뉴
void buzzer_menu() {
    int choice;

    while (1) {
        printf("\n[BUZZER 제어 메뉴]\n");
        printf("0. 돌아가기\n");
        printf("1. 부저 ON\n");
        printf("2. 부저 OFF\n");
        printf("선택: ");
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("부저 켜기\n");
                break;
            case 2:
                printf("부저 끄기\n");
                break;

            default:
                printf("잘못된 입력\n");
                continue;
        }

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 2; 
        cmd->action = choice;
        cmd->value[0] = '\0';   

        // 스레드 실행
        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_join(tid, NULL); 
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
        scanf("%d", &choice);

        if (choice == 0) break;

        switch (choice) {
            case 1:
                printf("조도값 확인\n");
                break;
            case 2:
                printf("밝기 기준 LED 제어\n");
                break;
            default:
                printf("잘못된 입력\n");
                continue;
        }

        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 3; 
        cmd->action = choice;
        cmd->value[0] = '\0';

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_join(tid, NULL);
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
        scanf("%d", &num);

        if (num == 0) break;
        if (num != 1) {
            printf("잘못된 입력입니다.\n");
            continue;
        }

        int input;
        printf("카운트다운 시작 숫자 입력 (0~9): ");
        scanf("%d", &input);

        if (input < 0 || input > 9) {
            printf("0~9 사이 숫자만 입력하세요.\n");
            continue;
        }

        // 명령 구조체
        CommandArgs* cmd = malloc(sizeof(CommandArgs));
        cmd->category = 4;
        cmd->action = 1;
        snprintf(cmd->value, sizeof(cmd->value), "%d", input);

        pthread_t tid;
        if (pthread_create(&tid, NULL, command_thread, (void*)cmd) != 0) {
            perror("쓰레드 생성 실패");
            free(cmd);
        } else {
            pthread_join(tid, NULL);
        }
    }
}



int main() {
    int choice;

    while (1) {
        printf("\n===== Piece of Pi =====\n");
        printf("1. LED 제어\n");
        printf("2. 부저 제어\n");
        printf("3. 조도센서 확인\n");
        printf("4. 7세그먼트 제어\n");
        printf("0. 종료\n");
        printf("선택: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                printf("프로그램을 종료합니다.\n");
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
            default:
                printf("다시 선택하세욤\n");
        }
    }

    return 0;
}
