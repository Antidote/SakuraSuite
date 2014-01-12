#ifndef SPRITEFRAME_HPP
#define SPRITEFRAME_HPP

#include <QFrame>

namespace Ui {
class SpriteEditorFrame;
}

class SpriteEditorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SpriteEditorFrame(QWidget *parent = 0);
    ~SpriteEditorFrame();

private:
    Ui::SpriteEditorFrame *ui;
};

#endif // SPRITEFRAME_HPP
