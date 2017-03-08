#include "sequencescalableextension.h"

SequenceScalableExtension::SequenceScalableExtension()
{

}

bool SequenceScalableExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    extension_start_code_identifier             = reader->readNBits_int(4);
    scalable_mode                               = reader->readNBits_int(2);
    layer_id                                    = reader->readNBits_int(4);
    if(scalable_mode == MPEGConst::SCALEMODE_SPATIAL_SCALABILITY)
    {
        lower_layer_prediction_horizontal_size  = reader->readNBits_int(14);
        marker_bit                              = reader->readNBits_int(1);
        lower_layer_prediction_vertical_size    = reader->readNBits_int(14);
        horizontal_subsampling_factor_m         = reader->readNBits_int(5);
        horizontal_subsampling_factor_n         = reader->readNBits_int(5);
        vertical_subsampling_factor_m           = reader->readNBits_int(5);
        vertical_subsampling_factor_n           = reader->readNBits_int(5);
    }
    else if(scalable_mode == MPEGConst::SCALEMODE_TEMPORAL_SCALABILITYE)
    {
        piture_mux_enable                       = reader->readNBits_int(1);
        if(piture_mux_enable)
        {
            mux_to_progressive_sequence         = reader->readNBits_int(1);
        }
        piture_mux_order                        = reader->readNBits_int(3);
        piture_mux_factor                       = reader->readNBits_int(3);
    }
    reader->next_start_code();

    qDebug() << "---------------seq scala ext-------";
    qDebug() << "scalable_mode" << scalable_mode;
    qDebug() << "layer_id" << layer_id;
}
