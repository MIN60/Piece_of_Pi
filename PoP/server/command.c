#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "command.h"

int handle_led_command(int action, const char* value);
int handle_buzzer_command(int action, const char* value);
int handle_cds_command(int action, const char* value);

int handle_segment_command(int action, const char* value);

int menu_command(int category, int action, const char* value) {
    switch (category) {
        case 1:
            return handle_led_command(action, value);
        case 2:
            return handle_buzzer_command(action, value);
        case 3:
            return handle_cds_command(action, value);
        case 4:
            return handle_segment_command(action, value);
        default:
            printf("[SYSTEM] 잘못된 선택입니다: %d\n", category);
            return -1;
    }
}


int handle_led_command(int action, const char* value) {
    void* handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[LED] dlopen 실패: %s\n", dlerror());
        return -1;
    }

    int (*led_init)();
    int (*led_on)();
    int (*led_off)();
    int (*led_set_brightness)(const char*);
    int (*led_clean)();

    led_init = dlsym(handle, "led_init");
    led_on   = dlsym(handle, "led_on");
    led_off  = dlsym(handle, "led_off");
    led_set_brightness = dlsym(handle, "led_set_brightness");
    led_clean = dlsym(handle, "led_clean");

    if (!led_init || !led_on || !led_off || !led_set_brightness || !led_clean) {
        fprintf(stderr, "[LED] dlsym 실패\n");
        dlclose(handle);
        return -1;
    }

    led_init();

    if (action == 1) {
        led_on();
    } else if (action == 2) {
        led_off();
    } else if (action == 3 && value != NULL) {
        led_set_brightness(value);
    } else {
        printf("[LED] 잘못된 명령입니다.\n");
    }

    return 0;
}


int handle_buzzer_command(int action, const char* value) {
    void* handle = dlopen("../lib/buzzer/libbuzzer.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[BUZZER] dlopen 실패: %s\n", dlerror());
        return -1;
    }

    int (*buzzer_init)();
    int (*buzzer_on)();
    int (*buzzer_off)();
    int (*buzzer_clean)();

    buzzer_init  = dlsym(handle, "buzzer_init");
    buzzer_on    = dlsym(handle, "buzzer_on");
    buzzer_off   = dlsym(handle, "buzzer_off");
    buzzer_clean = dlsym(handle, "buzzer_clean");

    if (!buzzer_init || !buzzer_on || !buzzer_off || !buzzer_clean) {
        fprintf(stderr, "[BUZZER] dlsym 실패\n");
        dlclose(handle);
        return -1;
    }

    buzzer_init();

    if (action == 1) {
        buzzer_on();
    } else if (action == 2) {
        buzzer_off();
    } else {
        printf("[BUZZER] 잘못된 명령입니다.\n");
    }

    return 0;
}

int handle_cds_command(int action, const char* value) {
    void* handle = dlopen("../lib/cds/libcds.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[CDS] dlopen 실패: %s\n", dlerror());
        return -1;
    }

    int (*cds_get_value)();
    int (*cds_with_led)();

    cds_get_value = dlsym(handle, "cds_get_value");
    cds_with_led  = dlsym(handle, "cds_with_led");

    if (!cds_get_value || !cds_with_led) {
        fprintf(stderr, "[CDS] dlsym 실패\n");
        dlclose(handle);
        return -1;
    }

    if (action == 1) {
        cds_get_value();      
    } else if (action == 2) {
        cds_with_led();    
    } else {
        printf("[CDS] 잘못된 명령입니다.\n");
    }

    return 0;
}


int handle_segment_command(int action, const char* value) {
    void* handle = dlopen("../lib/segment/libsegment.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[SEGMENT] dlopen 실패: %s\n", dlerror());
        return -1;
    }

    int (*segment_init)();
    int (*seg_countdown)(int);

    segment_init = dlsym(handle, "segment_init");
    seg_countdown = dlsym(handle, "seg_countdown");

    if (!segment_init || !seg_countdown) {
        fprintf(stderr, "[SEGMENT] dlsym 실패\n");
        dlclose(handle);
        return -1;
    }

    segment_init();

    if (action == 1 && value != NULL) {
        int num = atoi(value);
        if (num < 0 || num > 9) {
            printf("[SEGMENT] 0~9 사이 숫자만 입력\n");
            return -1;
        }
        seg_countdown(num);
    } else {
        printf("[SEGMENT] 잘못된 명령\n");
    }

    return 0;
}
