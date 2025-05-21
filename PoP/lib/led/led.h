#ifndef LED_H
#define LED_H

int led_init();  //초기화
int led_on();  // led 켬
int led_off(); //led 끔
int led_set_brightness(const char* level);  // high mid low로 밝기 설정
int led_clean(); // led 해제

#endif
