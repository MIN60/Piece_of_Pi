#include <stdio.h>
#include <wiringPi.h>
#include <unistd.h>
#include <dlfcn.h>
#include "segment.h"

// wiringPi 핀 배열 (a~g + dp)
static const int pins[8] = {
    27, // a
    26, // b
    21, // c
    22, // d
    30, // e
    31, // f
    25, // g
    5   // dp
};
static const int numbers[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};

// 초기화
int segment_init() {
    wiringPiSetup();
    for (int i = 0; i < 8; ++i) {
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], HIGH);
    }
    //printf("[SEGMENT] 초기화 완료\n");
    return 0;
}

// 숫자 표시
void segment_display(int num) {
    if (num < 0 || num > 9) return;

    for (int i = 0; i < 7; ++i) {
        digitalWrite(pins[i], numbers[num][i] ? LOW : HIGH);
    }
    digitalWrite(pins[7], HIGH);  // dp 동그라미는 꺼둠
}

// 모두 끄기
void segment_clear() {
    for (int i = 0; i < 8; ++i) {
        digitalWrite(pins[i], HIGH);
    }
    //printf("[SEGMENT] 클리어\n");
}

// 숫자 표시 후 1초마다 감소 → 0이면 부저 울림
int seg_countdown(int num) {
    segment_init();

    for (int i = num; i >= 0; --i) {
        segment_display(i);
        //printf("[SEGMENT] %d 표시\n", i);
        sleep(1);
    }

    segment_clear();

    // 부저 울리기
    void* handle = dlopen("../lib/buzzer/libbuzzer.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[SEGMENT] 부저 로드 실패: %s\n", dlerror());
        return -1;
    }

    int (*buzzer_init)();
    int (*buzzer_on)();
    int (*buzzer_off)();

    buzzer_init = dlsym(handle, "buzzer_init");
    buzzer_on   = dlsym(handle, "buzzer_on");
    buzzer_off  = dlsym(handle, "buzzer_off");

    if (buzzer_init && buzzer_on && buzzer_off) {
        buzzer_init();
        buzzer_on();
        sleep(2);
        buzzer_off();
    } else {
        fprintf(stderr, "[SEGMENT] 부저 dlsym 실패\n");
    }

    return 0;
}
