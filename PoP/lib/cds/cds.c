#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <dlfcn.h>
#include "cds.h"

#define PCF8591_ADDR 0x48   // 기본 I2C 주소
#define THRESHOLD 180// 임계

int cds_get_value() {
    int fd = wiringPiI2CSetupInterface("/dev/i2c-1", PCF8591_ADDR);
    if (fd < 0) {
        perror("[CDS] 초기화 실패");
        return -1;
    }

    wiringPiI2CWrite(fd, 0x00); 
    wiringPiI2CRead(fd);
    int val = wiringPiI2CRead(fd);

    //printf("[CDS] 현재 조도값: %d\n", val);
    printf("[CDS] 현재 조도값: %d → %s\n", val, val > THRESHOLD ? "Dark" : "Bright");

    return val;
}

int cds_with_led() {
    int val = cds_get_value();
    if (val == -1) return -1;

    void* handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[CDS] LED 로드 실패: %s\n", dlerror());
        return -1;
    }

    int (*led_init)();
    int (*led_on)();
    int (*led_off)();

    led_init = dlsym(handle, "led_init");
    led_on   = dlsym(handle, "led_on");
    led_off  = dlsym(handle, "led_off");

    if (!led_init || !led_on || !led_off) {
        fprintf(stderr, "[CDS] LED 함수 로딩 실패\n");
        dlclose(handle);
        return -1;
    }

    led_init();

    if (val > THRESHOLD) {
        printf("[CDS] 어두움 → LED 켜기\n");
        led_on();
    } else {
        printf("[CDS] 밝음 → LED 끄기\n");
        led_off();
    }
    

    // dlclose(handle); 
    return val;
}
