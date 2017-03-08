#ifndef MPEGCONST_H
#define MPEGCONST_H

#include<QObject>

#define MACROBLOCK_INTRA 1
#define MACROBLOCK_PATTERN 2
#define MACROBLOCK_MOTION_BACKWARD 4
#define MACROBLOCK_MOTION_FORWARD 8
#define MACROBLOCK_QUANT 16
class MPEGConst
{
public:
    static const int ZERO_BYTE_BITARRAY     = 0x00;
    static const int START_CODE_PREFIX      = 0x000001;

    //--------start_code, 8 bits------------
    static const int PICTURE_START_CODE     = 0x00000100;
    static const int SLICE_START_CODE_FIRST = 0x00000101;
    static const int SLICE_START_CODE_LAST  = 0x000001AF;
    //static const int RESERVED             = 0x000001B0;
    //static const int RESERVED             = 0x000001B1;
    static const int USER_DATA_START_CODE   = 0x000001B2;
    static const int SEQUENCE_HEADER_CODE   = 0x000001B3;
    static const int SEQUENCE_ERROR_CODE    = 0x000001B4;
    static const int EXTENSION_START_CODE   = 0x000001B5;
    //static const int RESERVED             = 0x000001B6;
    static const int SEQUENCE_END_CODE      = 0x000001B7;
    static const int GROUP_START_CODE       = 0x000001B8;
    static const int SYS_START_CODE_FIRST   = 0x000001B9;
    static const int SYS_START_CODE_LAST    = 0x000001FF;
    //--------start_code------------

    //------extension_start_code_identifier, 4 bits-------------------
    static const int SEQUENCE_EXTENSION_ID                  = 0x1;
    static const int SEQUENCE_DISPLAY_EXTENSION_ID          = 0x2;
    static const int QUANT_MATRIX_EXTENSION_ID              = 0x3;
    static const int SEQUENCE_SCALABLE_EXTENSION_ID         = 0x5;
    static const int PICTURE_DISPLAY_EXTENSION_ID           = 0x7;
    static const int PICTURE_CODING_EXTENSION_ID            = 0x8;
    static const int PICTURE_SPATIAL_SCALABLE_EXTENSION_ID  = 0x9;
    static const int PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID = 0xA;
    //------extension_start_code_identifier-------------------

    //------scale_mode,2 bits-------------------
    static const int SCALEMODE_DATA_PARTITIONING     = 0x0;
    static const int SCALEMODE_SPATIAL_SCALABILITY   = 0x1;
    static const int SCALEMODE_SNR_SCALABILITY       = 0x2;
    static const int SCALEMODE_TEMPORAL_SCALABILITYE = 0x3;
    //------scale_mode-------------------

//    int quantiser_scale_0[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36,
//                              38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58,  60,  62};
//    int quantiser_scale_1[] = { 0, 1, 2, 3, 4, 5,   6,  7,  8, 10, 12, 14, 16, 18, 20, 22, 24, 28, 32,
//                              36, 40, 44, 48, 52, 56, 64, 72, 80, 88, 96, 104, 112};
public:
    MPEGConst();
};

#endif // MPEGCONST_H
