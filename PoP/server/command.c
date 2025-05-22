#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include "utils.h"
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
    printf("[LED DEBUG] handle_led_command() 진입 (action=%d, value=%s)\n", action, value ? value : "NULL");

    //void* handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
    void* handle = dlopen(get_library_path("lib/led/libled.so"), RTLD_LAZY);

    if (!handle) {
        printf("[LED DEBUG] dlopen 실패: %s\n", dlerror());
        return "[LED] dlopen 실패";
    }
    printf("[LED DEBUG] dlopen 성공\n");

    // 함수 포인터
    int (*led_init)();
    int (*led_on)();
    int (*led_off)();
    int (*led_set_brightness)(const char*);
    int (*led_clean)();

    // 심볼 로드
    led_init = dlsym(handle, "led_init");
    led_on   = dlsym(handle, "led_on");
    led_off  = dlsym(handle, "led_off");
    led_set_brightness = dlsym(handle, "led_set_brightness");
    led_clean = dlsym(handle, "led_clean");

    if (!led_init || !led_on || !led_off || !led_set_brightness || !led_clean) {
        printf("[LED DEBUG] dlsym 실패: %s\n", dlerror());
        dlclose(handle);
        return "[LED] dlsym 실패";
    }

    // 초기화
    if (led_init() != 0) {
        printf("[LED DEBUG] led_init 실패\n");
        dlclose(handle);
        return "[LED] 초기화 실패";
    }
    printf("[LED DEBUG] led_init() 호출 완료\n");

    const char* result = "[LED] 잘못된 명령입니다.";

    // 실행 분기
    if (action == 1) {
        printf("[LED DEBUG] led_on() 호출 시작\n");
        led_on();
        printf("[LED DEBUG] led_on() 호출 완료\n");
        result = "LED ON 완료";
    } else if (action == 2) {
        printf("[LED DEBUG] led_off() 호출 시작\n");
        led_off();
        result = "LED OFF 완료";
    } else if (action == 3 && value) {
        printf("[LED DEBUG] led_set_brightness(%s) 호출 시작\n", value);
        led_set_brightness(value);
        result = "LED 밝기 설정 완료";
    }

    // 해제 후 반환
    //dlclose(handle);
    //printf("[LED DEBUG] dlclose(handle) 완료 후 반환\n");
    return result;
}

const char* handle_buzzer_command(int action, const char* value) {
    void* handle = dlopen(get_library_path("lib/buzzer/libbuzzer.so"), RTLD_LAZY);
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
    
    void* handle = dlopen(get_library_path("lib/cds/libcds.so"), RTLD_LAZY);
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
        //cds_with_led();
        int state = cds_with_led();  // 예: 밝음이면 0, 어두움이면 1 리턴
        if (state == 1)
            return "어두움 → LED ON";
        else
            return "밝음 → LED OFF";

        return "CDS + LED 처리 완료";
        
    } else {
        return "[CDS] 잘못된 명령입니다.";
    }
}

const char* handle_segment_command(int action, const char* value) {
    void* handle = dlopen(get_library_path("lib/segment/libsegment.so"), RTLD_LAZY);
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

    void* handle = dlopen(get_library_path("lib/game/libgame.so"), RTLD_LAZY);
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
