#ifndef COMMAND_H
#define COMMAND_H

int menu_command(int category, int action, const char* value);

// 장치
int handle_led_command(int action, const char* value);
int handle_buzzer_command(int action, const char* value);
int handle_cds_command(int action, const char* value);


#endif
