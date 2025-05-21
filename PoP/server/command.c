#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "command.h"

int menu_command(int category, int action, const char* value) {
    void* handle = NULL;

    // LED 제어만 해놨음 이따가 더 써야됨
    if (category == 1) {  // LED
        handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "[LED] dlopen 실패: %s\n", dlerror());
            return -1;
        }

        // 함수 포인터
        int (*led_init)();
        int (*led_on)();
        int (*led_off)();
        int (*led_set_brightness)(const char*);
        int (*led_clean)();

        led_init = dlsym(handle, "led_init");
        led_on = dlsym(handle, "led_on");
        led_off = dlsym(handle, "led_off");
        led_set_brightness = dlsym(handle, "led_set_brightness");
        led_clean = dlsym(handle, "led_clean");

        if (!led_init || !led_on || !led_off || !led_set_brightness || !led_clean) {
            fprintf(stderr, "[LED] dlsym 실패\n");
            dlclose(handle);
            return -1;
        }

        led_init();

        if (action == 1) { // 켜기
            led_on();
        } else if (action == 2) { // 끄기
            led_off();
        } else if (action == 3 && value != NULL) { // 밝기 설정
            led_set_brightness(value);
        } else {
            printf("[LED] 잘못된 명령\n");
        }

        //led_clean();
        //dlclose(handle);
        return 0;
    }

    printf("category: %d\n", category);
    return -1;
}
