#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "GameFile.hpp"
#include "Constants.hpp"
#include "PluginsManager.hpp"
#include "PluginInterface.hpp"
#include "AboutDialog.hpp"
#include <QNetworkReply>
#include <QLabel>
#include <QMenu>

#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentFile(NULL),
    m_pluginsManager(new PluginsManager(this)),
    m_aboutDialog(NULL)
{
    ui->setupUi(this);

    move(320, 240);
    setWindowIcon(QIcon(":/icon/Bomb64x64.png"));
    m_defaultWindowGeometry = this->saveGeometry();
    m_defaultWindowState = this->saveState();

    ui->documentList->addAction(ui->actionOpen);
    ui->documentList->addAction(ui->actionNew);
    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    ui->documentList->addAction(separator);
    ui->documentList->addAction(ui->actionSave);
    ui->documentList->addAction(ui->actionSaveAs);
    separator = new QAction(this);
    separator->setSeparator(true);
    ui->documentList->addAction(separator);
    ui->documentList->addAction(ui->actionClose);
    this->setWindowTitle(Constants::WIIKING2_TITLE);


    // Add default filter
    m_fileFilters << "All Files *.* (*.*)";

    // lets load the plugins
    m_pluginsManager->loadPlugins();

    // Setup the MRU list
    m_recentFileSeparator = ui->menuRecentFiles->insertSeparator(ui->actionClearRecent);
    for (int i = 0; i < MAXRECENT; ++i)
    {
        QAction* act = new QAction(this);
        act->setText(tr("&%1 %2").arg(i + 1).arg("test"));
        act->setVisible(false);
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_recentFileActions.append(act);
        ui->menuRecentFiles->insertAction(m_recentFileSeparator, m_recentFileActions[i]);
    }

#ifdef WK2_PREVIEW
    QWidget* bar = this->menuBar();
    QHBoxLayout* hbLay = new QHBoxLayout;
    hbLay->addStretch();
    QLabel* label  = new QLabel(bar);
    label->setText("PREVIEW BUILD");
    label->setMinimumSize(0, 12);
    hbLay->addWidget(label);
    bar->setLayout(hbLay);
#endif
    // Now load the MRU
    updateRecentFileActions();

    // Restore window from saved states
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());

    connect(&m_updateAccess, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    onCloseAll();
    delete ui;
}

void MainWindow::addFileFilter(const QString& filter)
{
    if (m_fileFilters.contains(filter))
        return;

    m_fileFilters << filter;
}

void MainWindow::removeFileFilter(const QString& filter)
{
    if (!m_fileFilters.contains(filter))
        return;

    m_fileFilters.removeOne(filter);
}

void MainWindow::closeFilesFromLoader(PluginInterface* loader)
{
    QList<GameFile*> targets;
    QList<QListWidgetItem*> items;
    foreach(GameFile* file, m_documents.values())
    {
        if (file->loadedBy() == loader)
        {
            targets.append(file);
            for (int i = 0; i < ui->documentList->count(); i++)
            {
                if (ui->documentList->item(i)->data(FILEPATH).toString() == file->filePath())
                    items.append(ui->documentList->item(i));
            }
        }
    }

    foreach(GameFile* file, targets)
    {
        m_documents.remove(file->filePath());
        delete file;
    }
    targets.clear();

    foreach (QListWidgetItem* item, items)
    {
        delete item;
    }
    items.clear();
}

void MainWindow::openFile(const QString& currentFile)
{
    if (m_documents.contains(currentFile))
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(Constants::WIIKING2_ALREADY_OPENED_NAG);
        msgBox.setText(Constants::WIIKING2_ALREADY_OPENED_NAG_MSG.arg(strippedName(currentFile)));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    PluginInterface* loader = m_pluginsManager->preferredPlugin(currentFile);
    if (!loader)
        return;

    GameFile* file = loader->loadFile(currentFile);

    // TODO: Show message upon failure
    if (!file)
        return;
    connect(file, SIGNAL(modified()), this, SLOT(updateWindowTitle()));
    m_documents[currentFile] = file;
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(FILENAME, m_documents[currentFile]->fileName());
    item->setData(FILEPATH, currentFile);
    item->setText(item->data(FILENAME).toString());
    if (!loader->icon().isNull())
        item->setIcon(loader->icon());

    ui->documentList->addItem(item);
    ui->documentList->setCurrentItem(item);
    m_currentFile = file;
    updateMRU(currentFile);
    QSettings settings;
    settings.setValue(Constants::Settings::WIIKING2_RECENT_DIRECTORY, QFileInfo(currentFile).absolutePath());
    updateWindowTitle();
}

