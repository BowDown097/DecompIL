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

    template<typename T>
    inline void enableSaveFor(auto signal)
    {
        const QList<T*> children = findChildren<T*>();
        for (T* child : children)
            connect(child, signal, this, &SettingsForm::enableSaveButton);
    }
private slots:
    void changeAppStyle();
    void changeEditorTabWidth(int width);
    void changeEditorTheme(int index);
    void enableSaveButton();
    void fillFromSettings();
    void promptFontChange();
    void revertLiveChanges();
    void saveSettings();
    void toggleEditorScrollBarH(bool on);
    void toggleEditorScrollBarV(bool on);
    void toggleEditorSidebar(bool on);
};
