# message(STATUS "src This is BINARY dir " ${PROJECT_BINARY_DIR})
# message(STATUS "src This is SOURCE dir " ${PROJECT_SOURCE_DIR})

include_directories(../include)

aux_source_directory(. SRC)
aux_source_directory(../ MAIN)

#aux_source_directory(ETISrc/Times SRCDIR)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_executable(easyCppTools ${SRC} ${MAIN})

