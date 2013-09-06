#include "GameFile.hpp"
#include <QTabWidget>
#include <QFileInfo>
#include <QDir>

GameFile::GameFile(const QString &file)
    : m_widget(NULL)
{
    m_widget = new QTabWidget;
    QTabWidget* tab = (QTabWidget*)m_widget;
    tab->addTab(new QWidget(m_widget), "Test1");
    tab->addTab(new QWidget(m_widget), "Test2");
    tab->addTab(new QWidget(m_widget), "Test3");

    QFileInfo fInfo(file);
    m_file = fInfo.fileName();
    m_path = fInfo.absolutePath();
}

GameFile::~GameFile()
{
    delete m_widget;
    m_widget = NULL;
}


QWidget* GameFile::widget()
{
    return m_widget;
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