void MainWindow::onDocumentChanged()
{
    if (!ui->documentList->currentItem())
    {
        m_currentFile = NULL;
        return;
    }

    // First we need to store the old Widget so we can remove
    // it from the main layout.
    GameFile* oldFile = m_currentFile;
    qDebug() << ui->documentList->currentItem()->data(FILEPATH).toString();
    m_currentFile = m_documents[ui->documentList->currentItem()->data(FILEPATH).toString()];

    if (!m_currentFile)
        return;

    if (oldFile)
    {
        if (oldFile->widget())
        {
            ui->mainLayout->removeWidget(oldFile->widget());
            oldFile->widget()->hide();
        }
    }

    if (m_currentFile->widget())
    {
        ui->mainLayout->addWidget(m_currentFile->widget());
        m_currentFile->widget()->show();
    }

    updateWindowTitle();
}


void MainWindow::onClose()
{
    if (m_documents.count() <= 0)
        return;

    qDebug() << "Closing file" << m_currentFile->filePath();
    m_documents.remove(m_currentFile->filePath());
    delete m_currentFile;
    m_currentFile = NULL;
    delete ui->documentList->currentItem();

    if (ui->documentList->count() <= 0)
        this->setWindowTitle(Constants::WIIKING2_TITLE);
}

void MainWindow::onCloseAll()
{
    while (m_documents.count() > 0)
        onClose();
}

void MainWindow::onOpen()
{
    QString MRD = mostRecentDirectory();
    QString file = QFileDialog::getOpenFileName(this, "Open file...", MRD, m_fileFilters.join(";;").trimmed());

    if (!file.isEmpty())
    {
        openFile(file);
    }
}

void MainWindow::onSave()
{

#ifdef WK2_PREVIEW
    QMessageBox mbox(this);
    mbox.setWindowTitle("Saving disabled");
    mbox.setText("<center>Saving has been disabled in this preview build.<br />"
                 "The application is far too unstable for everyday use.<br />"
                 "<br />"
                 "But your interest and support are much appreciated. <br />"
                 "<br />"
                 "<strong>Thank you for checking out this preview build.<strong></center>");
    mbox.setStandardButtons(QMessageBox::Ok);
    mbox.exec();
    return;
#endif

    if (m_documents.count() <= 0)
        return;
    if (!m_currentFile || !m_currentFile->isDirty())
        return;

    if (m_currentFile->filePath().isEmpty())
        onSaveAs();

    if (m_currentFile->save())
        statusBar()->showMessage(tr("Save successful"), 2000);
    else
        statusBar()->showMessage(tr("Save failed"), 2000);

    updateWindowTitle();
}

void MainWindow::onSaveAs()
{
#ifdef WK2_PREVIEW
    QMessageBox mbox;
    mbox.setWindowTitle("Saving disabled");
    mbox.setText("<center>Saving has been disabled in this preview build.<br />"
                 "The application is far too unstable for everyday use.<br />"
                 "<br />"
                 "But your interest and support are much appreciated. <br />"
                 "<br />"
                 "<strong>Thank you for checking out this preview build.<strong></center>");
    mbox.setStandardButtons(QMessageBox::Ok);
    mbox.exec();
    return;
#endif
    if (m_documents.count() <= 0)
        return;
    if (!m_currentFile)
        return;
    QString MRD = mostRecentDirectory();
    QString file = QFileDialog::getSaveFileName(this, "Save file as...", MRD, m_fileFilters.join(";;").trimmed());
    if (m_currentFile->filePath() != file)
    {
        m_documents.remove(m_currentFile->filePath());
        m_currentFile->save(file);
        m_documents[m_currentFile->filePath()] = m_currentFile;
        QListWidgetItem* item = ui->documentList->currentItem();
        item->setData(FILENAME, m_currentFile->fileName());
        item->setData(FILEPATH, m_currentFile->filePath());
        item->setText(item->data(FILENAME).toString());
    }
    else
    {
        m_currentFile->save();
    }

    updateWindowTitle();
}

void MainWindow::onExit()
{
    qApp->quit();
}

void MainWindow::onAbout()
{
    if (!m_aboutDialog)
        m_aboutDialog = new AboutDialog(this);

    m_aboutDialog->exec();
}

void MainWindow::onAboutQt()
{
    qApp->aboutQt();
}

void MainWindow::updateMRU(const QString& file)
{
    QSettings settings;
    QStringList files = settings.value(Constants::Settings::WIIKING2_RECENT_FILES).toStringList();
    files.removeAll(file);
    files.prepend(file);

    while (files.size() > MAXRECENT)
        files.removeLast();

    settings.setValue(Constants::Settings::WIIKING2_RECENT_FILES, files);
    updateRecentFileActions();

    foreach (QWidget* widget, QApplication::topLevelWidgets())
    {
        MainWindow* mainWin = qobject_cast<MainWindow*>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value(Constants::Settings::WIIKING2_RECENT_FILES).toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAXRECENT);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i+1).arg(strippedName(files[i]));
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
        m_recentFileActions[i]->setStatusTip(files[i]);
    }

    for (int j = numRecentFiles; j < MAXRECENT; ++j)
        m_recentFileActions[j]->setVisible(false);

    m_recentFileSeparator->setVisible(true);
}

