// This file is part of WiiKing2 Editor.
//
// WiiKing2 Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Wiiking2 Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WiiKing2 Editor.  If not, see <http://www.gnu.org/licenses/>

#ifndef PLUGINSDIALOG_HPP
#define PLUGINSDIALOG_HPP

#include <QDialog>
#include <QTimer>

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

    void updatePluginData();
protected:
    void showEvent(QShowEvent *se);

private slots:
    void onItemSelectionChanged();
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onSettingsClicked();
    void onReloadPlugin();
    void onCheckUpdate();
    void onPluginWarning(QString warning);
    void onTimeOut();
private:
    Ui::PluginsDialog *ui;

    PluginsManager* m_pluginsManager;
    QTimer          m_statusTimer;
};

#endif // PLUGINSDIALOG_HPP
