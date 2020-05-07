
cmake_minimum_required(VERSION 3.1)

project(Ladis)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)
find_package(Threads REQUIRED)

add_library(
    Ladis
    SHARED
    src/sdl/file/SDL_rwops.c
    src/sdl/audio/SDL_audiocvt.c
    src/sdl/audio/SDL_mixer_m68k.c
    src/sdl/audio/SDL_audiodev.c
    src/sdl/audio/SDL_mixer_MMX_VC.c
    src/sdl/audio/SDL_wave.c
    src/sdl/audio/dummy/SDL_dummyaudio.c
    src/sdl/audio/SDL_mixer.c
    src/sdl/audio/SDL_mixer_MMX.c
    src/sdl/audio/SDL_audio.c
    src/sdl/cpuinfo/SDL_cpuinfo.c
    src/sdl/main/dummy/SDL_dummy_main.c
    src/sdl/SDL_error.c
    src/sdl/thread/SDL_thread.c
    src/sdl/thread/pthread/SDL_sysmutex.c
    src/sdl/thread/pthread/SDL_syscond.c
    src/sdl/thread/pthread/SDL_systhread.c
    src/sdl/thread/pthread/SDL_syssem.c
    src/sdl/events/SDL_quit.c
    src/sdl/events/SDL_expose.c
    src/sdl/events/SDL_mouse.c
    src/sdl/events/SDL_resize.c
    src/sdl/events/SDL_events.c
    src/sdl/events/SDL_active.c
    src/sdl/events/SDL_keyboard.c
    src/sdl/video/SDL_pixels.c
    src/sdl/video/SDL_stretch.c
    src/sdl/video/SDL_bmp.c
    src/sdl/video/SDL_gamma.c
    src/sdl/video/SDL_RLEaccel.c
    src/sdl/video/SDL_video.c
    src/sdl/video/SDL_blit_0.c
    src/sdl/video/SDL_blit_1.c
    src/sdl/video/SDL_yuv_mmx.c
    src/sdl/video/dummy/SDL_nullevents.c
    src/sdl/video/dummy/SDL_nullmouse.c
    src/sdl/video/dummy/SDL_nullvideo.c
    src/sdl/video/SDL_blit.c
    src/sdl/video/SDL_yuv_sw.c
    src/sdl/video/SDL_blit_A.c
    src/sdl/video/SDL_cursor.c
    src/sdl/video/SDL_blit_N.c
    src/sdl/video/SDL_surface.c
    src/sdl/video/SDL_yuv.c
    src/sdl/SDL.c
    src/sdl/timer/SDL_timer.c
    src/sdl/timer/dummy/SDL_systimer.c
    src/sdl/timer/unix/SDL_systimer.c
    src/sdl/cdrom/linux/SDL_syscdrom.c
    src/sdl/cdrom/dummy/SDL_syscdrom.c
    src/sdl/cdrom/SDL_cdrom.c
    src/sdl/loadso/dummy/SDL_sysloadso.c
    src/sdl/loadso/dlopen/SDL_sysloadso.c
    src/sdl/joystick/linux/SDL_sysjoystick.c
    src/sdl/joystick/dummy/SDL_sysjoystick.c
    src/sdl/joystick/SDL_joystick.c
    src/sdl/stdlib/SDL_string.c
    src/sdl/stdlib/SDL_iconv.c
    src/sdl/stdlib/SDL_qsort.c
    src/sdl/stdlib/SDL_malloc.c
    src/sdl/stdlib/SDL_stdlib.c
    src/sdl/stdlib/SDL_getenv.c
    src/sdl/SDL_fatal.c
    src/core.cpp
    src/agent.cpp
)

target_link_libraries(Ladis PUBLIC Threads::Threads)
target_link_libraries(Ladis PUBLIC opencv_core opencv_highgui)

target_include_directories(Ladis PUBLIC "${CMAKE_SOURCE_DIR}/include/sdl" "${CMAKE_SOURCE_DIR}/include")

