#ifndef CINCLUDE_H
#define CINCLUDE_H


#ifdef _WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

    //define something for Windows (32-bit and 64-bit, this part is common)
    #ifdef _WIN64
        //define something for Windows (64-bit only)
        #define M_WIN64
    #else
        //define something for Windows (32-bit only)
        #define M_WIN32
    #endif
#elif defined __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif defined __ANDROID__
    // android
#elif defined __linux__
    // linux
    #define M_LINUX
#elif defined __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

#ifdef __GNUC__
#ifdef M_GNU
#undef M_GNU
#endif
#define M_GNU
#elif defined _MSC_VER
#ifdef M_MSVC
#undef M_MSVC
#endif
#define M_MSVC
#endif

#ifdef ALGDLL // ALGDLL
#ifdef ALGEXPORT
#define ALGAPI      __declspec(dllexport)
#else
#define ALGAPI      __declspec(dllimport)
#endif // ALGEXPORT
#else
#define ALGAPI
#endif // ALGDLL


#endif // CINCLUDE_H
