#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QMap>

class GameFile;
class PluginsManager;
class AboutDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum {FILENAME, FILEPATH, MAXRECENT = 10};
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addFileFilter(const QString& filter);
    void removeFileFilter(const QString& filter);

protected slots:
    void onDocumentChanged();
    void onClose();
    void onOpen();
    void onAbout();
    void onAboutQt();
    void onPlugins();
    void onClearRecent();

    void updateMRU(const QString& file);
    void openRecentFile();
protected:
    void showEvent(QShowEvent *se);
private:
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
    AboutDialog*             m_aboutDialog;
};

#endif // MAINWINDOW_HPP
