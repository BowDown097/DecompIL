#include "libraryhandle.h"
#include <QLibrary>

LibraryHandle::LibraryHandle(const QString& name)
    : m_handle(new QLibrary(name))
{
    if (m_handle->load())
    {
        // try explicit relative path
        m_handle->setFileName("./" + name);
        if (m_handle->load())
            throw std::runtime_error(QStringLiteral("Error loading library: %1").arg(m_handle->errorString()).toStdString());
    }
}

LibraryHandle::~LibraryHandle()
{
    m_handle->unload();
    m_handle->deleteLater();
}

QFunctionPointer LibraryHandle::resolveFunction(const char* name) const
{
    return m_handle->resolve(name);
}
