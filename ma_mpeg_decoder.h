#ifndef MA_MPEG_DECODER_H
#define MA_MPEG_DECODER_H

#include<QObject>
#include <QtWidgets>

#include "fileloader.h"
#include "ma_mpeg_decoder.h"
#include "streamreader.h"
#include "mpegconst.h"
#include "mpegcontext.h"
#include "mpegutil.h"

#include "./bitstream/sequenceheader.h"
#include "./bitstream/sequenceextension.h"
#include "mpegsequence.h"
#include "vlc.h"

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE

class ma_mpeg_decoder : public QObject
{
    Q_OBJECT
public:
    enum MPEG_CODE
    {
        MPEG_OK = 0,
        MPEG_SEQ_HDR_NOT_FOUND,
    };
    FileLoader* fileloader;
private:
    QString filename;

public:
    ma_mpeg_decoder();
    MPEG_CODE main_procdure();
    MPEG_CODE video_sequences(StreamReader * reader);

private:

public slots:
    void sl_open_file();
    void sl_mpg_file_opened(QString &filename);

signals:
    void sg_need_file();
};

#endif // MA_MPEG_DECODER_H
