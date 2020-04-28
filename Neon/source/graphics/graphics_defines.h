#pragma once
#include "./core/core_defines.h"

// TODO:: create a seperate macros for storing extra debug info, like resource names as strings

// API defines
#define __NEON_OPENGL	 0x00
#define __NEON_DIRECTX11 0x01
#define __NEON_DIRECTX12 0x02
#define __NEON_VULKAN	 0x03

// Graphics defines
//#define NEON_COMPILE_OPENGL
//#define NEON_COMPILE_DIRECTX11
#define NEON_COMPILE_DIRECTX12
#define NEON_COMPILE_VULKAN

// Default graphics API's per platform
#define NEON_PLATFORM_WINDOWS_DEFAULT_API __NEON_DIRECTX12

// Windows platform
#if defined(NEON_PLATFORM_WINDOWS) && defined(NEON_COMPILE_OPENGL)
#define NEON_SUPPORT_OPENGL
#endif
#if defined(NEON_PLATFORM_WINDOWS) && defined(NEON_COMPILE_DIRECTX11)
#define NEON_SUPPORT_DIRECTX11
#endif
#if defined(NEON_PLATFORM_WINDOWS) && defined(NEON_COMPILE_DIRECTX12)
#define NEON_SUPPORT_DIRECTX12
#endif
#if defined(NEON_PLATFORM_WINDOWS) && defined(NEON_COMPILE_VULKAN)
#define NEON_SUPPORT_VULKAN
#endif