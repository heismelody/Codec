#ifndef PICTURECODINGEXTENSION_H
#define PICTURECODINGEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class PictureCodingExtension: public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int f_code[2][2];
    int intra_dc_precision;
    int picture_structure;
    int top_field_first;
    int frame_pred_frame_dct;
    int concealment_motion_vectors;
    int q_scale_type;
    int intra_vlc_format;
    int alternate_scan;
    int repeat_first_field;
    int chroma_420_type;
    int progressive_frame;
    int composite_display_flag;
    int v_axis;
    int field_sequence;
    int sub_carrier;
    int burst_amplitude;
    int sub_carrier_phase;

public:
    PictureCodingExtension();
    bool parse(StreamReader * reader);
};

#endif // PICTURECODINGEXTENSION_H
