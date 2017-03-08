#include <QtWidgets>

#include "streamreader.h"

using namespace MpegUtil;
using namespace VLC;

StreamReader::StreamReader()
{
}

StreamReader::StreamReader(QString filename)
{
    readfile = new QFile(filename);
    readfile->open(QIODevice::ReadOnly);
    readstream = new QDataStream(readfile);
    _initpos = readfile->pos();
}

QBitArray StreamReader::read1Bit()
{
    QBitArray b = QBitArray(1,true);

    if(_byteoffset == 0)
    {
        b = readNBytes(1);
        _byteoffset = 8;
        b.truncate(1);
    }
    else
    {
        b.setBit( 0,_bytetemp & (1 << (_byteoffset - 1) ) );
    }

    _byteoffset--;
    return b;
}

QBitArray StreamReader::readNBits(int n)
{
    QBitArray result;
    int bytenum = n / 8;
    int byteoff = n % 8;

    if(0 != byteoff) { bytenum++; }
    result = readNBytes(bytenum);
    _byteoffset = ((byteoff == 0) ? 0 : (8 - byteoff));
    result.truncate(n);

    return result;
}

QBitArray StreamReader::readNBytes(int n)
{
    char* temp = new char[n];
    QBitArray result = QBitArray(n*8,true);

    if((-1 == readstream->readRawData(temp,n))
        || readfile->atEnd())
    {
        return result;
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            for(int b = 0; b < 8; b++)
            {
                result.setBit( i*8+b, ((temp[i]) & (1 << (7-b))) );
            }
        }
        _bytetemp = temp[n-1];
        return result;
    }
}

int StreamReader::read1Bit_int()
{
    int b = 0;

    if(_byteoffset == 0)
    {
        b = readNBytes_int(1);
        _byteoffset = 8;
        b = MpegUtil::truncate(b,8,1);
    }
    else
    {
        b = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
    }
    _byteoffset--;

    return b;
}

int StreamReader::readNBits_int(int n)
{
    int result = 0;

    if(n == 0) { return 0; }
    if(_byteoffset == 0)    //read bits from a byte's bound
    {
        int bytenum = n / 8;
        int byteoff = n % 8;

        if(0 != byteoff) { bytenum++; }
        result = readNBytes_int(bytenum);
        result = MpegUtil::truncate(result,bytenum*8,n);
        _byteoffset = ((byteoff == 0) ? 0 : (8 - byteoff));
    }
    else if(n > _byteoffset)    //read bits not from a byte's bound
    {
        n = n - _byteoffset;
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = _byteoffset; i > 0; i--)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }

        int bytenum = n / 8;
        int byteoff = n % 8;
        int temp;

        if(0 != byteoff) { bytenum++; }
        temp = readNBytes_int(bytenum);
        temp = MpegUtil::truncate(temp,bytenum*8,n);
        _byteoffset = ((byteoff == 0) ? 0 : (8 - byteoff));
        result = temp | (result << n);
    }
    else
    {
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = 1; i < n; i++)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }
    }

    return result;
}

int StreamReader::readNBytes_int(int n)
{
    char* temp = new char[n];
    int result = 0;

    if((-1 == readstream->readRawData(temp,n))
        || readfile->atEnd())
    {
        return result;
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            for(int b = 0; b < 8; b++)
            {
                result = result | ( (temp[i] >> (7-b)) & 0x1 );
                if((i == (n-1)) && (b == 7)) { break; }
                result<<=1;
            }
        }
        _bytetemp = temp[n-1];

        return result;
    }
}

int StreamReader::getNBytes_int(int n)
{
    char* temp = new char[n];
    int result = 0;

    if((-1 == readstream->readRawData(temp,n))
        || readfile->atEnd())
    {
        return result;
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            for(int b = 0; b < 8; b++)
            {
                result = result | ( (temp[i] >> (7-b)) & 0x1 );
                if((i == (n-1)) && (b == 7)) { break; }
                result<<=1;
            }
        }
        readfile->seek(readfile->pos() - n);

        return result;
    }
}

