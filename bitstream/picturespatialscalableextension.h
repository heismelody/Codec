#ifndef PICTURESPATIALSCALABLEEXTENSION_H
#define PICTURESPATIALSCALABLEEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class PictureSpatialScalableExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int lower_layer_temporal_reference;
    int marker_bit;
    int lower_layer_horizontal_offset;
    //sint marker_bit;
    int lower_layer_vertical_offset;
    int spatial_temporal_weight_code_table_index;
    int lower_layer_progressive_frame;
    int lower_layer_deinterlaced_field_select;

public:
    PictureSpatialScalableExtension();
    bool parse(StreamReader * reader);
};

#endif // PICTURESPATIALSCALABLEEXTENSION_H
