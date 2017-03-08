#include "sequencedisplayextension.h"

SequenceDisplayExtension::SequenceDisplayExtension()
{

}

bool SequenceDisplayExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    extension_start_code_identifier = reader->readNBits_int(4);
    video_format                    = reader->readNBits_int(3);
    colour_description              = reader->readNBits_int(1);
    if(colour_description)
    {
        colour_primaries            = reader->readNBits_int(8);
        transfer_characteristics    = reader->readNBits_int(8);
        matrix_coefficients         = reader->readNBits_int(8);
    }
    display_horizontal_size         = reader->readNBits_int(14);
    marker_bit                      = reader->readNBits_int(1);
    display_vertical_size           = reader->readNBits_int(14);
    reader->next_start_code();

    qDebug() << "--------seq display ext -------------";
    qDebug() << "video_format" << video_format;
    qDebug() << "colour_description" << colour_description;
    qDebug() << "display_horizontal_size" << display_horizontal_size;
    qDebug() << "display_vertical_size" << display_vertical_size;

    return true;
}
