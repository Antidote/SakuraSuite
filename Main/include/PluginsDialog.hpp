#ifndef PLUGINSDIALOG_HPP
#define PLUGINSDIALOG_HPP

#include <QDialog>

namespace Ui {
class PluginsDialog;
}

class PluginsManager;
class QTreeWidgetItem;

class PluginsDialog : public QDialog
{
    Q_OBJECT

public:
    enum
    {
        NameColumn,
        VersionColumn,
        ExtensionColumn,
        EnabledColumn,
        ColumnCount
    };
    explicit PluginsDialog(QWidget *parent, PluginsManager* pluginsManager);
    ~PluginsDialog();

protected:
    void showEvent(QShowEvent *se);

private slots:
    void onItemSelectionChanged();
    void onItemClicked(QTreeWidgetItem* item, int column);
private:
    Ui::PluginsDialog *ui;

    PluginsManager* m_pluginsManager;
};

#endif // PLUGINSDIALOG_HPP
