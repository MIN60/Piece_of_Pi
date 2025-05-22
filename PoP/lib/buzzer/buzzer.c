#include <stdio.h>
#include <wiringPi.h>
#include <softTone.h>
#include <unistd.h>
#include "buzzer.h"
#include <pthread.h> 

#define BUZZER_PIN 4  // GPIO 23 → wiringPi 4번 핀

// 학교종 멜로디 (계이름 주파수 배열)
static const int notes[] = {
    391, 391, 440, 440, 391, 391, 329, 329,
    391, 391, 329, 329, 293, 293, 293, 0,
    391, 391, 440, 440, 391, 391, 329, 329,
    391, 329, 293, 329, 261, 261, 261, 0
};

static int isPlaying = 0;
static pthread_t buzzer_thread;

void* bplay_thread(void* arg) {
    int length = sizeof(notes) / sizeof(notes[0]);
    for (int i = 0; i < length && isPlaying; ++i) {
        softToneWrite(BUZZER_PIN, notes[i]);
        usleep(300000);  // 0.3초
    }
    softToneWrite(BUZZER_PIN, 0);
    return NULL;
}

// 초기화
int buzzer_init() {
    static int initialized = 0;
    if (initialized) return 0;
    wiringPiSetup();
    pinMode(BUZZER_PIN, OUTPUT); 
    softToneCreate(BUZZER_PIN); 
    initialized = 1;
    //printf("[BUZZER] 초기화 (핀: %d)\n", BUZZER_PIN);
    return 0;
}

int buzzer_on() {
    if (isPlaying) return 0;
    isPlaying = 1;
    pthread_create(&buzzer_thread, NULL, bplay_thread, NULL);
    //printf("[BUZZER] 재생 시작\n");
    return 0;
}

int buzzer_off() {
    if (!isPlaying) return 0;
    isPlaying = 0;
    pthread_join(buzzer_thread, NULL);  
    softToneWrite(BUZZER_PIN, 0);
    //printf("[BUZZER] 음악 정지\n");
    return 0;
}


//종료
int buzzer_clean() {
    buzzer_off();
    pinMode(BUZZER_PIN, INPUT);
    //printf("[BUZZER] 부저 종료\n");
    return 0;
}
