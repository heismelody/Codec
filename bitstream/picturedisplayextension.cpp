#include "picturedisplayextension.h"

PictureDisplayExtension::PictureDisplayExtension()
{

}

bool PictureDisplayExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    reader->readNBits_int(4);
    int number_frame_centre_offsets = 0;
    for(int i = 0; i < number_frame_centre_offsets; i++)
    {
        frame_centre_horizontal_offset  = reader->readNBits_int(16);
        marker_bit                      = reader->readNBits_int(1);
        frame_centre_vertical_offset    = reader->readNBits_int(16);
        marker_bit                      = reader->readNBits_int(1);
        //frame_centre_offsets[i] = ;
    }
    qDebug() << "--------Pic display ext -------------";
    qDebug() << "frame_centre_horizontal_offset" << frame_centre_horizontal_offset;
    qDebug() << "frame_centre_vertical_offset" << frame_centre_vertical_offset;
    reader->next_start_code();
}
