#include "GameDocument.hpp"


GameDocument::GameDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
}

QString GameDocument::game() const
{
    return "UNKNOWN";
}
