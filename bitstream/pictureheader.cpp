#include "pictureheader.h"

PictureHeader::PictureHeader()
{

}

bool PictureHeader::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    temporal_reference            = reader->readNBits_int(10);
    pitcure_coding_type           = reader->readNBits_int(3);
    vbv_delay                     = reader->readNBits_int(16);
    if(pitcure_coding_type == 2 || pitcure_coding_type == 3)
    {
        full_per_forward_vector   = reader->readNBits_int(1);
        forward_f_code            = reader->readNBits_int(3);
    }
    if(pitcure_coding_type == 3)
    {
        full_per_backward_vector  = reader->readNBits_int(1);
        backward_f_code           = reader->readNBits_int(3);
    }
    while (reader->nextbits(1,1))
    {
        extra_bit_picture_1     = reader->readNBits_int(1);
        extra_information_picture = reader->readNBits_int(8);
    }
    extra_bit_picture_0           = reader->readNBits_int(1);
    reader->next_start_code();

    qDebug() << "--------Pic header -------------";
    qDebug() << "temporal_reference" << temporal_reference;
    qDebug() << "pitcure_coding_type" << pitcure_coding_type;
    qDebug() << "vbv_delay" << vbv_delay;
    return true;
}
