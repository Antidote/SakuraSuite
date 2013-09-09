#ifndef OOTSAVEPLUGIN_HPP
#define OOTSAVEPLUGIN_HPP

#include "ootsaveplugin_global.hpp"
#include <QObject>
#include <QIcon>
#include "PluginInterface.hpp"

class OOTSAVEPLUGINSHARED_EXPORT OoTSavePlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    OoTSavePlugin();
    ~OoTSavePlugin();

    QString filter()      const;
    QString extension()   const;
    QString name()        const;
    QString author()      const;
    QString version()     const;
    QString website()     const;
    void setPath(const QString &path);
    QString path()        const;
    QString license()     const;
    QString description() const;
    bool enabled() const;
    void setEnabled(const bool enable);

    GameFile* loadFile(const QString& file) const;
    bool canLoad(const QString& filename);

    QObject* object();

    QIcon icon() const;
signals:
    void enabledChanged();
public slots:

private:
    bool m_enabled;
    QString m_path;
    QIcon m_icon;
};

#endif // OOTSAVEPLUGIN_HPP
