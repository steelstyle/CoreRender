
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

include_directories(../../CoreRender/include)

add_executable(ProceduralContent ProceduralContent.cpp)
target_link_libraries(ProceduralContent CoreRender)
