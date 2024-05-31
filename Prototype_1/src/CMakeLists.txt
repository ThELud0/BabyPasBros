add_library(lib_simpleGame
  Source/Game.cpp
  Source/RoundTarget.cpp
  Source/RoundTarget.cpp
 "Source/Player.cpp")

target_include_directories(lib_simpleGame
 PRIVATE
  Include
)

target_link_libraries(lib_simpleGame PUBLIC sfml-graphics)

add_executable(simpleGame
  Source/Main.cpp
 "Source/Player.cpp")

target_include_directories(simpleGame
 PRIVATE
  Include
)

target_link_libraries(simpleGame PUBLIC lib_simpleGame sfml-graphics)

# The following command is executed only when cmake is executed
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})