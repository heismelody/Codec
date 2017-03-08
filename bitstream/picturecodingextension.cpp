#include "picturecodingextension.h"

PictureCodingExtension::PictureCodingExtension()
{

}

bool PictureCodingExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    reader->readNBits_int(4);
    f_code[0][0]               = reader->readNBits_int(4);
    f_code[0][1]               = reader->readNBits_int(4);
    f_code[1][0]               = reader->readNBits_int(4);
    f_code[1][1]               = reader->readNBits_int(4);
    intra_dc_precision         = reader->readNBits_int(2);
    picture_structure          = reader->readNBits_int(2);
    top_field_first            = reader->readNBits_int(1);
    frame_pred_frame_dct       = reader->readNBits_int(1);
    concealment_motion_vectors = reader->readNBits_int(1);
    q_scale_type               = reader->readNBits_int(1);
    intra_vlc_format           = reader->readNBits_int(1);
    alternate_scan             = reader->readNBits_int(1);
    repeat_first_field         = reader->readNBits_int(1);
    chroma_420_type            = reader->readNBits_int(1);
    progressive_frame          = reader->readNBits_int(1);
    composite_display_flag     = reader->readNBits_int(1);
    if(composite_display_flag)
    {
        v_axis                 = reader->readNBits_int(1);
        field_sequence         = reader->readNBits_int(3);
        sub_carrier            = reader->readNBits_int(1);
        burst_amplitude        = reader->readNBits_int(7);
        sub_carrier_phase      = reader->readNBits_int(8);
    }
    reader->next_start_code();

    qDebug() << "-------------pic coding ext--------";
    qDebug() << "f_code1" << f_code[0][0];
    qDebug() << "f_code2" << f_code[0][1];
    qDebug() << "f_code3" << f_code[1][0];
    qDebug() << "f_code4" << f_code[1][1];
    qDebug() << "intra_dc_precision" << intra_dc_precision;
    qDebug() << "picture_structure" << picture_structure;
    qDebug() << "top_field_first" << top_field_first;
    qDebug() << "frame_pred_frame_dct" << frame_pred_frame_dct;
    return true;
}
