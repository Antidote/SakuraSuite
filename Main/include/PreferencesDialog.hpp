#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

protected:
    void showEvent(QShowEvent* se);

protected slots:
    void accept();
    void reject();
private slots:
    void onCurrentIndexChanged(QString style);
    void onTextChanged(QString text);
private:
    Ui::PreferencesDialog *ui;
    QString    m_currentStyle;
    bool       m_currentChanged;
    QString    m_defaultStyle;
    bool       m_defaultChanged;
};

#endif // PREFERENCESDIALOG_HPP
