#pragma once
#include <QFunctionPointer>
#include <QString>

class LibraryHandle
{
public:
    explicit LibraryHandle(const QString& name);
    ~LibraryHandle();
    QFunctionPointer resolveFunction(const char* name) const;
private:
    class QLibrary* m_handle;
};
