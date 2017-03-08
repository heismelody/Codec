#include "picturetemporalscalableextension.h"

PictureTemporalScalableExtension::PictureTemporalScalableExtension()
{

}

bool PictureTemporalScalableExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    reader->readNBits_int(4);
    reference_select_code      = reader->readNBits_int(2);
    forward_temporal_reference = reader->readNBits_int(10);
    marker_bit                 = reader->readNBits_int(1);
    backwar_temporal_reference = reader->readNBits_int(10);
    reader->next_start_code();

    qDebug() << "--------Temporal ext -------------";
    qDebug() << "reference_select_code" << reference_select_code;
    qDebug() << "forward_temporal_reference" << forward_temporal_reference;
    qDebug() << "backwar_temporal_reference" << backwar_temporal_reference;
    return true;
}
