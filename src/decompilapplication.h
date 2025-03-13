#pragma once
#include "settingsstore.h"
#include <QApplication>

#define decompILApp (static_cast<DecompILApplication*>(QCoreApplication::instance()))

class DecompILApplication final : public QApplication
{
public:
    DecompILApplication(int& argc, char** argv);
    SettingsStore& settings() { return m_settings; }
private:
    SettingsStore m_settings;
};
