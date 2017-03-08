#include "gltest.h"

GLTest::GLTest(QWidget *parent)
    : QOpenGLWidget(parent),
      clearColor(Qt::black)
{
}

GLTest::GLTest(QOpenGLWidget * ogl)
    : QOpenGLWidget(ogl)
{

}

QSize GLTest::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLTest::sizeHint() const
{
    return QSize(200, 200);
}

const unsigned int W = 250;
const unsigned int H = 250;

void GLTest::initializeGL()
{
    initializeOpenGLFunctions();
}

void GLTest::paintGL()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_BYTE, data );
}

void GLTest::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLTest::sl_display_image(uint8_t * image)
{

    data = image;
    update();
}
