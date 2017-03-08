#include "mpegslice.h"

MPEGSlice::MPEGSlice()
{

}

bool MPEGSlice::parse(StreamReader * reader,MPEGContext * ctx)
{
//    qDebug() << reader->readNBits_int(32);
//    reader->skip_to_next_start_code();


    reader->readNBits_int(32);      //slice start code
    if(ctx->vertical_size > 2800)
    {
        slice_vertical_position_extension = reader->readNBits_int(3);
    }
    if(ctx->seq_scala_ext_present)
    {
        if(ctx->scalable_mode == MPEGConst::SCALEMODE_DATA_PARTITIONING)
        {
            priority_breakpoint           = reader->readNBits_int(7);
        }
    }
    quantiser_scale_mode                  = reader->readNBits_int(5);
    if( reader->nextbits(1,1) )
    {
        intra_slice_flag                  = reader->readNBits_int(1);
        intra_slice                       = reader->readNBits_int(1);
        reserved_bits                     = reader->readNBits_int(7);
        while ( reader->nextbits(1,1) )
        {
            extra_bit_slice               = reader->readNBits_int(1);
            extra_information_slice       = reader->readNBits_int(8);
        }
    }
    extra_bit_slice                       = reader->readNBits_int(1);
    int i = -1;
    do
    {
        i++;
        mbs.append(new Macroblock());
        parseMacroblock(reader,mbs.at(i));
    }
    while( !reader->nextbits(23,0x0) );
    reader->next_start_code();

    return true;
}

bool MPEGSlice::parseMacroblock(StreamReader * reader, Macroblock * mb)
{
    while ( reader->nextbits(11,00000001000))
    {
        mb->macroblock_escape = reader->readNBits_int(11);
    }
    mb->macroblock_address_increment = reader->readVLC_MBAddress();
    qDebug() << mb->macroblock_address_increment;

    //parseMacroModes();
//    if(macroblock_quant)
//    {
//        quantiser_scale_code;
//    }
//    if(macroblock_motion_forward
//       || (macroblock_intra && concealment_motion_vectors))
//    {
//        motion_vectors(0);
//    }
//    if(macroblock_motion_backward)
//    {
//        motion_vectors(1);
//    }
//    if( macroblock_intra && concealment_motion_vectors )
//    {
//        coded_block_pattern();
//    }
//    for(int i = 0; i < block_count; i++)
//    {
//        block(i);
//    }
    return true;
}


bool MPEGSlice::parseMacroModes(StreamReader * reader, Macroblock * mb)
{
    //macroblock_type = reader->readVLC_MbType_I();
//    if( (1 == spatial_temporal_weight_flag)
//       && (00 != spatial_temporal_weight_code_table_index))
//    {
//        spatial_temporal_weight_code = reader->readNBits_int(2);
//    }
//    if(macroblock_motion_forward
//       || macroblock_motion_backward)
//    {
//        if(picture_structure == "frame")
//        {
//            if(frame_pred_dct == 0)
//            {
//                frame_motion_type = reader->readNBits_int(2);
//            }
//        }
//        else
//        {
//            field_motion_type = reader->readNBits_int(2);
//        }
//    }
//    if(decode_dct_type)
//    {
//        dct_type = reader->readNBits_int(1);
//    }
//    return true;
}


bool MPEGSlice::parseMacroVectorS(StreamReader * reader, Macroblock * mb)
{
//    if(1 == motion_vector_count)
//    {
//        if((mv_format == field) && (dmv != 1))
//        {
//            motion_vertical_field_select[0][s] = reader->readNBits_int(1);
//        }
//        motion_vector(0,s);
//    }
//    else
//    {
//        motion_vertical_field_select[0][s] = reader->readNBits_int(1);
//        motion_vector(0,s);
//        motion_vertical_field_select[1][s] = reader->readNBits_int(1);
//        motion_vector(1,s);
//    }

    return true;
}


bool MPEGSlice::parseMacroVector(StreamReader * reader, Macroblock * mb)
{
//    motion_code[r][s][0];
//    if((1 != f_code[s][0]) && (0 != motion_code[r][s][0]))
//    {
//        motion_residual[r][s][0];
//    }
//    if(1 == dmv)
//    {
//        dmvector[0];
//    }
//    motion_code[r][s][1];
//    if((1 != f_code[s][0]) && (0 != motion_code[r][s][1]))
//    {
//        motion_residual[r][s][1];
//    }
//    if(1 == dmv)
//    {
//        dmvector[1];
//    }

    return true;
}


bool MPEGSlice::parseCodedBlockPattern(StreamReader * reader, Macroblock * mb)
{
//    coded_block_pattern_420;
//    if(chroma_format == "4:2:2")
//    {
//        coded_block_pattern_1 = reader->readNBits_int(2);
//    }
//    if(chroma_format == "4:4:4")
//    {
//        coded_block_pattern_2 = reader->readNBits_int(6);
//    }

    return true;
}


bool MPEGSlice::parseBlock(StreamReader * reader, Block * mb)
{
//    if(pattern_code[i])
//    {
//        if(macroblock_intra)
//        {
//            if(i < 4)
//            {
//                dct_dc_size_luminance;
//                if(0 != dct_dc_size_luminance)
//                {
//                    dct_dc_differential;
//                }
//            }
//            else
//            {
//                dct_dc_size_chrominance;
//                if(0 != dct_dc_size_chrominance)
//                {
//                    dct_dc_differential;
//                }
//            }
//        }
//        else
//        {
//            FirstDCTcoefficient;
//        }
//        while ( reader->nextbits(2,00))
//        {
//            subsequentDCTcoeff;
//        }
//        EndofBlock;
//    }

    return true;
}
