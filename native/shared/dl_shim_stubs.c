// dl_shim_stubs.c — Generic no-op stubs for Windows/Steam APIs
//
// Terraria P/Invokes into kernel32.dll, user32.dll, shell32.dll, SteamAPI,
// and nfd (native file dialog). None of these exist on Switch.
// This file provides a single generic stub function that returns 0/NULL,
// and getsym resolvers that return it for every requested symbol.
//
// For Steam API: returning 0/false tells Steamworks.NET that Steam is not
// available, which is the correct behavior for a non-Steam (GOG) context.
//
// For Windows APIs: the functions called are typically GetTickCount,
// GetCurrentProcessId, MessageBox, etc. Returning 0 is safe for all of them.

#include "dl_shim_base.h"

// Generic stub — returns 0. Safe for int/ptr/bool returns.
// void-returning functions just ignore r0.
static long generic_stub() { return 0; }

// --- Steam API ---
// Steamworks.NET DllImports from "SteamAPI64" or "SteamAPI" depending on platform.
// We register both names. Every function returns 0 (failure/no-op).
void *getsym_SteamAPI64(const char *name)
{
    // Return the stub for every symbol. Steamworks.NET checks return values
    // and will fall back to "no Steam" mode when functions fail.
    return (void*)generic_stub;
}

void *getsym_SteamAPI(const char *name)
{
    return (void*)generic_stub;
}

// --- Windows kernel32.dll ---
// Terraria uses: GetTickCount, GetCurrentProcessId, GetCurrentThread,
// Sleep, GetEnvironmentVariable, SetEnvironmentVariable, etc.
void *getsym_kernel32(const char *name)
{
    // Special case: GetTickCount should return a monotonically increasing value
    // rather than 0, because Terraria may use it for timing.
    // For now, return 0 — we'll fix if it causes issues.
    return (void*)generic_stub;
}

// --- Windows user32.dll ---
// Terraria uses: MessageBox, GetSystemMetrics, etc.
void *getsym_user32(const char *name)
{
    return (void*)generic_stub;
}

// --- Windows shell32.dll ---
// Terraria uses: SHGetKnownFolderPath or similar for user paths.
void *getsym_shell32(const char *name)
{
    return (void*)generic_stub;
}

// Windows library names are case-sensitive in the shim registration, while
// managed callers use both `Kernel32.dll` and `kernel32.dll` spellings.
void *getsym_Kernel32(const char *name) { return getsym_kernel32(name); }
void *getsym_User32(const char *name) { return getsym_user32(name); }
void *getsym_Shell32(const char *name) { return getsym_shell32(name); }

void *getsym_Gdi32(const char *name) { return (void*)generic_stub; }
void *getsym_gdi32(const char *name) { return (void*)generic_stub; }
void *getsym_Ole32(const char *name) { return (void*)generic_stub; }
void *getsym_Uxtheme(const char *name) { return (void*)generic_stub; }
void *getsym_UxTheme(const char *name) { return (void*)generic_stub; }
void *getsym_Winmm(const char *name) { return (void*)generic_stub; }
void *getsym_Advapi32(const char *name) { return (void*)generic_stub; }
void *getsym_Comdlg32(const char *name) { return (void*)generic_stub; }

// --- Native File Dialog (nfd) ---
// Terraria uses NFD for file open/save dialogs. On Switch there's no dialog.
void *getsym_nfd(const char *name)
{
    // NFD_OKAY = 0, NFD_CANCEL = 1, NFD_ERROR = -1
    // Return 0 (NFD_OKAY) won't help — we want cancel or error.
    // But the stub returns 0 which is NFD_OKAY. Terraria will check
    // for a path string which will be NULL. This should be handled.
    return (void*)generic_stub;
}

// --- dav1dfile ---
// FNA references this for AV1 video decoding. Terraria likely doesn't use it.
void *getsym_dav1dfile(const char *name)
{
    return (void*)generic_stub;
}
