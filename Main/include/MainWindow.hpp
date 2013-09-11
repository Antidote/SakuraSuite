#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMap>
#include <QNetworkAccessManager>
#include <QUrl>

class QNetworkReply;
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
    void onNetworkFinished(QNetworkReply*);

    void updateMRU(const QString& file);
    void openRecentFile();

    void updateWindowTitle();
protected:
    void showEvent(QShowEvent *se);
private:
    QUrl redirectUrl(const QUrl& possibleRedirect, const QUrl& oldRedirect) const;
    void openFile(const QString& currentFile);
    QString strippedName(const QString& fullFileName) const;
    QString mostRecentDirectory();
    void updateRecentFileActions();

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

    QNetworkAccessManager    m_updateAccess;
};

#endif // MAINWINDOW_HPP
