#ifndef PICTUREDISPLAYEXTENSION_H
#define PICTUREDISPLAYEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class PictureDisplayExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int frame_centre_horizontal_offset;
    int marker_bit;
    int frame_centre_vertical_offset;
    //int marker_bit;

public:
    PictureDisplayExtension();
    bool parse(StreamReader * reader);
};

#endif // PICTUREDISPLAYEXTENSION_H
