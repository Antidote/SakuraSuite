#include "SpriteCanvas.hpp"

SpriteCanvas::SpriteCanvas(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setAutoFillBackground(false);
}

void SpriteCanvas::initializeGL()
{
    qglClearColor(QColor().magenta());
    glEnable(GL_TEXTURE_2D);
}

void SpriteCanvas::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glVertex2f(16, 0);
    glColor3b(255, 255, 255);
    glVertex2f(32, 16);
    glColor3b(255, 255, 255);
    glVertex2f(32, 0);
    glColor3b(255, 255, 255);
    glVertex2f(32, 32);
    glColor3b(255, 255, 255);
    glEnd();
}

void SpriteCanvas::resizeGL(int w, int h)
{
    int side = qMin(w, h);
    glViewport((w-side) / 2, (h-side)/2 , side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5f, +0.5f, -0.5f, +0.5f, 4.0f, 15.0);
}
