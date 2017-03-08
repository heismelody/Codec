#include <QtUiTools>

#include "ma_jpeg_viewer.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    jpegViewer(new ma_jpeg_viewer),
    mpegDecoder(new ma_mpeg_decoder)
{
    ui->setupUi(this);

    QPushButton* ui_pushBtn = findChild<QPushButton*>("pushButton");
    QPushButton* mpgInBtn = findChild<QPushButton*>("pushButton_2");
    glWight = new GLTest( findChild<QOpenGLWidget*>("openGLWidget"));

    //connect
    connect(ui_pushBtn, SIGNAL (clicked()),jpegViewer, SLOT (sl_open_file()));
    connect(mpgInBtn, SIGNAL (clicked()),mpegDecoder, SLOT (sl_open_file()));
    connect(jpegViewer, SIGNAL (sg_display_file(uint8_t *)),
            glWight, SLOT (sl_display_image(uint8_t *)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
