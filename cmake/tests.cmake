enable_testing()

add_executable(TLX_TEST test/main.cpp)

target_link_libraries(TLX_TEST PRIVATE
    TLX
)