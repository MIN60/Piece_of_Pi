#!/bin/bash

PROJECT="PoP"

mkdir -p $PROJECT/{build,common,client/include,server/include}
mkdir -p $PROJECT/lib/{led,buzzer,sensor,segment}

# 공통
echo " " > $PROJECT/common/protocol.h
echo "add_library(common STATIC protocol.h)" > $PROJECT/common/CMakeLists.txt

# client
cat <<EOF > $PROJECT/client/CMakeLists.txt
add_executable(pop_client client.c)
target_include_directories(pop_client PRIVATE ../common include)
target_link_libraries(pop_client ssl crypto)
EOF

# server
cat <<EOF > $PROJECT/server/CMakeLists.txt
add_executable(pop_server main.c command.c daemon.c)
target_include_directories(pop_server PRIVATE ../common include)
target_link_libraries(pop_server ssl crypto dl pthread)
EOF

for mod in led buzzer sensor segment; do
  cat <<EOF > $PROJECT/lib/$mod/CMakeLists.txt
add_library($mod SHARED ${mod}.c)
target_include_directories($mod PUBLIC \${CMAKE_CURRENT_SOURCE_DIR})
EOF
  touch $PROJECT/lib/$mod/${mod}.c
  touch $PROJECT/lib/$mod/${mod}.h
done

# 최상위 CMakeLists.txt
cat <<EOF > $PROJECT/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(PoP C)

set(CMAKE_C_STANDARD 99)

include_directories(common)

add_subdirectory(common)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(lib/led)
add_subdirectory(lib/buzzer)
add_subdirectory(lib/sensor)
add_subdirectory(lib/segment)
EOF

echo "'$PROJECT' 프로젝트 디렉토리 및 CMakeLists.txt 생성"

