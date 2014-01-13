#ifndef PLUGINSETTINGSDIALOG_HPP
#define PLUGINSETTINGSDIALOG_HPP

#include <QDialog>

class PluginSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PluginSettingsDialog(QWidget* parent=0);
    virtual QWidget* centralWidget() const=0;
    virtual void loadSettings()=0;
public slots:
    virtual void restoreOwnership()=0;
};

#endif // PLUGINSETTINGSDIALOG_HPP
