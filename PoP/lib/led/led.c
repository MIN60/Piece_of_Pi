#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <pthread.h>
#include "led.h"

#define LED_PIN 1  // wiringPi 번호 (GPIO 18)

// 내부 초기화 여부 및 뮤텍스
static int initialized = 0;
static pthread_mutex_t led_mutex = PTHREAD_MUTEX_INITIALIZER;

// 초기화 함수는 main에서 호출하되, 재호출 대비 로직도 포함
int led_init() {
    pthread_mutex_lock(&led_mutex);

    if (initialized) {
        pthread_mutex_unlock(&led_mutex);
        return 0;
    }

    if (wiringPiSetup() == -1) {
        fprintf(stderr, "[LED INIT] wiringPi 초기화 실패\n");
        pthread_mutex_unlock(&led_mutex);
        return -1;
    }

    pinMode(LED_PIN, OUTPUT);

    if (softPwmCreate(LED_PIN, 0, 100) != 0) {
        fprintf(stderr, "[LED INIT] softPwmCreate 실패\n");
        pthread_mutex_unlock(&led_mutex);
        return -1;
    }

    initialized = 1;
    pthread_mutex_unlock(&led_mutex);
    return 0;
}

// ON
int led_on() {
    pthread_mutex_lock(&led_mutex);
    softPwmWrite(LED_PIN, 100);
    pthread_mutex_unlock(&led_mutex);
    return 0;
}

// OFF
int led_off() {
    pthread_mutex_lock(&led_mutex);
    softPwmWrite(LED_PIN, 0);
    pthread_mutex_unlock(&led_mutex);
    return 0;
}

// 밝기 설정
int led_set_brightness(const char* level) {
    int value = 0;

    if (strcmp(level, "HIGH") == 0) value = 100;
    else if (strcmp(level, "MID") == 0) value = 50;
    else if (strcmp(level, "LOW") == 0) value = 10;
    else {
        fprintf(stderr, "[LED] 밝기 설정 오류: '%s'\n", level);
        return -1;
    }

    pthread_mutex_lock(&led_mutex);
    softPwmWrite(LED_PIN, value);
    pthread_mutex_unlock(&led_mutex);
    return 0;
    char* get_library_path(const char* subpath);
}

// 정리
int led_clean() {
    pthread_mutex_lock(&led_mutex);
    softPwmWrite(LED_PIN, 0);
    pinMode(LED_PIN, INPUT);
    initialized = 0;
    pthread_mutex_unlock(&led_mutex);
    return 0;
}
