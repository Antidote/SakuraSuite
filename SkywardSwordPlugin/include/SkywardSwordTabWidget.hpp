#ifndef SKYWARDSWORDTABWIDGET_HPP
#define SKYWARDSWORDTABWIDGET_HPP

#include <QTabWidget>

namespace Ui {
class SkywardSwordTabWidget;
}

class SkywardSwordTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SkywardSwordTabWidget(QWidget *parent = 0);
    ~SkywardSwordTabWidget();

    QTabBar* tabBar() const;
private:
    Ui::SkywardSwordTabWidget *ui;
};

#endif // SKYWARDSWORDTABWIDGET_HPP
