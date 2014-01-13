#include "SpriteDocument.hpp"
#include "SpriteEditorFrame.hpp"
#include "SpriteEditorPlugin.hpp"
#include "SpriteTree.hpp"
#include "SSpriteFileReader.hpp"
#include "SSpriteFile.hpp"
#include <MainWindowBase.hpp>
#include <QMainWindow>
#include <Exception.hpp>

SpriteDocument::SpriteDocument(const PluginInterface* loader, const QString& file)
    : DocumentBase(loader, file)
{
    m_widget = new SpriteEditorFrame();

    try
    {
    }
    catch(zelda::error::Exception e)
    {
    }
}
