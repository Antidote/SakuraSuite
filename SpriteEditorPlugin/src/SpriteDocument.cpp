#include "SpriteDocument.hpp"
#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "SpriteTree.hpp"
#include <MainWindowBase.hpp>
#include <QMainWindow>

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
    m_widget = new SpriteEditorFrame();
}
