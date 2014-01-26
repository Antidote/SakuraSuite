#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class WiiKeyManager;

class QAbstractButton;
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
    void onDirButtonClicked();
    void onButtonClicked(QAbstractButton* button);
    void onLoadKeys();
    void onLoadMac();

private:
    void saveSettings();
    void updateKeys();
    Ui::PreferencesDialog *ui;
    QString    m_currentStyle;
    bool       m_currentChanged;
    QString    m_defaultStyle;
    bool       m_defaultChanged;
    bool       m_singleInstance;
    WiiKeyManager* m_keyManager;
};

#endif // PREFERENCESDIALOG_HPP
