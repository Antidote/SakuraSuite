#ifndef SPRITEDOCUMENT_HPP
#define SPRITEDOCUMENT_HPP

#include <DocumentBase.hpp>

class SpriteTree;
class SSpriteFile;
class SpriteDocument : public DocumentBase
{
    Q_OBJECT
public:
    SpriteDocument(const PluginInterface* loader, const QString& file = QString());

private:
    SSpriteFile* m_spriteFile;
};

#endif // SPRITEDOCUMENT_HPP
