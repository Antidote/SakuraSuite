#ifndef TESTPLUGIN_HPP
#define TESTPLUGIN_HPP

#include "skywardswordplugin_global.hpp"
#include <QObject>
#include <QIcon>
#include "PluginInterface.hpp"

class SettingsDialog;
class SettingsManager;

class SKYWARDSWORDPLUGGIN_EXPORT SkywardSwordPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    SkywardSwordPlugin();
    ~SkywardSwordPlugin();
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

    QDialog* settingsDialog();
    QObject* object();

    QIcon icon() const;

    static SkywardSwordPlugin* instance();
    static SettingsManager* settings();
signals:
    void enabledChanged();
public slots:

private:
    bool m_enabled;
    QString m_path;
    QIcon m_icon;
    SettingsDialog* m_settingsDialog;
    static SkywardSwordPlugin* m_instance;
};

#endif // TESTPLUGIN_HPP