#include "include/SkywardSwordTabWidget.hpp"
#include "ui_SkywardSwordTabWidget.h"

SkywardSwordTabWidget::SkywardSwordTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::SkywardSwordTabWidget)
{
    ui->setupUi(this);
}

SkywardSwordTabWidget::~SkywardSwordTabWidget()
{
    delete ui;
}

QTabBar* SkywardSwordTabWidget::tabBar() const
{
    return QTabWidget::tabBar();
}
