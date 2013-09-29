#include "SpriteDocument.hpp"
#include <QGLWidget>

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
    m_widget = new QGLWidget();
}
