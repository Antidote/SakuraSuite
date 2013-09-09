#include "GameFile.hpp"
#include <QTabWidget>
#include <QFileInfo>
#include <QDir>

GameFile::GameFile(const QString &file)
    : m_widget(NULL)
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

bool GameFile::isDirty()
{
    return true;
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
