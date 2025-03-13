#pragma once
#include "marshalnullable.h"
#include "marshalstring.h"

struct MarshalException
{
    MarshalString className;
    MarshalString message;
    MarshalString stackTrace;
    MarshalNullable<MarshalException> innerException;

    MarshalException(const MarshalException& other)
        : className(other.className),
          message(other.message),
          stackTrace(other.stackTrace),
          innerException(other.innerException) {}

    MarshalException(MarshalException&& other)
        : className(std::move(other.className)),
          message(std::move(other.message)),
          stackTrace(std::move(other.stackTrace)),
          innerException(std::move(other.innerException)) {}

    QString what() const
    {
        int size = className.size();
        if (!message.empty())
            size += 2 + message.size();
        if (innerException)
            size += 1 + innerException->message.size();
        if (!stackTrace.empty())
            size += 1 + stackTrace.size();

        QString result;
        result.reserve(size);

        result += className;
        if (!message.empty())
        {
            result += ": ";
            result += message;
        }
        if (innerException)
        {
            result += '\n';
            result += innerException->what();
        }
        if (!stackTrace.empty())
        {
            result += '\n';
            result += stackTrace;
        }

        return result;
    }
};
