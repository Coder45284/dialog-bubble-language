cmake_minimum_required(VERSION 3.11)

enable_testing()

add_executable(sql_test ../src/sql.c sql.c)
target_link_libraries(sql_test ${SQLite3_LIBRARIES})
add_test(NAME sql_test COMMAND $<TARGET_FILE:sql_test>)
