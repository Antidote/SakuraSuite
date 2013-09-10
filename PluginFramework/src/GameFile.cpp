#include "GameFile.hpp"
#include <QTabWidget>
#include <QFileInfo>
#include <QDir>

GameFile::GameFile(const PluginInterface* loader, const QString &file)
    : m_loader((PluginInterface*)loader),
      m_dirty(false),
      m_widget(NULL)
{
    QFileInfo fInfo(file);
    m_file = fInfo.fileName();
    m_path = fInfo.absolutePath();
}

GameFile::~GameFile()
{
    delete m_widget;
    m_widget = NULL;
}


QWidget* GameFile::widget() const
{
    return (QWidget*)m_widget;
}

QString GameFile::fileName() const
{
    return m_file;
}

QString GameFile::fileDir() const
{
    return m_path;
}

QString GameFile::filePath() const
{
    return m_path + QDir::separator() + m_file;
}

PluginInterface* GameFile::loadedBy()
{
    return m_loader;
}

void GameFile::setDirty(bool dirty)
{
    m_dirty = dirty;
}

bool GameFile::isDirty()
{
    return m_dirty;
}

QString GameFile::game() const
{
    return "UNKNOWN";
}

bool GameFile::save(const QString &filename)
{
    Q_UNUSED(filename);
    return false;
}
