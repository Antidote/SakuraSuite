#include "include/SpriteTree.hpp"
#include "ui_SpriteTree.h"

SpriteTree::SpriteTree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpriteTree)
{
    ui->setupUi(this);
}

SpriteTree::~SpriteTree()
{
    delete ui;
}
