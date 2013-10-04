#include "SpriteDocument.hpp"
#include "SpriteCanvas.hpp"

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
    m_widget = new SpriteCanvas();
}
