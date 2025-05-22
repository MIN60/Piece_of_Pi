#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>

static int correct_answer[3] = {3, 1, 4};  // 정답: 3.14!
static int notes[6] = {0, 261, 293, 329, 349, 392};  // 1~5번 음

// 부저 함수 로딩
static int load_buzzer_funcs(void** handle, int (**play)(int), int (**stop)()) {
    *handle = dlopen("../lib/buzzer/libbuzzer.so", RTLD_LAZY);
    if (!*handle) {
        fprintf(stderr, "[GAME] buzzer dlopen 실패: %s\n", dlerror());
        return -1;
    }

    *play = dlsym(*handle, "buzzer_play");
    *stop = dlsym(*handle, "buzzer_stop");

    if (!*play || !*stop) {
        fprintf(stderr, "[GAME] buzzer dlsym 실패\n");
        dlclose(*handle);
        return -1;
    }

    return 0;
}

static void play_note(int num, int duration_ms) {
    // --- segment ---
    void* seg_handle = dlopen("../lib/segment/libsegment.so", RTLD_LAZY);
    if (!seg_handle) {
        fprintf(stderr, "[GAME] segment dlopen 실패: %s\n", dlerror());
        return;
    }

    void (*segment_init)();
    void (*segment_display)(int);
    void (*segment_clear)();  // 선언

    segment_init = dlsym(seg_handle, "segment_init");
    segment_display = dlsym(seg_handle, "segment_display");
    segment_clear = dlsym(seg_handle, "segment_clear"); 

    if (!segment_init || !segment_display || !segment_clear) {
        fprintf(stderr, "[GAME] segment 함수 로드 실패\n");
        dlclose(seg_handle);
        return;
    }

    segment_init();
    segment_clear();       
    segment_display(num);   
    dlclose(seg_handle);

    // --- buzzer ---
    void* buz_handle;
    int (*buzzer_play)(int);
    int (*buzzer_stop)();
    if (load_buzzer_funcs(&buz_handle, &buzzer_play, &buzzer_stop) != 0) return;

    buzzer_play(notes[num]);
    usleep(duration_ms * 1000);
    buzzer_stop();
    usleep(300 * 1000);
    dlclose(buz_handle);
}


static void play_note_no_display(int num, int duration_ms) {
    // buzzer only
    void* buz_handle;
    int (*buzzer_play)(int);
    int (*buzzer_stop)();
    if (load_buzzer_funcs(&buz_handle, &buzzer_play, &buzzer_stop) != 0) return;

    buzzer_play(notes[num]);
    usleep(duration_ms * 1000);
    buzzer_stop();
    usleep(300 * 1000);
    dlclose(buz_handle);
}




static void play_warning() {
    void* handle;
    int (*play)(int);
    int (*stop)();

    if (load_buzzer_funcs(&handle, &play, &stop) != 0) return;

    for (int i = 0; i < 3; ++i) {
        play(1000);
        usleep(300 * 1000);
        stop();
        usleep(200 * 1000);
    }

    dlclose(handle);
}

static void play_melody() {
    void* handle;
    int (*play)(int);
    int (*stop)();

    if (load_buzzer_funcs(&handle, &play, &stop) != 0) return;

    int melody[4] = {261, 329, 392, 523};
    for (int i = 0; i < 4; ++i) {
        play(melody[i]);
        usleep(250 * 1000);
        stop();
        usleep(100 * 1000);
    }

    dlclose(handle);
}

static void play_error() {
    void* handle;
    int (*play)(int);
    int (*stop)();

    if (load_buzzer_funcs(&handle, &play, &stop) != 0) return;

    for (int i = 0; i < 2; ++i) {
        play(100);
        usleep(200 * 1000);
        stop();
        usleep(200 * 1000);
    }

    dlclose(handle);
}

void pi_game_start() {
    //printf("[GAME] 게임 시작: 1~5 순차 재생 + 문제 음 재생\n");

    // 1~5 순서대로 숫자와 음 재생
    for (int i = 1; i <= 5; ++i) {
        play_note(i, 1000);  // 1초짜리 음 재생
    }

    sleep(2);  // 2초 대기
    play_warning();  // 띠띠띠

    // 정답 음 (3, 1, 4) 재생
    for (int i = 0; i < 3; ++i) {
        play_note_no_display(correct_answer[i], 1000);
    }

    // --- 세그먼트 클리어 ---
    void* seg_handle = dlopen("../lib/segment/libsegment.so", RTLD_LAZY);
    if (!seg_handle) {
        fprintf(stderr, "[GAME] segment dlopen 실패 (clear용): %s\n", dlerror());
        return;
    }

    void (*segment_init)() = dlsym(seg_handle, "segment_init");
    void (*segment_clear)() = dlsym(seg_handle, "segment_clear");

    if (segment_init && segment_clear) {
        segment_init();     // GPIO 핀 설정
        segment_clear();    // 화면 초기화
    } else {
        fprintf(stderr, "[GAME] segment dlsym 실패 (init or clear)\n");
    }

    dlclose(seg_handle);
}



void pi_game_check(const char* ans) {
    //printf("[GAME] 입력된 정답: %s\n", ans);
    if (strlen(ans) != 3) {
        printf("[GAME] 잘못된 입력 길이\n");
        play_error();
        return;
    }

    for (int i = 0; i < 3; ++i) {
        if ((ans[i] - '0') != correct_answer[i]) {
            //printf("[GAME] 오답\n");
            play_error();
            return;
        }
    }

    //printf("[GAME] 정답!\n");
    play_melody();


    // 세그먼트 정리
void* seg_handle = dlopen("../lib/segment/libsegment.so", RTLD_LAZY);
if (seg_handle) {
    void (*segment_init)() = dlsym(seg_handle, "segment_init");
    void (*segment_clear)() = dlsym(seg_handle, "segment_clear");
    if (segment_init && segment_clear) {
        segment_init();
        segment_clear();
    }
    dlclose(seg_handle);
}

}
