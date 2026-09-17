#include "core.h"
#include "dl_shim.h"

#define REGISTER_LIBRARY(name, string, id) \
	const intptr_t LibHandle_##name = (intptr_t)(0xABC00000 | id); \
	const char* LibName_##name = string; \
	void* getsym_##name(const char *name);

#define CHECK_LIB_NAME(value, name) \
do { \
	if (strcmp(value, LibName_##name) == 0) {\
		return (void *)LibHandle_##name; \
	}\
} while (0)

// Core libraries
REGISTER_LIBRARY(Libnx, "libnx", 0x01)
REGISTER_LIBRARY(Internal, "__Internal", 0x02)
REGISTER_LIBRARY(SystemNative, "libSystem.Native", 0x03)
REGISTER_LIBRARY(GlobalizationNative, "libSystem.Globalization.Native", 0x04)
REGISTER_LIBRARY(CompressionNative, "libSystem.IO.Compression.Native", 0x05)

// Optional libraries.
// Don't forget to also register them in the LoadLibrary and GetSymbol functions below, otherwise they won't be found.
#if defined(DLSHIM_SDL2)
REGISTER_LIBRARY(SDL2, "SDL2", 0x100)
#endif
#if defined(DLSHIM_SDL3)
REGISTER_LIBRARY(SDL3, "SDL3", 0x106)
#endif

#if defined(DLSHIM_SDL2_IMAGE)
REGISTER_LIBRARY(SDL2_image, "SDL2_image", 0x101)
#endif

#if defined(DLSHIM_CIMGUI)
REGISTER_LIBRARY(Cimgui, "cimgui", 0x102)
#endif

#if defined(DLSHIM_OPENGL)
REGISTER_LIBRARY(Egl, "libEGL.dll", 0x103)
REGISTER_LIBRARY(Glad, "glad", 0x104)
#endif

#if defined(DLSHIM_OPENAL)
REGISTER_LIBRARY(OpenAl, "openal32.dll", 0x105)
#endif

// FNA native libraries
#if defined(DLSHIM_FNA3D)
REGISTER_LIBRARY(FNA3D, "FNA3D", 0x110)
#endif

#if defined(DLSHIM_FNA)
REGISTER_LIBRARY(FNAudio, "FAudio", 0x111)

// Stub libraries — return 0 for all symbols
#if defined(DLSHIM_STUBS)
REGISTER_LIBRARY(SteamAPI64, "SteamAPI64", 0x120)
REGISTER_LIBRARY(SteamAPI, "SteamAPI", 0x121)
REGISTER_LIBRARY(Kernel32, "Kernel32.dll", 0x122)
REGISTER_LIBRARY(kernel32, "kernel32.dll", 0x123)
REGISTER_LIBRARY(User32, "User32", 0x124)
REGISTER_LIBRARY(user32, "user32.dll", 0x125)
REGISTER_LIBRARY(Shell32, "shell32.dll", 0x126)
REGISTER_LIBRARY(nfd, "nfd", 0x127)
REGISTER_LIBRARY(dav1dfile, "dav1dfile", 0x128)
#endif
#endif

void *dlshim_loadLibrary(const char *name, int flags, char **err, void *user_data)
{
    if (!name)
        return (void *)LibHandle_Internal;

    CHECK_LIB_NAME(name, Libnx);
    CHECK_LIB_NAME(name, Internal);
    CHECK_LIB_NAME(name, SystemNative);
    CHECK_LIB_NAME(name, GlobalizationNative);
    CHECK_LIB_NAME(name, CompressionNative);

    #if defined(DLSHIM_SDL2)
	CHECK_LIB_NAME(name, SDL2);
	#endif
	#if defined(DLSHIM_SDL3)
	CHECK_LIB_NAME(name, SDL3);
	#endif

	#if defined(DLSHIM_SDL2_IMAGE)
	CHECK_LIB_NAME(name, SDL2_image);
	#endif

	#if defined(DLSHIM_CIMGUI)
	CHECK_LIB_NAME(name, Cimgui);
	#endif	

	#if defined(DLSHIM_OPENGL)
	CHECK_LIB_NAME(name, Egl);
	CHECK_LIB_NAME(name, Glad);
	#endif
	
	#if defined(DLSHIM_OPENAL)
	CHECK_LIB_NAME(name, OpenAl);
	#endif	

	#if defined(DLSHIM_FNA3D)
	CHECK_LIB_NAME(name, FNA3D);
	#endif

	#if defined(DLSHIM_FNA)
	CHECK_LIB_NAME(name, FNAudio);
	#endif

	#if defined(DLSHIM_STUBS)
	CHECK_LIB_NAME(name, SteamAPI64);
	CHECK_LIB_NAME(name, SteamAPI);
	CHECK_LIB_NAME(name, Kernel32);
	CHECK_LIB_NAME(name, kernel32);
	CHECK_LIB_NAME(name, User32);
	CHECK_LIB_NAME(name, user32);
	CHECK_LIB_NAME(name, Shell32);
	CHECK_LIB_NAME(name, nfd);
	CHECK_LIB_NAME(name, dav1dfile);
	#endif

	if (g_config.mononx_logging)
    	io_debugf("dlshim_loadLibrary %s library=%s", "unknown library", name);

    return NULL;
}

void *dlshim_closeLibrary(void *handle, void *user_data)
{
    return NULL;
}

void *dlshim_getSymbol(void *handle, const char *name, char **err, void *user_data)
{
    void *symbol = NULL;
	const char* resolvedLibrary = "<none>";

#define CHECK_LIB_SYMBOL(libName) \
	case LibHandle_##libName: \
		resolvedLibrary = LibName_##libName; \
		symbol = getsym_##libName(name);  \
		break;

	if (!handle)
		return NULL;

	switch ((intptr_t)handle)
	{		
		CHECK_LIB_SYMBOL(Libnx)
		CHECK_LIB_SYMBOL(Internal)
		CHECK_LIB_SYMBOL(SystemNative)
		CHECK_LIB_SYMBOL(GlobalizationNative)
		CHECK_LIB_SYMBOL(CompressionNative)

	#if defined(DLSHIM_SDL2)
		CHECK_LIB_SYMBOL(SDL2)
	#endif
	#if defined(DLSHIM_SDL3)
		CHECK_LIB_SYMBOL(SDL3)
	#endif

	#if defined(DLSHIM_SDL2_IMAGE)
		CHECK_LIB_SYMBOL(SDL2_image)
	#endif

	#if defined(DLSHIM_CIMGUI)
		CHECK_LIB_SYMBOL(Cimgui)
	#endif

	#if defined(DLSHIM_OPENGL)
		CHECK_LIB_SYMBOL(Egl)
		CHECK_LIB_SYMBOL(Glad)
	#endif

	#if defined(DLSHIM_OPENAL)
		CHECK_LIB_SYMBOL(OpenAl)
	#endif

	#if defined(DLSHIM_FNA3D)
	CHECK_LIB_SYMBOL(FNA3D)
	#endif

	#if defined(DLSHIM_FNA)
	CHECK_LIB_SYMBOL(FNAudio)
	#endif

	#if defined(DLSHIM_STUBS)
	CHECK_LIB_SYMBOL(SteamAPI64)
	CHECK_LIB_SYMBOL(SteamAPI)
	CHECK_LIB_SYMBOL(Kernel32)
	CHECK_LIB_SYMBOL(kernel32)
	CHECK_LIB_SYMBOL(User32)
	CHECK_LIB_SYMBOL(user32)
	CHECK_LIB_SYMBOL(Shell32)
	CHECK_LIB_SYMBOL(nfd)
	CHECK_LIB_SYMBOL(dav1dfile)
	#endif
	}

    if (symbol) {
		if (g_config.mononx_logging)
        	io_debugf("dlshim_getSymbol resolved: handle=%p lib=%s symbol=%s", handle, resolvedLibrary, name);

        return symbol;
	}

    if (g_config.mononx_logging)
        io_debugf("dlshim_getSymbol error: handle=%p lib=%s symbol=%s", handle, resolvedLibrary, name);

    return NULL;
}

void* getsym_Internal(const char *name)
{
	if (strcmp(name, "console_ensure_init") == 0) return (void *)console_ensure_init;
    else if (strcmp(name, "console_dispose") == 0) return (void *)console_dispose;
    else if (strcmp(name, "console_update") == 0) return(void *)console_update;

	return NULL;
}