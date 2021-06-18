project("DeepMatching Helper")
cmake_minimum_required(VERSION 2.60)

set(DEEPMATCHING_PATH "DEEPMATCHING-NOT-FOUND" CACHE PATH "DeepMatching source code path")

if(DEEPMATCHING_PATH STREQUAL "DEEPMATCHING-NOT-FOUND")
	message(FATAL_ERROR "There're no DeepMatching path")
endif()

include_directories(${DEEPMATCHING_PATH})
set(DEEPMATCHING_HEADER_FILES 
	${DEEPMATCHING_PATH}/array_types.h 
	${DEEPMATCHING_PATH}/conv.h 
	${DEEPMATCHING_PATH}/deep_matching.h 
    ${DEEPMATCHING_PATH}/hog.h 
    ${DEEPMATCHING_PATH}/image.h 
	${DEEPMATCHING_PATH}/maxfilter.h 
	${DEEPMATCHING_PATH}/pixel_desc.h
	${DEEPMATCHING_PATH}/std.h )
set(DEEPMATCHING_SRC_FILES 
	${DEEPMATCHING_PATH}/conv.cpp 
	${DEEPMATCHING_PATH}/deep_matching.cpp 
	${DEEPMATCHING_PATH}/hog.cpp 
	${DEEPMATCHING_PATH}/image.cpp 
	${DEEPMATCHING_PATH}/maxfilter.cpp 
	${DEEPMATCHING_PATH}/pixel_desc.cpp
	${DEEPMATCHING_PATH}/std.cpp)

add_definitions(-DLIBDEEPMATCHING_HELPER_EXPORTS)
set(DEEPMATCHING_HELPER_FILES dm_helper.h dm_helper.cpp)
add_library("dm_helper" SHARED ${DEEPMATCHING_HEADER_FILES} ${DEEPMATCHING_SRC_FILES} ${DEEPMATCHING_HELPER_FILES})
target_link_libraries("dm_helper")

