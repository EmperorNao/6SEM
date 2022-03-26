cmake_minimum_required(VERSION 2.8)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")


add_executable(main "main.cpp" "Socket.cpp" "WSALoader.hpp" "ISenadble.hpp" "t.cpp" "ConnectionManager.cpp" "Client.hpp")
set_source_files_properties(main.c PROPERTIES LANGUAGE CXX )

if(WIN32)
  target_link_libraries(main wsock32 ws2_32)
endif()
