// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "Constants.hpp"

#include <QFileSystemWatcher>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QUrl>
#include <QTimer>

#include <Updater.hpp>
#include <MainWindowBase.hpp>

class QLabel;
class QHBoxLayout;
class DocumentBase;
class PluginsManager;
class AboutDialog;
class PreferencesDialog;
class WiiKeyManager;

namespace Ui {
class MainWindow;
}

class PluginInterface;

class MainWindow : public QMainWindow, public MainWindowBase
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

    QMenu* fileMenu()        const;
    QMenu* newDocumentMenu() const;
    QMenu* editMenu()        const;
    QMenu* helpMenu()        const;
    QMenu* viewMenu()        const;
    QMenuBar* menuBar()      const;
    QStatusBar* statusBar()  const;
    QToolBar* toolBar()      const;
    QMainWindow* mainWindow()const;

public slots:
    void onNewDocument(DocumentBase* document);
protected slots:
    void onDocumentChanged(int row);
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
    void onReload();
    void onExportWiiSave();

    void onStyleChanged();

    void onFileChanged(const QString& file);
    void updateMRU(const QString& file);
    void openRecentFile();

    void updateWindowTitle();

    // Updater
    void onUpdateDone();
    void onUpdateError(Updater::ErrorType);
    void onUpdateWarning(QString message);
    void onNoUpdate();

    // Lock file
    void onLockTimeout();
protected:
    void showEvent(QShowEvent* se);
    void closeEvent(QCloseEvent* e);
    void dragEnterEvent(QDragEnterEvent* e);
    void dropEvent(QDropEvent* e);
private:
    void loadWiiKeys();
    bool checkLock();
    void initUpdater();
    void restoreDefaultGeometry();
    void injectPreviewLabel();
    void initDocumentList();
    void initMRU();
    void initFSWatcher();
    void openFile(const QString& currentFile);
    QString strippedName(const QString& fullFileName) const;
    QString mostRecentDirectory();
    void updateRecentFileActions();
    void setupStyleActions();

    Ui::MainWindow *ui;
    DocumentBase*                m_currentFile;

    QFileSystemWatcher       m_fileSystemWatcher;
    QList<QAction*>          m_recentFileActions;
    QAction*                 m_recentFileSeparator;
    QMap<QString, DocumentBase*> m_documents;
    QStringList              m_fileFilters;
    PluginsManager*          m_pluginsManager;
    QByteArray               m_defaultWindowGeometry;
    QByteArray               m_defaultWindowState;
    AboutDialog*             m_aboutDialog;
    Updater*                 m_updater;
    PreferencesDialog*       m_preferencesDialog;
    QMessageBox              m_updateMBox;
    QTimer                   m_lockTimer;
    bool                     m_cancelClose;
    WiiKeyManager*           m_keyManager;
    int                      m_untitledDocs;
#if defined(SS_PREVIEW) || defined(SS_INTERNAL)
    QHBoxLayout*             m_previewLayout;
    QLabel*                  m_previewLabel;
#endif
};

#endif // MAINWINDOW_HPP
