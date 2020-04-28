#pragma once

// Core defines

// Configuration defines
#if   defined (__DEBUG)
#define NEON_DEBUG
#elif defined(__RELEASE)
#define NEON_RELEASE
#endif

// Aditional debug features in release
#define ReleaseDebuggingEanabled

#if defined(NEON_DEBUG) || defined(ReleaseDebuggingEanabled)
#define NEON_CHECK(x, y) if(x) { printf(y); }
#elif
#define NEON_CHECK(x, y) (x, y)
#endif

// Platform defines
#define NEON_PLATFORM_WINDOWS
