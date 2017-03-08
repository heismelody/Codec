#ifndef MA_JPEG_VIEWER_H
#define MA_JPEG_VIEWER_H

#include <QMainWindow>

#include "ma_jpeg_codec.h"

QT_BEGIN_NAMESPACE
class QFile;
class QFileDialog;
class QString;
QT_END_NAMESPACE

class ma_jpeg_viewer : public QMainWindow
{
    Q_OBJECT
public:
    ma_jpeg_viewer();
    bool loadFile(const QString &fileName);
    ma_jpeg_codec *codec;
private:
    QByteArray *imageFile;

public:
    //setter
    void setImageFile(QByteArray *imageFile);

public slots:
    void sl_open_file();
    void sl_decode_complete(uint8_t *data);

signals:
    void sg_out_file(QByteArray *imageFile);
    void sg_display_file(uint8_t *data);
};

#endif // MA_JPEG_VIEWER_H