int StreamReader::getNBits_int(int n)
{
    int result = 0;
    int preoff = _byteoffset;
    int pretempB = _bytetemp;

    if(n == 0) { return 0; }
    if(_byteoffset == 0)    //read bits from a byte's bound
    {
        int bytenum = n / 8;
        int byteoff = n % 8;

        if(0 != byteoff) { bytenum++; }
        result = getNBytes_int(bytenum);
        result = MpegUtil::truncate(result,bytenum*8,n);
    }
    else if(n > _byteoffset)    //read bits not from a byte's bound
    {
        n = n - _byteoffset;
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = _byteoffset; i > 0; i--)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }

        int bytenum = n / 8;
        int byteoff = n % 8;
        int temp;

        if(0 != byteoff) { bytenum++; }
        temp = getNBytes_int(bytenum);
        temp = MpegUtil::truncate(temp,bytenum*8,n);
        _byteoffset = ((byteoff == 0) ? 0 : (8 - byteoff));
        result = temp | (result << n);
    }
    else
    {
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = 1; i < n; i++)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }
    }
    _byteoffset = preoff;
    _bytetemp = pretempB;

    return result;
}

int StreamReader::get1bit_int(int n)
{
    int b = 0;

    if(_byteoffset == 0)
    {
        b = getNBytes_int(1);
        b = MpegUtil::truncate(b,8,1);
    }
    else
    {
        b = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
    }

    return b;
}

uint StreamReader::getNBytes_uint(int n)
{
    char* temp = new char[n];
    uint result = 0;

    if((-1 == readstream->readRawData(temp,n))
        || readfile->atEnd())
    {
        return result;
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            for(int b = 0; b < 8; b++)
            {
                result = result | ( (temp[i] >> (7-b)) & 0x1 );
                if((i == (n-1)) && (b == 7)) { break; }
                result<<=1;
            }
        }
        readfile->seek(readfile->pos() - n);

        return result;
    }
}

uint StreamReader::getNBits_uint(int n)
{
    uint result = 0;
    int preoff = _byteoffset;
    int pretempB = _bytetemp;

    if(n == 0) { return 0; }
    if(_byteoffset == 0)    //read bits from a byte's bound
    {
        int bytenum = n / 8;
        int byteoff = n % 8;

        if(0 != byteoff) { bytenum++; }
        result = getNBytes_uint(bytenum);
        result = MpegUtil::truncate(result,bytenum*8,n);
    }
    else if(n > _byteoffset)    //read bits not from a byte's bound
    {
        n = n - _byteoffset;
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = _byteoffset; i > 0; i--)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }

        int bytenum = n / 8;
        int byteoff = n % 8;
        int temp;

        if(0 != byteoff) { bytenum++; }
        temp = getNBytes_uint(bytenum);
        temp = MpegUtil::truncate(temp,bytenum*8,n);
        _byteoffset = ((byteoff == 0) ? 0 : (8 - byteoff));
        result = temp | (result << n);
    }
    else
    {
        result = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
        _byteoffset--;
        for(int i = 1; i < n; i++)
        {
            result<<=1;
            result = result | ( (_bytetemp>>(_byteoffset - 1)) & 1 );
            _byteoffset--;
        }
    }
    _byteoffset = preoff;
    _bytetemp = pretempB;

    return result;
}

uint StreamReader::get1bit_uint(int n)
{
    uint b = 0;

    if(_byteoffset == 0)
    {
        b = getNBytes_uint(1);
        b = MpegUtil::truncate(b,8,1);
    }
    else
    {
        b = ( (_bytetemp>>(_byteoffset - 1)) & 1 );
    }

    return b;
}

bool StreamReader::bytealigned()
{
    return (_byteoffset == 0) ? true : false;
}

bool StreamReader::nextbits(int size,int bits)
{
    int b = getNBits_int(size);

    return (bits == b);
}

