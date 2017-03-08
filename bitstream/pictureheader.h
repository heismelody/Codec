#ifndef PICTUREHEADER_H
#define PICTUREHEADER_H

#include "basebitstream.h"
#include "../streamreader.h"

class PictureHeader : public BaseBitStream
{
public:
    int temporal_reference;
    int pitcure_coding_type;
    int vbv_delay;
    int full_per_forward_vector;
    int forward_f_code;
    int full_per_backward_vector;
    int backward_f_code;
    int extra_bit_picture_1;
    int extra_information_picture;
    int extra_bit_picture_0;

public:
    PictureHeader();
    bool parse(StreamReader * reader);
};

#endif // PICTUREHEADER_H
