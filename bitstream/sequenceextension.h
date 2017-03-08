#ifndef SEQUENCEEXTENSION_H
#define SEQUENCEEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class SequenceExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int profile_and_level_indication;
    int progressive_sequence;
    int chroma_format;
    int horizontal_size_extension;
    int vertical_size_extension;
    int bit_rate_extension;
    int marker_bit;
    int vbv_buffer_size_extension;
    int low_delay;
    int frame_rate_extension_n;
    int frame_rate_extension_d;
public:
    SequenceExtension();
    bool parse(StreamReader * reader);
};

#endif // SEQUENCEEXTENSION_H
