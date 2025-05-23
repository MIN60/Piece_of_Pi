# Piece of Pi

## 프로젝트 개요
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

## 프로젝트 구조
```
PoP/
├── build-arm64/                   
│   ├── client/                          
│   ├── common/                     
│   ├── lib/                          
│   │   ├── [libbuzzer.so](http://libbuzzer.so/)
│   │   ├── [libcds.so](http://libcds.so/)
│   │   ├── [libgame.so](http://libgame.so/)
│   │   ├── [libled.so](http://libled.so/)
│   │   └── [libsegment.so](http://libsegment.so/)
│   └── server/                    

├── build-x86/                         
│   ├── client/                      
│   ├── common/
│   ├── lib/
│   ├── server/
│   ├── CMakeFiles/             
│   ├── CMakeCache.txt
│   ├── Makefile
│   └── cmake_install.cmake

├── client/                              # 클라이언트 소스 코드
│   ├── client.c                        
│   └── pi_web/                          # 웹
│       ├── PoP.html
│       └── style.css

├── common/                             
│   ├── utils.c
│   ├── utils.h
│   └── CMakeLists.txt

├── lib/                                 # 각 장치 기능 라이브러리
│   ├── buzzer/
│   │   ├── buzzer.c
│   │   ├── buzzer.h
│   │   └── CMakeLists.txt
│   ├── cds/
│   │   ├── cds.c
│   │   ├── cds.h
│   │   └── CMakeLists.txt
│   ├── game/
│   │   ├── game.c
│   │   ├── game.h
│   │   └── CMakeLists.txt
│   ├── led/
│   │   ├── led.c
│   │   ├── led.h
│   │   └── CMakeLists.txt
│   └── segment/
│       ├── segment.c
│       ├── segment.h
│       └── CMakeLists.txt

├── server/                           
│   ├── main.c
│   ├── command.c
│   ├── command.h
│   ├── web_server.c
│   ├── web_server.h
│   └── CMakeLists.txt

├── [build.sh](http://build.sh/)                            
├── CMakeLists.txt                     
├── start_server.sh                      # 서버 실행 스크립트
├── start_client.sh                      # 클라이언트 실행 스크립트
└──  toolchain-arm64.cmake
```

## 세부 구현 내용
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

게임 시작을 선택하면 5개의 음이 들립니다.<br/>
이후 띠-띠-띠 알림음이 울립니다.<br/>
알림음 이후 3개의 음이 울립니다.<br/>
이 3개의 음을 맞추는 게임입니다.<br/>
정답 힌트는 프로젝트명에 있습니다! (Pi=3.14=314)<br/><br/>

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

클라이언트에서 웹 메뉴를 선택하면 자동으로 웹페이지가 열립니다.<br/>
웹 페이지 내에서 모든 조작이 가능합니다.<br/><br/>

