#!/bin/bash
set -e

BASE_DIR=$(dirname "$(readlink -f "$0")")

# 라이브러리 경로 설정
export LD_LIBRARY_PATH="$BASE_DIR/build-arm64/lib:$LD_LIBRARY_PATH"

# 서버 IP 확인 및 저장
IP=$(hostname -I | awk '{print $1}')
echo "[INFO] 서버 IP 주소는: $IP"
echo "$IP" > "$BASE_DIR/server_ip.txt"

# 웹 리소스 복사 (PoP.html, style.css → BASE_DIR에 위치시킴)
cp "$BASE_DIR/client/pi_web/PoP.html" "$BASE_DIR/"
cp "$BASE_DIR/client/pi_web/style.css" "$BASE_DIR/"
echo "[INFO] PoP.html 및 style.css 복사 완료"

# 서버 실행 (데몬화, 로그 분리)
cd "$BASE_DIR/build-arm64/server"
echo "[INFO] 서버 실행 중 (백그라운드)..."
./pop_server > /tmp/pop_server.log 2>&1 &

echo "[INFO] 서버 PID: $!"
echo "[INFO] 로그: tail -f /tmp/pop_server.log"
