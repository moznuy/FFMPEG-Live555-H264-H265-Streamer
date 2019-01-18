cmake_minimum_required(VERSION 3.10)
project(Test1)

set(CMAKE_CXX_STANDARD 11)

message("Additional path: $ENV{FFMPEG_PATH}")
set(CMAKE_PREFIX_PATH "$ENV{FFMPEG_PATH}")

SET(live555example_HEADERS
        Live555FFMPEG/Definitions.h
        Live555FFMPEG/FFMPEGClass.h
        Live555FFMPEG/Live555Class.h
        Live555FFMPEG/Live555UsageEnv.h
        Live555FFMPEG/Live_AnalyserInput.h
        Live555FFMPEG/Live_AnalyserSource.h
        Live555FFMPEG/Live_AnalysingServerMediaSubsession.h
        Live555FFMPEG/MutexUtility.h
        )

SET(live555example_SOURCES
        Live555FFMPEG/FFMPEGClass.cpp
        Live555FFMPEG/Live555Class.cpp
        Live555FFMPEG/Live555FFMPEG.cpp
        Live555FFMPEG/Live_AnalyserInput.cpp
        Live555FFMPEG/Live_AnalyserSource.cpp
        Live555FFMPEG/Live_AnalysingServerMediaSubsession.cpp
        )


# mark headers as headers...
SET_SOURCE_FILES_PROPERTIES( ${live555example_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE )
# append to sources so that dependency checks work on headers
LIST(APPEND live555example_SOURCES ${live555example_HEADERS})

add_executable(live555Example ${live555example_SOURCES})



# PKG_CONFIG
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    message("PkgConfig Found")
endif()


## SDL2
#pkg_check_modules(SDL2 REQUIRED sdl2)
#if (SDL2_FOUND)
#    message("SDL2 found: ${SDL2_VERSION}")
#    message("Libraries: ${SDL2_LIBRARIES}")
#    message("Include dirs: ${SDL2_INCLUDE_DIRS}")
#    message("Additional flags: ${SDL2_CFLAGS_OTHER}")
#endif()
#
#target_link_libraries(Test1 ${SDL2_LIBRARIES})
#target_include_directories(Test1 PUBLIC ${SDL2_INCLUDE_DIRS})
#target_compile_options(Test1 PUBLIC ${SDL2_CFLAGS_OTHER})
#
#
# FFMPEG
pkg_check_modules(FFMPEG REQUIRED libavformat libavcodec libswscale libavutil)
if (FFMPEG_FOUND)
    message("FFMPEG found: ${FFMPEG_VERSION}")
    message("Libraries: ${FFMPEG_LINK_LIBRARIES}")
    message("Include dirs: ${FFMPEG_INCLUDE_DIRS}")
    message("Additional flags: ${FFMPEG_CFLAGS_OTHER}")
endif()

target_link_libraries(live555Example PRIVATE ${FFMPEG_LINK_LIBRARIES})
target_include_directories(live555Example PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_compile_options(live555Example PRIVATE ${FFMPEG_CFLAGS_OTHER})


# LIVE555
message("LIVE555 path: $ENV{LIVE555}")
message("LIVE555 path: $ENV{LIVE555}")
target_link_options(live555Example PRIVATE -L$ENV{LIVE555}/lib)
target_link_libraries(live555Example PRIVATE groupsock liveMedia BasicUsageEnvironment UsageEnvironment)
target_include_directories(live555Example PRIVATE $ENV{LIVE555}/include/BasicUsageEnvironment)
target_include_directories(live555Example PRIVATE $ENV{LIVE555}/include/groupsock)
target_include_directories(live555Example PRIVATE $ENV{LIVE555}/include/liveMedia)
target_include_directories(live555Example PRIVATE $ENV{LIVE555}/include/UsageEnvironment)

## Threads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(live555Example PRIVATE Threads::Threads)
