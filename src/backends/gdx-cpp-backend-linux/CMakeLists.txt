project(gdx-cpp-backend-linux)

include_directories(${GDXCPP_INCLUDE_DIR} ${SVG_BACKEND_ANTIGRAIN_INCLUDES})

set(GDX_CPP_BACKEND_LINUX_SRC LinuxApplication.cpp LinuxGL10.cpp LinuxOpenALMusic.cpp
LinuxGraphics.cpp LinuxGL20.cpp LinuxGL11.cpp LinuxGLU.cpp LinuxSystem.cpp LinuxInput.cpp LinuxFiles.cpp LinuxFileHandle.cpp init.cpp
LinuxOpenALAudio.cpp LinuxOpenALSound.cpp LinuxOgg.cpp LinuxOggInputStream.cpp LinuxOpenALMusic.cpp
)
set(GDX_CPP_BACKEND_LINUX_HEADERS LinuxApplication.hpp LinuxGL10.hpp LinuxGraphics.hpp 
LinuxGL20.hpp LinuxFiles.hpp LinuxFileHandle.hpp LinuxGL11.hpp LinuxGLU.hpp LinuxSystem.hpp LinuxInput.hpp
LinuxOpenALAudio.hpp LinuxOpenALSound.hpp LinuxOgg.hpp LinuxOggInputStream.hpp LinuxOpenALMusic.hpp
)

add_library(gdx-cpp-backend-linux SHARED ${GDX_CPP_BACKEND_LINUX_SRC} ${GDX_CPP_BACKEND_LINUX_HEADERS})
add_dependencies(gdx-cpp-backend-linux gdx-cpp gdx-cpp-agg-svg)

target_link_libraries(gdx-cpp-backend-linux openal vorbis ogg vorbisfile gdx-cpp SDL pthread rt)

install(DIRECTORY . DESTINATION include/gdx-cpp/backends/gdx-cpp-backend-linux
          FILES_MATCHING PATTERN "*.hpp")

install(TARGETS gdx-cpp-backend-linux
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

