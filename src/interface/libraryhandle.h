#pragma once

class LibraryHandle
{
public:
    explicit LibraryHandle(const char* name);
    ~LibraryHandle();

    void* resolveFunction(const char* name) const;
private:
    void* m_handle{};
};
