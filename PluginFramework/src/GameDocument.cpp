#include "GameDocument.hpp"


GameDocument::GameDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file),
      m_isWiiSave(false)
{
}

QString GameDocument::game() const
{
    return "UNKNOWN";
}

bool GameDocument::supportsWiiSave() const
{
    return false;
}

bool GameDocument::exportWiiSave() const
{
    return false;
}

bool GameDocument::isWiiSave() const
{
    return m_isWiiSave;
}
