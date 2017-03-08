#ifndef SEQUENCEHEADER_H
#define SEQUENCEHEADER_H

#include <QWidget>

#include "basebitstream.h"
#include "../streamreader.h"
#include "../mpegutil.h"
#include "../mpegconst.h"

class SequenceHeader : public BaseBitStream
{
public:
    int seq_hdr_code;
    int horizontal_size_value;
    int vertical_size_value;
    int aspect_ratio_information;
    int frame_rate_code;
    int bit_rate_value;
    int marker_bit;
    int vbv_buffer_size_value;
    int constrained_parameters_flag;
    int load_intra_quantiser_matrix;
    int *intra_quantiser_matrix;
    int load_non_intra_quantiser_matrix;
    int *non_intra_quantiser_matrix;

public:
    SequenceHeader();
    bool parse(StreamReader * reader);
};

#endif // SEQUENCEHEADER_H
