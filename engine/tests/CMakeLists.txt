include(FetchContent)

FetchContent_Declare(check
        GIT_REPOSITORY https://github.com/libcheck/check.git
        GIT_TAG        0.15.2)
FetchContent_MakeAvailable(check)

add_executable(check_engine tests.c containers/check_darray.c)
target_link_libraries(check_engine Check::check engine)
