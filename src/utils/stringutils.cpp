#include "stringutils.h"
#include <QTextStream>

namespace StringUtils
{
    // this appears to be faster than QRegularExpression
    void applyImplicitUsings(QString& str)
    {
        QTextStream stream(&str);

        static std::array<QString, 7> ImplicitUsings = {
            "System",
            "System.Collections.Generic",
            "System.IO",
            "System.Linq",
            "System.Net.Http",
            "System.Threading",
            "System.Threading.Tasks"
        };

        while (!stream.atEnd())
        {
            qsizetype pos = stream.pos();
            QString line = stream.readLine();

            if (!line.startsWith("using "))
                break;

            if (std::ranges::any_of(ImplicitUsings, [&line](const QString& u) { return line == "using " + u + ';'; }))
            {
                str.remove(pos, line.length() + 1);
                stream.seek(pos);
            }
        }

        trimFront(str);
    }

    void stripILWarnings(QString& str)
    {
        QTextStream stream(&str);

        while (!stream.atEnd())
        {
            qsizetype pos = stream.pos();
            QString line = stream.readLine();

            auto it = std::ranges::find_if_not(line, [](QChar c) { return c.isSpace(); });
            if (QStringView sv(it, line.cend()); sv.startsWith(QLatin1String("//IL_")))
            {
                str.remove(pos, line.length() + 1);
                stream.seek(pos);
            }
        }
    }

    void trimFront(QString& str)
    {
        auto it = std::ranges::find_if_not(str, [](QChar c) { return c.isSpace(); });
        str.remove(0, it - str.begin());
    }
}
