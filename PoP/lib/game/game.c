#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../buzzer/buzzer.h"
#include "../segment/segment.h"

static int correct_answer[3] = {3, 1, 4};  //정답은 3.14!

static int notes[6] = {0, 261, 293, 329, 349, 392};  // 1~5번 음

static void play_note(int num, int duration_ms) {
    show_number(num);
    buzzer_play(notes[num]);
    usleep(duration_ms * 1000);
    buzzer_stop();
    usleep(300 * 1000);
}

static void play_warning() {
    for (int i = 0; i < 3; ++i) {
        buzzer_play(1000);
        usleep(300 * 1000);
        buzzer_stop();
        usleep(200 * 1000);
    }
}

static void play_melody() {
    int melody[4] = {261, 329, 392, 523};
    for (int i = 0; i < 4; ++i) {
        buzzer_play(melody[i]);
        usleep(250 * 1000);
        buzzer_stop();
        usleep(100 * 1000);
    }
}

static void play_error() {
    for (int i = 0; i < 2; ++i) {
        buzzer_play(100);
        usleep(200 * 1000);
        buzzer_stop();
        usleep(200 * 1000);
    }
}

void pi_game_start() {
    printf("[GAME] 게임 시작: 랜덤 음 5개 + 문제 음 3개 재생\n");

    srand(time(NULL));
    for (int i = 0; i < 5; ++i) {
        int n = (rand() % 5) + 1;
        play_note(n, 2000);
    }

    play_warning();

    for (int i = 0; i < 3; ++i) {
        play_note(correct_answer[i], 1000);
    }
}

void pi_game_check(const char* ans) {
    printf("[GAME] 입력된 정답: %s\n", ans);
    if (strlen(ans) != 3) {
        printf("[GAME] 잘못된 입력 길이\n");
        play_error();
        return;
    }

    for (int i = 0; i < 3; ++i) {
        if ((ans[i] - '0') != correct_answer[i]) {
            printf("[GAME] 오답\n");
            play_error();
            return;
        }
    }

    printf("[GAME] 정답!\n");
    play_melody();
}
