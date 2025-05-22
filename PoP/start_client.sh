#!/bin/bash
set -e

BASE_DIR=$(dirname "$(readlink -f "$0")")

# 서버 IP 확인
if [[ -f "$BASE_DIR/server_ip.txt" ]]; then
    SERVER_IP=$(cat "$BASE_DIR/server_ip.txt")
else
    read -p "서버 IP 주소를 입력하세요: " SERVER_IP
fi

echo "[INFO] 연결할 서버 IP: $SERVER_IP"

# client 실행
cd "$BASE_DIR/build-x86/client"
./client "$SERVER_IP"

