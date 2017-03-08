#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ma_jpeg_viewer.h"
#include "ma_mpeg_decoder.h"
#include "gltest.h"

namespace Ui {
class MainWindow;
}

class GLTest;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    ma_jpeg_viewer *jpegViewer;
    Ui::MainWindow *ui;
    ma_mpeg_decoder* mpegDecoder;
    GLTest* glWight;

};

#endif // MAINWINDOW_H