bool StreamReader::next_start_code()
{
    while(!bytealigned())
    {
        skip_zero_bit();
    }
    while(!nextbits(24,MPEGConst::START_CODE_PREFIX))
    {
        skip_zero_byte();
    }
    return true;
}

int StreamReader::skip(int n)
{
//    char * temp;
//    int preoff = _byteoffset;
//    int skip = readstream->skipRawData(n);

//    _byteoffset = (_byteoffset + 8 - (n%8)) % 8;
//    if((n >= preoff) && (0 == _byteoffset))
//    {
//        readstream->readRawData(temp,1);
//        readfile->seek(readfile->pos() - 8);
//    }
//    else
//    {
//        readfile->seek(readfile->pos() - (8 - _byteoffset));
//        readstream->readRawData(temp,1);
//        readfile->seek(readfile->pos() - _byteoffset);
//    }
//    _bytetemp = temp[0];

    readNBytes_int(n);

    return 0;
}

bool StreamReader::skip_zero_bit()
{
    int b = read1Bit_int();

    return (b == 0) ? true : false;
}

bool StreamReader::skip_zero_byte()
{
    int b = readNBytes_int(1);

    return (b == MPEGConst::ZERO_BYTE_BITARRAY) ? true : false;
}

//remove later
int StreamReader::skip_to_next_start_code()
{
    while(!bytealigned())
    {
        skip(1);
    }
    while(!nextbits(24,MPEGConst::START_CODE_PREFIX))
    {
        skip(1);
    }
    int startbits = getNBits_int(32);

    return startbits;
}
//#define MACROBLOCK_INTRA 1
//#define MACROBLOCK_PATTERN 2
//#define MACROBLOCK_MOTION_BACKWARD 4
//#define MACROBLOCK_MOTION_FORWARD 8
//#define MACROBLOCK_QUANT 16
int StreamReader::readVLC_MBAddress()
{
    const MBAtab * tab;

    tab = readMbAddress(getNBytes_uint(4));
    skip(tab->len);

    return tab->mba;
}

int StreamReader::readVLC_MbType_I()
{
    const MBTtab * tab;

    tab = readMbType_I(getNBytes_uint(4));
    skip(tab->len);

    return tab->type;
}

int StreamReader::readVLC_MbType_B()
{
    const MBTtab * tab;

    tab = readMbType_B(getNBytes_uint(4));
    skip(tab->len);

    return tab->type;
}

int StreamReader::readVLC_MbType_P()
{
    const MBTtab * tab;

    tab = readMbType_P(getNBytes_uint(4));
    skip(tab->len);

    return tab->type;
}

int StreamReader::readVLC_MbPattern()
{
    const CBPtab * tab;

    tab = readMbPattern(getNBytes_uint(4));
    skip(tab->len);

    return tab->cbp;
}

int StreamReader::readVLC_MVCode()
{
    const MVtab * tab;

    tab = readMVCode(getNBytes_uint(4));
    skip(tab->len);

    return tab->code;
}

int StreamReader::readVLC_DMVector()
{
    const DMVtab * tab;

    tab = readDMVector(getNBytes_uint(4));
    skip(tab->len);

    return tab->dmv;
}

int StreamReader::readVLC_LumDCSize()
{
    const DCtab * tab;

    tab = readLumDCSize(getNBytes_uint(4));
    skip(tab->len);

    return tab->size;
}

int StreamReader::readVLC_ChromDCSize()
{
    const DCtab * tab;

    tab = readChromDCSize(getNBytes_uint(4));
    skip(tab->len);

    return tab->size;
}

//DCTtab * StreamReader::readVLC_DctTableZero()
//{
//    const DCTtab * tab;
//    DCTtab * result;

//    tab = readDctTableZero(getNBytes_uint(4));
//    skip((tab->len) + 1);

//    return result;
//}

//int StreamReader::readVLC_DctTableOne()
//{
//    const DCTtab * tab;
//    DCTtab * result;

//    tab = readDctTableOne(getNBytes_uint(4));
//    skip((tab->len) + 1);

//    return result;
//}
