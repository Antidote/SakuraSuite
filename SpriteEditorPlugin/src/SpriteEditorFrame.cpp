#include "SpriteEditorFrame.hpp"
#include "ui_SpriteEditorFrame.h"

SpriteEditorFrame::SpriteEditorFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SpriteEditorFrame)
{
    ui->setupUi(this);
}

SpriteEditorFrame::~SpriteEditorFrame()
{
    delete ui;
}
