#!/bin/bash
BASE_DIR="$(cd "$(dirname "$0")" && pwd)"

export LD_LIBRARY_PATH="$BASE_DIR/build-arm64/lib:$LD_LIBRARY_PATH"
echo "[INFO] LD_LIBRARY_PATH 설정됨: $LD_LIBRARY_PATH"

cd "$BASE_DIR/build-arm64/server"
echo "[INFO] 서버 실행 중..."
./pop_server

