#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "command.h"

const char* handle_led_command(int action, const char* value);
const char* handle_buzzer_command(int action, const char* value);
const char* handle_cds_command(int action, const char* value);
const char* handle_segment_command(int action, const char* value);
const char* handle_game_command(int action, const char* value);

const char* menu_command(int category, int action, const char* value) {
    switch (category) {
        case 1: return handle_led_command(action, value);
        case 2: return handle_buzzer_command(action, value);
        case 3: return handle_cds_command(action, value);
        case 4: return handle_segment_command(action, value);
        case 5: return handle_game_command(action, value);
        default: return "[SYSTEM] 잘못된 명령입니다.";
    }
}

const char* handle_led_command(int action, const char* value) {
    void* handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
    if (!handle) return "[LED] dlopen 실패";

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
        dlclose(handle);
        return "[LED] dlsym 실패";
    }

    led_init();

    if (action == 1) {
        led_on();
        return "LED ON 완료";
    } else if (action == 2) {
        led_off();
        return "LED OFF 완료";
    } else if (action == 3 && value) {
        led_set_brightness(value);
        return "LED 밝기 설정 완료";
    } else {
        return "[LED] 잘못된 명령입니다.";
    }
}

const char* handle_buzzer_command(int action, const char* value) {
    void* handle = dlopen("../lib/buzzer/libbuzzer.so", RTLD_LAZY);
    if (!handle) return "[BUZZER] dlopen 실패";

    int (*buzzer_init)();
    int (*buzzer_on)();
    int (*buzzer_off)();
    int (*buzzer_clean)();

    buzzer_init  = dlsym(handle, "buzzer_init");
    buzzer_on    = dlsym(handle, "buzzer_on");
    buzzer_off   = dlsym(handle, "buzzer_off");
    buzzer_clean = dlsym(handle, "buzzer_clean");

    if (!buzzer_init || !buzzer_on || !buzzer_off || !buzzer_clean) {
        dlclose(handle);
        return "[BUZZER] dlsym 실패";
    }

    buzzer_init();

    if (action == 1) {
        buzzer_on();
        return "부저 ON 완료";
    } else if (action == 2) {
        buzzer_off();
        return "부저 OFF 완료";
    } else {
        return "[BUZZER] 잘못된 명령입니다.";
    }
}

const char* handle_cds_command(int action, const char* value) {
    void* handle = dlopen("../lib/cds/libcds.so", RTLD_LAZY);
    if (!handle) return "[CDS] dlopen 실패";

    int (*cds_get_value)();
    int (*cds_with_led)();

    cds_get_value = dlsym(handle, "cds_get_value");
    cds_with_led  = dlsym(handle, "cds_with_led");

    if (!cds_get_value || !cds_with_led) {
        dlclose(handle);
        return "[CDS] dlsym 실패";
    }

    static char msg[64];
    if (action == 1) {
        int cds_val = cds_get_value();
        snprintf(msg, sizeof(msg), "조도값: %d", cds_val);
        return msg;
    } else if (action == 2) {
        cds_with_led();
        return "CDS + LED 처리 완료";
    } else {
        return "[CDS] 잘못된 명령입니다.";
    }
}

const char* handle_segment_command(int action, const char* value) {
    void* handle = dlopen("../lib/segment/libsegment.so", RTLD_LAZY);
    if (!handle) return "[SEGMENT] dlopen 실패";

    int (*segment_init)();
    int (*seg_countdown)(int);

    segment_init = dlsym(handle, "segment_init");
    seg_countdown = dlsym(handle, "seg_countdown");

    if (!segment_init || !seg_countdown) {
        dlclose(handle);
        return "[SEGMENT] dlsym 실패";
    }

    segment_init();

    if (action == 1 && value) {
        int num = atoi(value);
        if (num < 0 || num > 9)
            return "[SEGMENT] 0~9 사이 숫자만 입력하세요.";

        seg_countdown(num);
        return "카운트다운 완료";
    } else {
        return "[SEGMENT] 잘못된 명령입니다.";
    }
}

const char* handle_game_command(int action, const char* value) {
    void* handle = dlopen("../lib/game/libgame.so", RTLD_LAZY);
    if (!handle) return "[GAME] dlopen 실패";

    void (*pi_game_start)();
    void (*pi_game_check)(const char*);

    pi_game_start = dlsym(handle, "pi_game_start");
    pi_game_check = dlsym(handle, "pi_game_check");

    if (!pi_game_start || !pi_game_check) {
        dlclose(handle);
        return "[GAME] dlsym 실패";
    }

    if (action == 1) {
        pi_game_start();
        return "게임 시작!";
    } else if (action == 2 && value) {
        pi_game_check(value);
        return "정답 제출 완료";
    } else {
        return "[GAME] 잘못된 명령입니다.";
    }
}
