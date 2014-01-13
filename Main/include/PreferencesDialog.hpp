#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class WiiKeyManager;

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(WiiKeyManager* keyManager, QWidget *parent = 0);
    ~PreferencesDialog();

protected:
    void showEvent(QShowEvent* se);

protected slots:
    void accept();
    void reject();
private slots:
    void onCurrentIndexChanged(QString style);
    void onTextChanged(QString text);
    void onSingleInstanceToggled(bool checked);
    void onLoadKeys();
    void onLoadMac();

private:
    void updateKeys();
    void removePluginTabs();
    Ui::PreferencesDialog *ui;
    QString    m_currentStyle;
    bool       m_currentChanged;
    QString    m_defaultStyle;
    bool       m_defaultChanged;
    bool       m_singleInstance;
    WiiKeyManager* m_keyManager;
};

#endif // PREFERENCESDIALOG_HPP
