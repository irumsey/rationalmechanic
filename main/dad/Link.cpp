///
///	lucid libraries
///

#if defined(CONFIG)
#	undef CONFIG
#endif

#if defined(_DEBUG)
#	define CONFIG "d"
#else
#	define CONFIG "r"
#endif

#if defined(PLATFORM)
#	undef PLATFORM
#endif

#if defined(_WIN64)
#	define PLATFORM "64"
#else
#	define PLATFORM "32"
#endif

#pragma comment (lib,      "core." PLATFORM CONFIG ".lib")
#pragma comment (lib,  "core.win." PLATFORM CONFIG ".lib")
#pragma comment (lib, "gal.d3d11." PLATFORM CONFIG ".lib")
#pragma comment (lib,      "gigl." PLATFORM CONFIG ".lib")
#pragma comment (lib,     "orbit." PLATFORM CONFIG ".lib")
