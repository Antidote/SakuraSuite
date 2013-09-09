#ifndef GAMEFILE_HPP
#define GAMEFILE_HPP

#include <QWidget>

class GameFile
{
public:
    GameFile(const QString& file = QString());
    virtual ~GameFile();

    QWidget* widget() const;

    /// \brief Name of file without path.
    QString fileName() const;
    /// \brief Absolute path to file, excluding filename.
    QString fileDir() const;
    /// \brief Absolute path to file, including filename.
    QString filePath() const;

    virtual bool isDirty();

    virtual QString game() const;

    virtual bool save(const QString& filename = QString());
protected:
    QString  m_file;
    QString  m_path;
    QWidget* m_widget;
};

#endif // GAMEFILE_HPP
