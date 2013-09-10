#ifndef GAMEFILE_HPP
#define GAMEFILE_HPP

#include <QWidget>
class PluginInterface;

class GameFile : public QObject
{
    Q_OBJECT
public:
    GameFile(const PluginInterface* loader, const QString& file = QString());
    virtual ~GameFile();

    QWidget* widget() const;

    /// \brief Name of file without path.
    QString fileName() const;
    /// \brief Absolute path to file, excluding filename.
    QString fileDir() const;
    /// \brief Absolute path to file, including filename.
    QString filePath() const;

    PluginInterface* loadedBy();
    void setDirty(bool dirty);
    virtual bool isDirty();

    virtual QString game() const;

    virtual bool save(const QString& filename = QString());

signals:
    void modified();
    void enabledChanged();
protected:
    PluginInterface* m_loader;
    bool     m_dirty;
    QString  m_file;
    QString  m_path;
    QWidget* m_widget;
};

#endif // GAMEFILE_HPP
