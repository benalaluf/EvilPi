
file(GLOB_RECURSE COMMON_SOURCES "src/*.cpp" "include/*.h")

function(create_executable target_name main_source)
    add_executable(${target_name} ${COMMON_SOURCES} ${main_source})
    target_include_directories(${target_name} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endfunction()


create_executable(Admin main_admin_conn.cpp)
create_executable(ProxyServer main_server_proxy.cpp)
create_executable(Client main_client_conn.cpp)
create_executable(MainTest main_test.cpp)
