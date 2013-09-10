#ifndef SETTINGSDIALOG_HPP
#define SETTINGSDIALOG_HPP

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

public slots:
    void accept();
protected:
    void showEvent(QShowEvent *se);
private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_HPP
