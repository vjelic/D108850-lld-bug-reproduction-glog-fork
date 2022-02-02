cmake_minimum_required(VERSION 3.16)

project(lld-multiinvocation-bisector VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM: ${LLVM_DIR}")
find_package(LLD REQUIRED CONFIG HINTS "${LLVM_BUILD_BINARY_DIR}" "${LLVM_INSTALL_PREFIX}/tools")
message(STATUS "Found LLD: ${LLD_DIR}")

list(GET LLD_INCLUDE_DIRS 0 LLD_SRC_INCLUDE)
execute_process(COMMAND grep "-q" "bool disableOutput"
    "${LLD_SRC_INCLUDE}/lld/Common/Driver.h"
    RESULT_VARIABLE HAS_OLD_LINK)

add_executable(wrapper wrapper.cpp)
target_include_directories(wrapper PRIVATE ${LLVM_INCLUDE_DIRS} ${LLD_INCLUDE_DIRS})
target_link_libraries(wrapper lldCommon lldELF)
add_compile_definitions(OLD_LINK=${HAS_OLD_LINK})
