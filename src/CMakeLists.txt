# message(STATUS "src This is BINARY dir " ${PROJECT_BINARY_DIR})
# message(STATUS "src This is SOURCE dir " ${PROJECT_SOURCE_DIR})

include_directories(../include)

aux_source_directory(. SRC)
aux_source_directory(../ MAIN)

#aux_source_directory(ETISrc/Times SRCDIR)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# add_library(EasyCppTools SHARED ${SRC} ${MAIN})   # 生成动态库文件

add_executable(${PROJECT_NAME} ${SRC} ${MAIN}) # 生成可执行文件