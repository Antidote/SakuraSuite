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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Constants.hpp"

#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QUrl>

#include <Updater.hpp>

class QLabel;
class QHBoxLayout;
class GameFile;
class PluginsManager;
class AboutDialog;

namespace Ui {
class MainWindow;
}

class PluginInterface;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum
    {
        FILENAME = Qt::UserRole + 1,
        FILEPATH = FILENAME + 1,
        MAXRECENT = 10
    };
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addFileFilter(const QString& filter);
    void removeFileFilter(const QString& filter);

    void closeFilesFromLoader(PluginInterface* loader);
    QString cleanPath(const QString& currentFile);

    bool isInternalBuild();
    bool isPreviewBuild();
protected slots:
    void onDocumentChanged();
    void onClose();
    void onCloseAll();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExit();
    void onAbout();
    void onAboutQt();
    void onPlugins();
    void onClearRecent();
    void onRestoreDefault();
    void onCheckUpdate();

    void onStyleChanged();
    void updateMRU(const QString& file);
    void openRecentFile();

    void updateWindowTitle();

    // Updater
    void onUpdateDone();
    void onUpdateError(Updater::ErrorType);
    void onUpdateWarning(QString message);
    void onNoUpdate();
protected:
    void showEvent(QShowEvent *se);
private:
    void openFile(const QString& currentFile);
    QString strippedName(const QString& fullFileName) const;
    QString mostRecentDirectory();
    void updateRecentFileActions();
    void setupStyleActions();

    Ui::MainWindow *ui;
    GameFile*                m_currentFile;

    QList<QAction*>          m_recentFileActions;
    QAction*                 m_recentFileSeparator;
    QMap<QString, GameFile*> m_documents;
    QStringList              m_fileFilters;
    PluginsManager*          m_pluginsManager;
    QByteArray               m_defaultWindowGeometry;
    QByteArray               m_defaultWindowState;
    AboutDialog*             m_aboutDialog;
    Updater*                 m_updater;
    QMessageBox              m_updateMBox;
#if defined(WK2_PREVIEW) || defined(WK2_INTERNAL)
    QHBoxLayout*             m_previewLayout;
    QLabel*                  m_previewLabel;
#endif
};

#endif // MAINWINDOW_HPP
