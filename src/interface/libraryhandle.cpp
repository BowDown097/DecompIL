#include "libraryhandle.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <system_error>
#include <Windows.h>
#elif defined(__unix__)
#include <dlfcn.h>
#include <system_error>
#else
#include <stdexcept>
#endif

LibraryHandle::LibraryHandle(const char* name)
{
#ifdef WIN32
    m_handle = LoadLibraryA(name);
    if (!m_handle)
        throw std::system_error(GetLastError(), std::system_category(), "Error loading interface library");
#elif defined(__unix__)
    m_handle = dlopen(name, RTLD_LAZY);
    if (!m_handle)
        throw std::system_error(errno, std::system_category(), "Error loading interface library");
#else
    throw std::runtime_error("Error loading interface library: Unsupported platform");
#endif
}

LibraryHandle::~LibraryHandle()
{
#ifdef WIN32
    FreeLibrary(m_handle);
#elif defined(__unix__)
    dlclose(m_handle);
#endif
}

void* LibraryHandle::resolveFunction(const char* name) const
{
#ifdef WIN32
    return GetProcAddress(m_handle, name);
#elif defined(__unix__)
    return dlsym(m_handle, name);
#endif
}
