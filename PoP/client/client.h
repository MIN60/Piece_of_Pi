#ifndef CLIENT_H
#define CLIENT_H

#define SERVER_IP "192.168.0.76"
#define SERVER_PORT 60000
#define BUF_SIZE 128

void send_command(int category, int action, const char* value);
void led_menu();
void buzzer_menu();
void cds_menu();
void segment_menu();
void game_menu();

#endif
