#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QtCore>
#include <QString>

class GameFile;

class PluginInterface
{
public:
    virtual ~PluginInterface(){}

    virtual QString filter()      const=0;
    virtual QString extension()   const=0;
    virtual QString name() const=0;
    virtual QString author() const=0;
    virtual QString version() const=0;
    virtual QString website() const=0;
    virtual void setPath(const QString& path)=0;
    virtual QString path() const=0;
    virtual QString license() const=0;
    virtual QString description() const=0;
    virtual bool enabled() const=0;
    virtual void setEnabled(const bool enable = true)=0;
    virtual bool canLoad(const QString& file)=0;

    virtual GameFile* loadFile(const QString& file) const=0;

    virtual QObject* object()=0;

    virtual QIcon icon() const=0;
protected:
    virtual void enabledChanged(){};
};

Q_DECLARE_INTERFACE(PluginInterface, "org.wiiking2.PluginInterface/1.0")
#endif // PLUGIN_HPP
