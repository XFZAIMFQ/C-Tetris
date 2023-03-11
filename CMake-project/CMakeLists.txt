cmake_minimum_required(VERSION 3.24)
project(Tetris)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ ")
#set(CMAKE_EXE_LINKER_FLAGS "-lpthread -L libwinpthread-1.dll")
#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")

include_directories(./EasyX/include)
link_directories(./EasyX/lib64)
link_libraries(libeasyx.a)

add_executable(Tetris main.cpp main1.cpp main2.cpp data.h test.cpp TCS.cpp)
add_executable(main1 main1.cpp)
add_executable(main main.cpp)
add_executable(main2 main2.cpp)
add_executable(test test.cpp)
add_executable(TCS TCS.cpp)