void MainWindow::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if (action)
        openFile(action->data().toString());
}

void MainWindow::updateWindowTitle()
{
    if (m_currentFile->isDirty() && !ui->documentList->currentItem()->text().contains("*"))
        ui->documentList->currentItem()->setText(ui->documentList->currentItem()->text() + "*");
    else if (!m_currentFile->isDirty() && ui->documentList->currentItem()->text().contains("*"))
    {
        ui->documentList->currentItem()->setText(ui->documentList->currentItem()->data(FILENAME).toString());
    }

    setWindowTitle(Constants::WIIKING2_TITLE_FILE.arg(m_currentFile->fileName()).arg(m_currentFile->isDirty() ? Constants::WIIKING2_TITLE_DIRTY : ""));
}

void MainWindow::showEvent(QShowEvent* se)
{
    QMainWindow::showEvent(se);

    if (m_pluginsManager->plugins().count() <= 0)
    {
        QMessageBox mbox;
        mbox.setWindowTitle(Constants::WIIKING2_NO_PLUGINS_ERROR);
        mbox.setText(Constants::WIIKING2_NO_PLUGINS_ERROR_MSG);
        mbox.setStandardButtons(QMessageBox::Ok);
        mbox.exec();
        exit(1);
    }
}

QUrl MainWindow::redirectUrl(const QUrl& possibleRedirect, const QUrl& oldRedirect) const
{
    if (!possibleRedirect.isEmpty() && possibleRedirect != oldRedirect)
        return possibleRedirect;

    return QUrl();

}

void MainWindow::onPlugins()
{
    m_pluginsManager->dialog();
}

void MainWindow::onClearRecent()
{
    QSettings settings;
    settings.setValue(Constants::Settings::WIIKING2_RECENT_FILES, QVariant());

    updateRecentFileActions();
}

void MainWindow::onRestoreDefault()
{
    this->restoreState(m_defaultWindowState);
    this->restoreGeometry(m_defaultWindowGeometry);
}

void MainWindow::onCheckUpdate()
{
    m_updateAccess.get(QNetworkRequest(QUrl("http://update.wiiking2.com/wiiking2_editorv2/latest.update")));
}

void MainWindow::onNetworkFinished(QNetworkReply* nr)
{
    QMessageBox msgBox;
    QUrl possibleRedirect = nr->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    QUrl redirect = redirectUrl(possibleRedirect, nr->url());
    qDebug() << redirect;
    if (redirect.isEmpty() && nr->isReadable())
    {
        QStringList data = QString(nr->readAll()).split("\n");
        if (data[0].contains("version"))
        {
            QString serverVersion = data[0].split("=")[1];
            if (!QString::compare(serverVersion, Constants::WIIKING2_APP_VERSION, Qt::CaseInsensitive))
            {
                msgBox.setWindowTitle(Constants::WIIKING2_LATEST_VERSION);
                msgBox.setText(Constants::WIIKING2_LATEST_VERSION_MSG);
                msgBox.exec();
            }
            else
            {
                QString url;
                QString platform;
#ifdef QT_ARCH_X86_64
#ifdef Q_OS_LINUX
                platform = "unix-x64";
#elif Q_OS_WIN32
                platform = "win32";
#endif
#else
#ifdef Q_OS_LINUX
                platform = "unix-x86";
#elif Q_OS_WIN32
                platform = "win32";
#endif
#endif
                if (platform.isEmpty())
                    return;

                if (data.filter(platform).count() <= 0)
                    return;

                url = data.filter(platform)[0];
                if (url.contains("="))
                    url = url.split("=")[1];

                QString changeLog;
                if (data.filter("changelog").count() > 0)
                {
                    QString rawString = data.filter("changelog")[0];
                    if (rawString.contains("="))
                        changeLog = rawString.split("=")[1];
                }

                msgBox.setWindowTitle(Constants::WIIKING2_NOT_LATEST_VERSION.arg(Constants::WIIKING2_TITLE));
                msgBox.setText(Constants::WIIKING2_NOT_LATEST_VERSION_MSG.arg(Constants::WIIKING2_TITLE).arg(url).arg(changeLog));
                msgBox.exec();
            }
        }
        else
        {
            msgBox.setWindowTitle(Constants::WIIKING2_UPDATE_ERROR);
            msgBox.setText(Constants::WIIKING2_UPDATE_ERROR_MSG.arg(Constants::WIIKING2_TITLE));
            msgBox.exec();
        }
        qDebug() << data;
    }
    else
    {
        m_updateAccess.get(QNetworkRequest(redirect));
    }
}

QString MainWindow::strippedName(const QString& fullFileName) const
{
    return QFileInfo(fullFileName).fileName();
}

QString MainWindow::mostRecentDirectory()
{
    QSettings settings;
    return settings.value(Constants::Settings::WIIKING2_RECENT_DIRECTORY).toString();
}
