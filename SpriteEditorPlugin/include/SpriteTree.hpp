#ifndef SPRITETREE_HPP
#define SPRITETREE_HPP

#include <QWidget>

namespace Ui {
class SpriteTree;
}

class SpriteTree : public QWidget
{
    Q_OBJECT

public:
    explicit SpriteTree(QWidget *parent = 0);
    ~SpriteTree();

private:
    Ui::SpriteTree *ui;
};

#endif // SPRITETREE_HPP
