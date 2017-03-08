#include "sequenceheader.h"

SequenceHeader::SequenceHeader()
{
}

bool SequenceHeader::parse(StreamReader * reader)
{
    this->seq_hdr_code                      = reader->readNBits_int(32);
    if( MPEGConst::SEQUENCE_HEADER_CODE != seq_hdr_code )
    {
        return false;
    }
    this->horizontal_size_value             = reader->readNBits_int(12);
    this->vertical_size_value               = reader->readNBits_int(12);
    this->aspect_ratio_information          = reader->readNBits_int(4);
    this->frame_rate_code                   = reader->readNBits_int(4);
    this->bit_rate_value                    = reader->readNBits_int(18);
    this->marker_bit                        = reader->readNBits_int(1);
    this->vbv_buffer_size_value             = reader->readNBits_int(10);
    this->constrained_parameters_flag       = reader->readNBits_int(1);
    this->load_intra_quantiser_matrix       = reader->readNBits_int(1);
    if(this->load_intra_quantiser_matrix)
    {
        this->intra_quantiser_matrix = new int[64];
        for (int i = 0; i < 64; i++)
        {
            this->intra_quantiser_matrix[i] = reader->readNBits_int(8);
        }
    }
    this->load_non_intra_quantiser_matrix   = reader->readNBits_int(1);
    if(this->load_non_intra_quantiser_matrix)
    {
        this->non_intra_quantiser_matrix = new int[64];
        for (int i = 0; i < 64; i++)
        {
            this->non_intra_quantiser_matrix[i] = reader->readNBits_int(8);
        }
    }
    reader->next_start_code();
    qDebug() << "horizontal_size_value" << this->horizontal_size_value;
    qDebug() << "vertical_size_value" << vertical_size_value;
    qDebug() << "aspect_ratio_information" << aspect_ratio_information;
    qDebug() << "frame_rate_code" << frame_rate_code;
    qDebug() << "bit_rate_value" << bit_rate_value;
    qDebug() << "vbv_buffer_size_value" << vbv_buffer_size_value;
    qDebug() << "constrained_parameters_flag" << constrained_parameters_flag;
    qDebug() << "load_intra_quantiser_matrix" << load_intra_quantiser_matrix;
    qDebug() << "load_non_intra_quantiser_matrix" << load_non_intra_quantiser_matrix;

    return true;
}
