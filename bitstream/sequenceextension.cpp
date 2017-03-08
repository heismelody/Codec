#include "sequenceextension.h"

SequenceExtension::SequenceExtension()
{

}

bool SequenceExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    extension_start_code_identifier = reader->readNBits_int(4);
    profile_and_level_indication = reader->readNBits_int(8);
    progressive_sequence         = reader->readNBits_int(1);
    chroma_format                = reader->readNBits_int(2);
    horizontal_size_extension    = reader->readNBits_int(2);
    vertical_size_extension      = reader->readNBits_int(2);
    bit_rate_extension           = reader->readNBits_int(12);
    marker_bit                   = reader->readNBits_int(1);
    vbv_buffer_size_extension    = reader->readNBits_int(8);
    low_delay                    = reader->readNBits_int(1);
    frame_rate_extension_n       = reader->readNBits_int(2);
    frame_rate_extension_d       = reader->readNBits_int(5);
    reader->next_start_code();

    qDebug() << "-------------seq extension" ;
    qDebug() << "extension_start_code_identifier" << extension_start_code_identifier;
    qDebug() << "profile_and_level_indication" << profile_and_level_indication;
    qDebug() << "progressive_sequence" << progressive_sequence;
    qDebug() << "chroma_format" << chroma_format;
    qDebug() << "horizontal_size_extension" << horizontal_size_extension;
    qDebug() << "vertical_size_extension" << vertical_size_extension;
    qDebug() << "bit_rate_extension" << bit_rate_extension;
    qDebug() << "marker_bit" << marker_bit;
    qDebug() << "vbv_buffer_size_extension" << vbv_buffer_size_extension;
    qDebug() << "low_delay" << low_delay;
    qDebug() << "frame_rate_extension_n" << frame_rate_extension_n;
    qDebug() << "frame_rate_extension_d" << frame_rate_extension_d;

    return true;
}
