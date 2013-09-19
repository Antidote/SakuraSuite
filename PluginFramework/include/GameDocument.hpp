#ifndef GAMEDOCUMENT_HPP
#define GAMEDOCUMENT_HPP

#include "DocumentBase.hpp"

class GameDocument : public DocumentBase
{
    Q_OBJECT
public:
    GameDocument(const PluginInterface* loader, const QString& file = QString());
    virtual QString game() const;
};

#endif // GAMEDOCUMENT_HPP
