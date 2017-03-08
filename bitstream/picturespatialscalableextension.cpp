#include "picturespatialscalableextension.h"

PictureSpatialScalableExtension::PictureSpatialScalableExtension()
{

}

bool PictureSpatialScalableExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    reader->readNBits_int(4);
    lower_layer_temporal_reference           = reader->readNBits_int(10);
    marker_bit                               = reader->readNBits_int(1);
    lower_layer_horizontal_offset            = reader->readNBits_int(15);
    marker_bit                               = reader->readNBits_int(1);
    lower_layer_vertical_offset              = reader->readNBits_int(15);
    spatial_temporal_weight_code_table_index = reader->readNBits_int(2);
    lower_layer_progressive_frame            = reader->readNBits_int(1);
    lower_layer_deinterlaced_field_select    = reader->readNBits_int(1);
    reader->next_start_code();

    qDebug() << "--------Scalable ext -------------";
    qDebug() << "lower_layer_temporal_reference" << lower_layer_temporal_reference;
    qDebug() << "lower_layer_horizontal_offset" << lower_layer_horizontal_offset;
    qDebug() << "lower_layer_vertical_offset" << lower_layer_vertical_offset;
    return true;
}
