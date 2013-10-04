#ifndef SPRITECANVAS_HPP
#define SPRITECANVAS_HPP

#include <QGLWidget>

class SpriteCanvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit SpriteCanvas(QWidget *parent = 0);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
signals:

public slots:
};

#endif // SPRITECANVAS_HPP
