#pragma once
#include <QWidget>

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsForm(QWidget* parent = nullptr);
    ~SettingsForm();
protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
private:
    Ui::SettingsForm* ui;
private slots:
    void changeAppStyle();
    void changeEditorTabWidth(int width);
    void changeEditorTheme(int index);
    void enableSaveButton();
    void fillFromSettings();
    void promptFontChange();
    void revertChanges();
    void saveSettings();
};
