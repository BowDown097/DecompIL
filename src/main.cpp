#include "mainwindow.h"
#include "decompilapplication.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setApplicationName(DECOMPIL_APP_NAME);
    QApplication::setApplicationVersion(DECOMPIL_VERSION_NAME);

#if QT_VERSION <= QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

#ifdef Q_OS_WIN
    QApplication::setStyle("fusion");
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=2");
#endif

    DecompILApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
