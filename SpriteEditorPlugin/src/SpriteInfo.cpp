#include "SpriteInfo.hpp"
#include "ui_SpriteInfo.h"

SpriteInfo::SpriteInfo(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::SpriteInfo)
{
    ui->setupUi(this);
}

SpriteInfo::~SpriteInfo()
{
    delete ui;
}
