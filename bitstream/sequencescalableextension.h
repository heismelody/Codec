#ifndef SEQUENCESCALABLEEXTENSION_H
#define SEQUENCESCALABLEEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"
#include "../mpegconst.h"

class SequenceScalableExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int scalable_mode;
    int layer_id;
    int lower_layer_prediction_horizontal_size;
    int marker_bit;
    int lower_layer_prediction_vertical_size;
    int horizontal_subsampling_factor_m;
    int horizontal_subsampling_factor_n;
    int vertical_subsampling_factor_m;
    int vertical_subsampling_factor_n;
    int piture_mux_enable;
    int mux_to_progressive_sequence;
    int piture_mux_order;
    int piture_mux_factor;

public:
    SequenceScalableExtension();
    bool parse(StreamReader * reader);
};

#endif // SEQUENCESCALABLEEXTENSION_H
