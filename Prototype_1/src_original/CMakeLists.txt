add_executable(simpleGameOriginal
  Source/Game.cpp Source/Main.cpp
  )

target_include_directories(simpleGameOriginal
 PRIVATE
  Include
)

target_link_libraries(simpleGameOriginal PUBLIC sfml-graphics)

# The following command is executed only when cmake is executed
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})