클라이언트
![image](https://github.com/user-attachments/assets/dcf5750c-33d0-4d45-8c18-dc32a5bb207f)

웹
![image](https://github.com/user-attachments/assets/0b3516a7-f881-4988-b925-ec03b51ab0cd)


아코디언 식으로 제작되어 각 카테고리를 누르면 상세 내용이 나옵니다.
![image](https://github.com/user-attachments/assets/fd05b058-ec81-4afe-991e-a85ff2769453)


### 서버
Daemon 형식으로 구현<br/>
![image](https://github.com/user-attachments/assets/f73b4f18-cd7b-4f81-9b1f-37578dbd6048)


## 사용자 편의 기능
### 서버, 클라이언트 실행 간편화
.sh 파일 작성을 통해 내부 폴더에서 간단한 명령어를 통해 프로그램을 실행할 수 있음<br/>
![image](https://github.com/user-attachments/assets/b2beca5d-29c3-4c8d-aa06-97fc4a3572ce)<br/>
```
cd ~/PoP
./start_server.sh
```
▲ 서버 실행<br/>

![image](https://github.com/user-attachments/assets/4df5f663-0c5c-4be2-8947-74639d7c4c4c)
```
cd ~/PoP
./start_client.sh
```
▲ 클라이언트 실행<br/>
서버 실행 시 ip주소가 출력되어 별도의 명령어 없이 확인 후 클라이언트와 연결 가능합니다. <br/>

### 웹 GUI

![image](https://github.com/user-attachments/assets/a873c117-94fa-4873-af5c-bd7c3b5d7b9e)<br/>
웹 GUI 구성을 통해 사용자가 편하게 장치를 제어할 수 있도록 만들었습니다.<br/>
![image](https://github.com/user-attachments/assets/720b5749-0d61-4b7b-8421-20329dddda2b)
클라이언트 메뉴에서 GUI 메뉴를 선택하면 인터넷에 주소를 입력할 필요 없이 창이 자동으로 열립니다.


## 동작 영상

[![Video Label](http://img.youtube.com/vi/xZbWqxYo6DE/0.jpg)](https://youtu.be/xZbWqxYo6DE)<br/>
▲ 클릭 시 영상이 실행됩니다.<br/><br/>
영상은 github 폴더 전체를 다운 받은 것으로, 경로가 ~/Pop/PoP 에서 시작합니다.<br/>
git관련 파일이 존재하는 PoP 폴더(.ignore 등)를 제외한 PoP 폴더를 가지고 있을 경우 ~/PoP에서 시작하시면 됩니다.<br/>


## 실행 환경
### 라즈베리파이4 회로구성
![image](https://github.com/user-attachments/assets/1dfe6ec7-db6a-489b-b3e8-81b5fe89c782)<br/>
세그먼트 연결 시 방향 주의!
<br/><br/>
### 실행 명령어
순서를 따라 실행해주세요.<br/>
***경로 이동***<br/>
git에서 바로 다운 받았을 때 기준으로, PoP내부에 PoP 폴더가 하나 더 있습니다.<br/>
그럴 때는
```
cd ~/PoP/PoP
```

git 파일이 없는 PoP 단일 폴더일 경우(안에 PoP 폴더가 하나 더 없을 경우)
```
cd ~/PoP
```

***빌드***<br/>
```
./build.sh
```
위의 명령으로 자동 빌드됩니다.<br/>
build-arm64 폴더와 build-x86 폴더가 생기게 됩니다.<br/>

이제 이 PoP 폴더가 (1)라즈베리파이 서버, (2) 우분투 리눅스 클라이언트 2곳에 존재해야 합니다.

***라즈베리파이 서버 실행***<br/>
위의 '경로 이동'에 작성해 둔 것 처럼, 라즈베리파이 PoP 폴더 내부 경로에서
![image](https://github.com/user-attachments/assets/f26a37f8-e733-4e1d-8fee-b375a2cca86d)<br/>
사진처럼 .sh 파일이 있는 곳에서
```
./start_server.sh
```
명령어로 서버를 실행합니다.<br/>
![image](https://github.com/user-attachments/assets/6f7d950b-debb-4d06-8e72-639126c580c3)<br/>
여기서 ip주소를 확인 가능합니다.<br/>

***우분투 클라이언트 실행***<br/>
위의 '경로 이동'에 작성해 둔 것 처럼, 우분투 리눅스 PoP 폴더 내부 경로에서
![image](https://github.com/user-attachments/assets/cbc1f3cc-894b-40c2-82b9-7aa377fb3f04)
사진처럼 .sh 파일이 있는 곳에서
```
./start_client.sh
```
명령어로 클라이언트를 실행합니다.<br/>
![image](https://github.com/user-attachments/assets/fe53c94d-2f52-40a8-806a-2afc92a4bd11)<br/>
서버 실행으로 알 수 있었던 ip 주소를 입력하면 접속이 완료됩니다.<br/>


