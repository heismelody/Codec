#ifndef FILELOADER_H
#define FILELOADER_H

#include<QMainWindow>

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QString;
class QMediaPlayer;
QT_END_NAMESPACE

class FileLoader : public QMainWindow
{
    Q_OBJECT
public:
    FileLoader();

private:
    bool loadFile(QString& fileName);

public slots:
    void sl_open_file();

signals:
    void sg_opened_mpg_file(QString& fileName);
};

#endif // FILELOADER_H
