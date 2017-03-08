#ifndef MPEGGOP_H
#define MPEGGOP_H

#include "./bitstream/gopheader.h"

#include "mpegpicture.h"
class MPEGGOP
{
public:
    MPEGGOP();
    bool parseHeader(StreamReader * reader);
    bool parseUserNExtData(StreamReader * reader);

    MPEGPicture *getPicAt(int pic_index);
    bool addNewPic();

//variable
    GOPHeader hdr;
    QList<int> userdata;
private:
    QList<MPEGPicture *> pics;

    int test;
};

#endif // MPEGGOP_H
