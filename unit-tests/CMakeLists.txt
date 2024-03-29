cmake_minimum_required(VERSION 3.11)

enable_testing()

add_executable(database_test ../src/database.c database.c)
target_link_libraries(database_test ${SQLite3_LIBRARIES})
add_test(NAME database_test COMMAND $<TARGET_FILE:database_test>)

add_executable(voice_test ../src/voice.c voice.c)
target_link_libraries(voice_test m)
add_test(NAME voice_test COMMAND $<TARGET_FILE:voice_test>)
