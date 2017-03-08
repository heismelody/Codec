#ifndef GOPHEADER_H
#define GOPHEADER_H

#include "basebitstream.h"
#include "../streamreader.h"

class GOPHeader : public BaseBitStream
{
public:
    int group_start_code;
    int time_code;
    int closed_gop;
    int broken_link;

public:
    GOPHeader();
    bool parse(StreamReader * reader);
};

#endif // GOPHEADER_H
