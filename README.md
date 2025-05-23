# Piece of Pi
Piece of Pi는 TCP를 이용한 원격 장치 제어 프로그램입니다.<br/>
라즈베리파이4에 올린 Server와 우분투 리눅스 환경에서의 Client를 통해 LED/Buzzer/CDS/7Segment 를 조작할 수 있습니다. 

## 구현 내용
- 멀티 스레드를 이용한 장치 제어
- 공유 라이브러리를 이용한 장치 제어
- 강제종료가 방지된 Client 프로그램
- Daemon 프로세스 형식의 Server
- CMAKE를 이용한 빌드 자동화
- 웹 서버를 이용한 장치 제어

## 구현 기능
1. LED 제어
2. 부저 제어
3. CDS 센서
4. 7세그먼트
5. GUESS Pi 게임
6. 웹 GUI 접속

### 세부 구현 내용
![image](https://github.com/user-attachments/assets/de374aa9-14ba-4b84-bd06-2b4c2abd5ede)

### 1. LED 제어
클라이언트<br/>
![image](https://github.com/user-attachments/assets/7fdcaacc-15da-4a5c-8cac-e2c3def32fe1)

서버
```
menu_command(1, action, value) → handle_led_command()
```

공유 라이브러리
```
libled.so
```

웹
![image](https://github.com/user-attachments/assets/b259b0b4-648c-4730-a90b-5196fc552443)



### 2. BUZZER 제어
클라이언트
![image](https://github.com/user-attachments/assets/0621260d-336d-467b-9711-a9e0064c6058)

서버
```
menu_command(2, action, NULL) → handle_buzzer_command()
```

공유 라이브러리
```
libbuzzer.so
```

웹
![image](https://github.com/user-attachments/assets/f93258b6-73f7-4d7a-ae3c-bf02836f2297)

### 3. CDS 센서
클라이언트
![image](https://github.com/user-attachments/assets/599a3aa3-5452-427d-91c0-3b9ec806b664)

서버
```
menu_command(3, action, NULL) → handle_cds_command()
```

공유 라이브러리
```
libcds.so
```

웹
![image](https://github.com/user-attachments/assets/15dfc4ee-8c04-46ee-9f44-bd028e3a54c1)

### 4. 7세그먼트
클라이언트
![image](https://github.com/user-attachments/assets/d2364abb-0916-43d9-b7a2-30c3823e95e9)

서버
```
menu_command(4, 1, value) → handle_segment_command()
```

공유 라이브러리
```
libsegment.so
```

웹
![image](https://github.com/user-attachments/assets/14d1bb6e-20d4-4267-9fbb-9d89634b983c)

### 5. GUESS Pi 게임 - 추가 기능
클라이언트
![image](https://github.com/user-attachments/assets/5a873243-7355-4f9a-a750-4ef2b33ffef9)

서버
```
menu_command(5, action, value) → handle_game_command()
```

공유 라이브러리
```
libgame.so
```

웹
![image](https://github.com/user-attachments/assets/c79ce69b-5085-477f-8d25-269d3195f2be)


### 6. 웹 GUI 접속 - 추가 기능
클라이언트
![image](https://github.com/user-attachments/assets/dcf5750c-33d0-4d45-8c18-dc32a5bb207f)

웹
![image](https://github.com/user-attachments/assets/0b3516a7-f881-4988-b925-ec03b51ab0cd)


아코디언 식으로 제작되어 각 카테고리를 누르면 상세 내용이 나옵니다.
![image](https://github.com/user-attachments/assets/fd05b058-ec81-4afe-991e-a85ff2769453)


### 서버
Daemon 형식으로 구현
![image](https://github.com/user-attachments/assets/f73b4f18-cd7b-4f81-9b1f-37578dbd6048)



## 사용자 편의 기능

## 실행 환경

### 라즈베리파이4 회로구성

### 실행 명령어
