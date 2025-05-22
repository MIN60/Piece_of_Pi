#!/bin/bash
set -e

BASE_DIR=$(dirname "$(readlink -f "$0")")
export LD_LIBRARY_PATH="$BASE_DIR/build-arm64/lib:$LD_LIBRARY_PATH"

IP=$(hostname -I | awk '{print $1}')
echo "[INFO] 서버 IP 주소는: $IP"
echo "$IP" > "$BASE_DIR/server_ip.txt"

cd "$BASE_DIR/build-arm64/server"
echo "[INFO] 서버 실행 중..."
./pop_server
