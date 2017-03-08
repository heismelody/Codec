#ifndef PICTURETEMPORALSCALABLEEXTENSION_H
#define PICTURETEMPORALSCALABLEEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class PictureTemporalScalableExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int reference_select_code;
    int forward_temporal_reference;
    int marker_bit;
    int backwar_temporal_reference;

public:
    PictureTemporalScalableExtension();
    bool parse(StreamReader * reader);
};

#endif // PICTURETEMPORALSCALABLEEXTENSION_H
