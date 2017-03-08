#ifndef MPEGMARCOBLOCK_H
#define MPEGMARCOBLOCK_H

#include "streamreader.h"
class MPEGMarcoblock
{
public:
    MPEGMarcoblock();
    bool parse(StreamReader * reader);
};

#endif // MPEGMARCOBLOCK_H
