
set (CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -static")

add_executable(main "main.cpp" "Socket.cpp" "WSALoader.hpp" "ISenadble.hpp" "t.cpp" "ConnectionManager.cpp" "Client.hpp" "./easyloggingpp/src/easylogging++.cc" ISendable.cpp)
set_source_files_properties(main.c PROPERTIES LANGUAGE CXX )
include_directories(${argparse/include/argparse/})
include_directories(${easyloggingpp/src/})


target_link_libraries(main wsock32 ws2_32)

