#ifndef BUZZER_H
#define BUZZER_H

int buzzer_init();
int buzzer_on();
int buzzer_off();
int buzzer_clean();

int buzzer_play(int freq);
int buzzer_stop();
#endif
