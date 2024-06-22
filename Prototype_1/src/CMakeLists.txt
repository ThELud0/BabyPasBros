add_library(lib_simpleGame
  Source/Game.cpp
  Source/RoundTarget.cpp
  Source/RoundTarget.cpp
 "Source/Player.cpp" "Source/Wall.cpp" "Source/Entity.cpp" "Source/Group.cpp" "Source/Door.cpp" "Source/Pacifier.cpp" "Source/UnmovingEntity.cpp" "Source/InteractibleUnmoving.cpp")

target_include_directories(lib_simpleGame
 PRIVATE
  Include
)

target_link_libraries(lib_simpleGame PUBLIC pugixml sfml-graphics sfml-audio)

add_executable(simpleGame
  Source/Main.cpp
 "Source/Player.cpp" "Source/Wall.cpp" "Source/Entity.cpp" "Source/Group.cpp" "Source/Door.cpp" "Source/Pacifier.cpp" "Source/UnmovingEntity.cpp" "Source/InteractibleUnmoving.cpp")

target_include_directories(simpleGame
 PRIVATE
  Include
)

target_link_libraries(simpleGame PUBLIC pugixml lib_simpleGame sfml-graphics sfml-audio)

add_custom_target(copy-resources ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(GLOB RESOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/resources/*.*)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/resources
                   DEPENDS ${CMAKE_SOURCE_DIR}/resources
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/resources
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${RESOURCES}
                           ${CMAKE_CURRENT_BINARY_DIR}/resources)
add_dependencies(simpleGame copy-resources)

if(WIN32)
   file(GLOB_RECURSE DYNAMIC_LIBS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/lib_openal32/${ARCH}/*.dll)
   file(COPY ${DYNAMIC_LIBS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()


# The following command is executed only when cmake is executed
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})