#ifndef SEQUENCEDISPLAYEXTENSION_H
#define SEQUENCEDISPLAYEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class SequenceDisplayExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int video_format;
    int colour_description;
    int colour_primaries;
    int transfer_characteristics;
    int matrix_coefficients;
    int display_horizontal_size;
    int marker_bit;
    int display_vertical_size;

public:
    SequenceDisplayExtension();
    bool parse(StreamReader * reader);
};

#endif // SEQUENCEDISPLAYEXTENSION_H
