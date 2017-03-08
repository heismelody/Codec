#ifndef STREAMREADER_H
#define STREAMREADER_H

#include<QObject>
#include<QFile>
#include<QDataStream>

#include "mpegutil.h"
#include "streamreader.h"
#include "mpegconst.h"
#include "vlc.h"

QT_BEGIN_NAMESPACE
class QDataStream;
class QString;
class QFile;
class QBitArray;
QT_END_NAMESPACE

class StreamReader : public QObject
{
    Q_OBJECT
public:
    StreamReader();
    StreamReader(QString filename);

    QBitArray read1Bit();
    QBitArray readNBits(int n);
    QBitArray readNBytes(int n);
    int readNBits_int(int n);
    int readNBytes_int(int n);
    int read1Bit_int();

    int getNBytes_int(int n);
    int getNBits_int(int n);
    int get1bit_int(int n);

    uint getNBytes_uint(int n);
    uint getNBits_uint(int n);
    uint get1bit_uint(int n);

    bool nextbits(int size,int bits);
    bool bytealigned();
    bool next_start_code();
    int skip(int n);
    bool skip_zero_byte();
    bool skip_zero_bit();

    int readVLC();

    int skip_to_next_start_code();

    int readVLC_MBAddress();
    int readVLC_MbType_I();
    int readVLC_MbType_B();
    int readVLC_MbType_P();
    int readVLC_MbPattern();
    int readVLC_MVCode();
    int readVLC_DMVector();
    int readVLC_LumDCSize();
    int readVLC_ChromDCSize();
private:
    QDataStream* readstream;
    QFile* readfile;

    int _initpos;
    char _bytetemp;
    int _byteoffset = 0;  //indicate the position of current readed byte in read stream.
                        //[7 6 5 4 3 2 1 0]is the byte position count.
};

#endif // STREAMREADER_H
