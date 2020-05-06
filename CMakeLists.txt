
cmake_minimum_required(VERSION 3.1)

project(Ladis)

find_package(Threads REQUIRED)

add_library(
    Ladis
    SHARED
    ./src/file/SDL_rwops.c
    ./src/audio/SDL_audiocvt.c
    ./src/audio/SDL_mixer_m68k.c
    ./src/audio/SDL_audiodev.c
    ./src/audio/SDL_mixer_MMX_VC.c
    ./src/audio/SDL_wave.c
    ./src/audio/dummy/SDL_dummyaudio.c
    ./src/audio/SDL_mixer.c
    ./src/audio/SDL_mixer_MMX.c
    ./src/audio/SDL_audio.c
    ./src/cpuinfo/SDL_cpuinfo.c
    ./src/main/dummy/SDL_dummy_main.c
    ./src/SDL_error.c
    ./src/thread/SDL_thread.c
    ./src/thread/pthread/SDL_sysmutex.c
    ./src/thread/pthread/SDL_syscond.c
    ./src/thread/pthread/SDL_systhread.c
    ./src/thread/pthread/SDL_syssem.c
    ./src/events/SDL_quit.c
    ./src/events/SDL_expose.c
    ./src/events/SDL_mouse.c
    ./src/events/SDL_resize.c
    ./src/events/SDL_events.c
    ./src/events/SDL_active.c
    ./src/events/SDL_keyboard.c
    ./src/video/SDL_pixels.c
    ./src/video/SDL_stretch.c
    ./src/video/SDL_bmp.c
    ./src/video/SDL_gamma.c
    ./src/video/SDL_RLEaccel.c
    ./src/video/SDL_video.c
    ./src/video/SDL_blit_0.c
    ./src/video/SDL_blit_1.c
    ./src/video/SDL_yuv_mmx.c
    ./src/video/dummy/SDL_nullevents.c
    ./src/video/dummy/SDL_nullmouse.c
    ./src/video/dummy/SDL_nullvideo.c
    ./src/video/SDL_blit.c
    ./src/video/SDL_yuv_sw.c
    ./src/video/SDL_blit_A.c
    ./src/video/SDL_cursor.c
    ./src/video/SDL_blit_N.c
    ./src/video/SDL_surface.c
    ./src/video/SDL_yuv.c
    ./src/SDL.c
    ./src/timer/SDL_timer.c
    ./src/timer/dummy/SDL_systimer.c
    ./src/timer/unix/SDL_systimer.c
    ./src/cdrom/linux/SDL_syscdrom.c
    ./src/cdrom/dummy/SDL_syscdrom.c
    ./src/cdrom/SDL_cdrom.c
    ./src/loadso/dummy/SDL_sysloadso.c
    ./src/loadso/dlopen/SDL_sysloadso.c
    ./src/joystick/linux/SDL_sysjoystick.c
    ./src/joystick/dummy/SDL_sysjoystick.c
    ./src/joystick/SDL_joystick.c
    ./src/stdlib/SDL_string.c
    ./src/stdlib/SDL_iconv.c
    ./src/stdlib/SDL_qsort.c
    ./src/stdlib/SDL_malloc.c
    ./src/stdlib/SDL_stdlib.c
    ./src/stdlib/SDL_getenv.c
    ./src/SDL_fatal.c
)

target_link_libraries(Ladis PUBLIC Threads::Threads)

target_include_directories(Ladis PUBLIC "${CMAKE_SOURCE_DIR}/include")

