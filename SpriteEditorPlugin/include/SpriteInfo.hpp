#ifndef SPRITEINFO_HPP
#define SPRITEINFO_HPP

#include <QDockWidget>

namespace Ui {
class SpriteInfo;
}

class SpriteInfo : public QDockWidget
{
    Q_OBJECT

public:
    explicit SpriteInfo(QWidget *parent = 0);
    ~SpriteInfo();

private:
    Ui::SpriteInfo *ui;
};

#endif // SPRITEINFO_HPP
