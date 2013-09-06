#ifndef GAMEFILE_HPP
#define GAMEFILE_HPP

#include <QWidget>

class GameFile : public QObject
{
    Q_OBJECT
public:
    GameFile(const QString& file);
    ~GameFile();

    QWidget* widget();

    /// \brief Name of file without path.
    QString fileName() const;
    /// \brief Absolute path to file, excluding filename.
    QString fileDir() const;
    /// \brief Absolute path to file, including filename.
    QString filePath() const;

    virtual bool isDirty();

protected:
    QString m_file;
    QString m_path;
    QWidget* m_widget;
};

#endif // GAMEFILE_HPP
