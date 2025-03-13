#include "decompilapplication.h"
#include "utils/uiutils.h"
#include <QStyle>

DecompILApplication::DecompILApplication(int& argc, char** argv)
    : QApplication(argc, argv)
{
    m_settings.initialize();
    UIUtils::g_defaultStyle = style()->objectName();
    UIUtils::setAppStyle(m_settings.appStyle, m_settings.darkTheme);
}
