#ifndef COMMAND_H
#define COMMAND_H

const char* menu_command(int category, int action, const char* value);

const char* handle_led_command(int action, const char* value);
const char* handle_buzzer_command(int action, const char* value);
const char* handle_cds_command(int action, const char* value);
const char* handle_segment_command(int action, const char* value);
const char* handle_game_command(int action, const char* value);

#endif // COMMAND_H
