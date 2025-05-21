#include <stdio.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>

// 공유 라이브러리, 쓰레드
void* led_thread(void* arg) {
    void* handle = dlopen("../lib/led/libled.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "[LED] dlopen 실패: %s\n", dlerror());
        return NULL;
    }

    int (*led_init)();
    int (*led_on)();
    int (*led_off)();

    int (*led_clean)(); 

    // 함수 포인터
    led_init = dlsym(handle, "led_init");
    led_on = dlsym(handle, "led_on");
    led_off = dlsym(handle, "led_off");
    led_clean = dlsym(handle, "led_clean");

    //로딩
    if (!led_init || !led_on || !led_off || !led_clean) {
        fprintf(stderr, "[LED] dlsym 실패\n");
        dlclose(handle);
        return NULL;
    }

    //testt
    led_init();
    led_on();
    sleep(1);
    led_off();
    led_clean();

    dlclose(handle);
    return NULL;
}

int main() {
    pthread_t tid;

    printf("LED 테스트\n");

    //쓰레드 생성
    if (pthread_create(&tid, NULL, led_thread, NULL) != 0) {
        perror("쓰레드 생성 실패");
        return 1;
    }

    // 쓰레드 완료 대기
    pthread_join(tid, NULL);

    printf("PoP 서버 종료\n");
    return 0;
}
