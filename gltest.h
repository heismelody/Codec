#ifndef GLTEST_H
#define GLTEST_H

#include <QtWidgets>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLTest : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLTest(QWidget *parent);
    GLTest(QOpenGLWidget * ogl);
    //~GLTest();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
    //void setClearColor(const QColor &color);

signals:
    void clicked();
public slots:
    void sl_display_image(uint8_t * image);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:

    QColor clearColor;
    QPoint lastPos;
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
    uint8_t * data;
};

#endif // GLTEST_H
