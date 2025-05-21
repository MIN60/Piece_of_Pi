#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "led.h"

#define LED_PIN 1  //wiringPi GPIO 18

// LED 초기화
int led_init() {
    wiringPiSetup(); // wiringPi 초기화
    pinMode(LED_PIN, OUTPUT); // 핀 출력 모드 설정
    softPwmCreate(LED_PIN, 0, 100); //PWM 생성
    printf("[LED] 초기화 완료 (핀: %d)\n", LED_PIN);
    return 0;
}

//led on
int led_on() {
    digitalWrite(LED_PIN, HIGH);
    printf("[LED] 켜짐\n");
    return 0;
}

//led off
int led_off() {
    digitalWrite(LED_PIN, LOW);
    printf("[LED] 꺼짐\n");
    return 0;
}

// 밝기 지정 high=100 mid=50, low=20
int led_set_brightness(const char* level) {
    int value = 0;

    if (strcmp(level, "HIGH") == 0) value = 100;
    else if (strcmp(level, "MEDIUM") == 0) value = 50;
    else if (strcmp(level, "LOW") == 0) value = 10;
    else {
        fprintf(stderr, "[LED] 밝기 설정 오류: '%s'\n", level);
        return -1;
    }

    softPwmWrite(LED_PIN, value); //PWM 값 설정
    printf("[LED] 밝기 설정: %s (%d)\n", level, value);
    return 0;
}

//led 해제
int led_clean() {
    softPwmWrite(LED_PIN, 0); // PWM 끄기
    pinMode(LED_PIN, INPUT);  // 핀 입력모드로 초기화
    printf("[LED] 종료 및 핀 해제\n");
    return 0;
}
