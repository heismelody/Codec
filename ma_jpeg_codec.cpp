#include <QtWidgets>
#include <QtMath>

#include "ma_jpeg_codec.h"
#define BYTECLIP(v) Clip8[(uint)(v) & 0x3FF]

#define JPEG_MARKER_SOF0   0xFFC0
#define JPEG_MARKER_SOF1   0xFFC1
#define JPEG_MARKER_SOF2   0xFFC2
#define JPEG_MARKER_SOF3   0xFFC3

#define JPEG_MARKER_SOF5   0xFFC5
#define JPEG_MARKER_SOF6   0xFFC6
#define JPEG_MARKER_SOF7   0xFFC7

#define JPEG_MARKER_JPG    0xFFC8
#define JPEG_MARKER_SOF9   0xFFC9
#define JPEG_MARKER_SOF10  0xFFCA
#define JPEG_MARKER_SOF11  0xFFCB

#define JPEG_MARKER_SOF13  0xFFCD
#define JPEG_MARKER_SOF14  0xFFCE
#define JPEG_MARKER_SOF15  0xFFCF

#define JPEG_MARKER_DHT  0xFFC4
#define JPEG_MARKER_DAC  0xFFCC
//#define JPEG_MARKER_RST  0xFFD0 - 0xFFD7
#define IS_JPEG_MARKER_RST(m) (m >= 0xFFD0) && (m <= 0xFFD7) ? 1 : 0

#define JPEG_MARKER_SOI  0xFFD8
#define JPEG_MARKER_EOI  0xFFD9
#define JPEG_MARKER_SOS  0xFFDA
#define JPEG_MARKER_DQT  0xFFDB
#define JPEG_MARKER_DNL  0xFFDC
#define JPEG_MARKER_DRI  0xFFDD
#define JPEG_MARKER_DHP  0xFFDE
#define JPEG_MARKER_EXP  0xFFDF
//#define JPEG_MARKER_APP  0xFFE0 - 0xFFEF
//#define JPEG_MARKER_JPG  0xFFF0 - 0xFFFD
#define IS_JPEG_MARKER_APP(m) (m >= 0xFFE0) && (m <= 0xFFEF) ? 1 : 0
#define IS_JPEG_MARKER_JPG(m) (m >= 0xFFF0) && (m <= 0xFFFD) ? 1 : 0
#define JPEG_MARKER_COM  0xFFFE

#define JPEG_MARKER_TEM  0xFF01
//#define JPEG_MARKER_RES  0xFF02 - 0xFFBF

using namespace std;

// Zig-zag sequence of quantized DCT coefficients
const uint8_t ma_jpeg_codec::zigzad_order[64] =
{
    0,   1,  5,  6, 14, 15, 27, 28,
    2,   4,  7, 13, 16, 26, 29, 42,
    3,   8, 12, 17, 25, 30, 41, 43,
    9,  11, 18, 24, 31, 40, 44, 53,
    10, 19, 23, 32, 39, 45, 52, 54,
    20, 22, 33, 38, 46, 51, 55, 60,
    21, 34, 37, 47, 50, 56, 59, 61,
    35, 36, 48, 49, 57, 58, 62, 63,
};

const uint8_t ma_jpeg_codec::zigzad_reverse_order[64] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
};

static int16_t fsd[1024] = {
    /* 0..255 */
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
    64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
    96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
    128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
    160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
    192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
    224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255,
    /* 256..511 */
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    /* -512..-257 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* -256..-1 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const uint8_t ma_jpeg_codec::default_quantization_table_luma[] =
{
    16,11,10,16, 24, 40, 51, 61,
    12,12,14,19, 26, 58, 60, 55,
    14,13,16,24, 40, 57, 69, 56,
    14,17,22,29, 51, 87, 80, 62,
    18,22,37,56, 68,109,103, 77,
    24,35,55,64, 81,104,113, 92,
    49,64,78,87,103,121,120,101,
    72,92,95,98,112,100,103, 99,
};

const uint8_t ma_jpeg_codec::default_quantization_table_chroma[] =
{
    17,18,24,47,99,99,99,99,
    18,21,26,66,99,99,99,99,
    24,26,56,99,99,99,99,99,
    47,66,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,
};

const uint8_t ma_jpeg_codec::default_huffman_table_dc_len_luma[16] =
{
    0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0
};

const uint8_t ma_jpeg_codec::default_huffman_table_dc_len_chroma[16] =
{
    0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0
};

const uint8_t ma_jpeg_codec::default_huffman_table_dc_val_luma[12] =
{
    0,1,2,3,4,5,6,7,8,9,10,11
};

const uint8_t ma_jpeg_codec::default_huffman_table_dc_val_chroma[12] =
{
    0,1,2,3,4,5,6,7,8,9,10,11
};

const uint8_t ma_jpeg_codec::default_huffman_table_ac_len_luma[16] =
{
    0x00,0x02,0x01,0x03,0x03,0x02,0x04,0x03,0x05,0x05,0x04,0x04,0x00,0x00,0x01,0x7D
};

const uint8_t ma_jpeg_codec::default_huffman_table_ac_len_chroma[16] =
{
    0x00,0x02,0x01,0x02,0x04,0x04,0x03,0x04,0x07,0x05,0x04,0x04,0x00,0x01,0x02,0x77
};

const uint8_t ma_jpeg_codec::default_huffman_table_ac_val_luma[] =
{
    0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
    0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xA1, 0x08, 0x23, 0x42, 0xB1, 0xC1, 0x15, 0x52, 0xD1, 0xF0,
    0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0A, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
    0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
    0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
    0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3, 0xC4, 0xC5,
    0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xE1, 0xE2,
    0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};

const uint8_t ma_jpeg_codec::default_huffman_table_ac_val_chroma[] =
{
    0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
    0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xA1, 0xB1, 0xC1, 0x09, 0x23, 0x33, 0x52, 0xF0,
    0x15, 0x62, 0x72, 0xD1, 0x0A, 0x16, 0x24, 0x34, 0xE1, 0x25, 0xF1, 0x17, 0x18, 0x19, 0x1A, 0x26,
    0x27, 0x28, 0x29, 0x2A, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x4A, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x6A, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8A, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0xA2, 0xA3, 0xA4, 0xA5,
    0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xC2, 0xC3,
    0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA,
    0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0xFA
};

uint16_t get_ptr_16bits_data(char * ptr)
{
    uint16_t data;
    QByteArray tempData;

    tempData = tempData + *ptr;
    tempData = tempData + *(ptr+1) + *(ptr+2) + *(ptr+3);
    data = strtoul(tempData.data(), NULL, 16);

    return data;
}

uint8_t get_ptr_8bits_data(char * ptr)
{
    uint8_t data;
    QByteArray tempData;

    tempData = tempData + *ptr;
    tempData = tempData + *(ptr+1);
    data = strtoul(tempData.data(), NULL, 16);

    return data;
}

uint8_t get_ptr_4bits_data(char * ptr)
{
    uint8_t data;
    QByteArray tempData;

    tempData = tempData + *ptr;
    data = strtoul(tempData.data(), NULL, 16);

    return data;
}

bool isMarkerTableOrMisStart(uint16_t marker)
{
    return ((JPEG_MARKER_DQT == marker)
            || (JPEG_MARKER_DHT == marker)
            || (JPEG_MARKER_DAC == marker)
            || (JPEG_MARKER_DRI == marker)
            || IS_JPEG_MARKER_APP(marker)
            || (JPEG_MARKER_COM == marker));
}

bool isMarkerSOF(uint16_t marker)
{
    return ((JPEG_MARKER_SOF0 == marker)
            || (JPEG_MARKER_SOF1 == marker)
            || (JPEG_MARKER_SOF2 == marker)
            || (JPEG_MARKER_SOF3 == marker)
            || (JPEG_MARKER_SOF5 == marker)
            || (JPEG_MARKER_SOF6 == marker)
            || (JPEG_MARKER_SOF7 == marker)
            || (JPEG_MARKER_JPG == marker)
            || (JPEG_MARKER_SOF9 == marker)
            || (JPEG_MARKER_SOF10 == marker)
            || (JPEG_MARKER_SOF11 == marker)
            || (JPEG_MARKER_SOF13 == marker)
            || (JPEG_MARKER_SOF14 == marker)
            || (JPEG_MARKER_SOF15 == marker));
}

bool isMarkerFrameStart(uint16_t marker)
{
    return (isMarkerSOF(marker)
            || isMarkerTableOrMisStart(marker)
            || (JPEG_MARKER_EXP == marker));
}

bool isMarkerScanStart(uint16_t marker)
{
    return ((JPEG_MARKER_SOS == marker)
            || isMarkerTableOrMisStart(marker));
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis(
    char ** pCur,
    Decode *dec)
{
    uint16_t marker;

parse_tableORmis_part_loop:
    marker = get_ptr_16bits_data(*pCur);
    switch (marker & 0xffff)
    {
        //Define quantization table(s)/DQT
        case JPEG_MARKER_DQT:
            jpeg_parse_tableORmis_qtable(pCur,dec);
            goto parse_tableORmis_part_loop;
        //  Huffman table/DHT
        case JPEG_MARKER_DHT:
            jpeg_parse_tableORmis_hufftable(pCur,dec);
            goto parse_tableORmis_part_loop;

        //Arithmetic coding conditioning
        case JPEG_MARKER_DAC:
            jpeg_parse_tableORmis_arithtable(pCur,dec);
            goto parse_tableORmis_part_loop;

        // Define restart interval/DRI
        case JPEG_MARKER_DRI:
            jpeg_parse_tableORmis_restart(pCur,dec);
            goto parse_tableORmis_part_loop;

        //  Comment/COM
        case JPEG_MARKER_COM:
            jpeg_parse_tableORmis_comment(pCur,dec);
            goto parse_tableORmis_part_loop;

        //Reserved for application segments/APP
        case 0xFFE0:
        case 0xFFE1:
        case 0xFFE2:
        case 0xFFE3:
        case 0xFFE4:
        case 0xFFE5:
        case 0xFFE6:
        case 0xFFE7:
        case 0xFFE8:
        case 0xFFE9:
        case 0xFFEA:
        case 0xFFEB:
        case 0xFFEC:
        case 0xFFED:
        case 0xFFEE:
        case 0xFFEF:
            jpeg_parse_tableORmis_app(pCur,dec);
            goto parse_tableORmis_part_loop;

        default:
            break;
    }
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_qtable(
    char **pCur,
    Decode *dec)
{
    //DQT
    uint16_t length;
    uint16_t offset_byte_from_Lh = 0;
    uint qtable_precosion,
         qtable_dst_id;

    *pCur+=4;
    //Quantization table definition length
    length = get_ptr_16bits_data(*pCur);
    *pCur+=4;

    // Quantization table elements
    while ((offset_byte_from_Lh + 2) < length)
    {
        //Quantization table element precision
        qtable_precosion = get_ptr_4bits_data(*pCur);
        qtable_precosion &= 1;
        (*pCur)++;
        //Quantization table destination identifie
        qtable_dst_id = get_ptr_4bits_data(*pCur);
        qtable_dst_id &= 3;
        (*pCur)++;

        //8-bit.the precision of the Qk values.
        if(qtable_precosion == 0)
        {
           for(int i = 0; i < 64; i++)
           {
                dec->qtable[qtable_dst_id][i] = (uint16_t)get_ptr_8bits_data(*pCur);
                *pCur+=2;
           }
        }
        //16-bit
        else if(qtable_precosion == 1)
        {
           for(int i = 0; i < 64; i++)
           {
                dec->qtable[qtable_dst_id][i] = get_ptr_16bits_data(*pCur);
                (*pCur)++;
           }
        }

        offset_byte_from_Lh = 65 + 64 * qtable_precosion + offset_byte_from_Lh;
    }
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_hufftable(
    char * *pCur,
    Decode *dec)
{
    uint16_t length;
    uint16_t offset_byte_from_Lh = 0;
    uint8_t hufftable_class,       //Table class-- 0 = DC table or lossless table, 1 = AC table
         hufftable_dst_id;      //Th

    *pCur+=4;
    //Huffman table definition length
    length = get_ptr_16bits_data(*pCur);
    *pCur+=4;
    //Multiple huffman table
    while ((offset_byte_from_Lh + 2) < length)
    {
        uint16_t sum_temp = 0;

        //table class,AC or DC
        hufftable_class = (HUFFCLASS)(uint8_t)get_ptr_4bits_data(*pCur);
        (*pCur)++;
        //des id
        hufftable_dst_id = (HUFFTABLE_ID)(uint8_t)get_ptr_4bits_data(*pCur);
        (*pCur)++;
        //Number of Huffman codes of length
        for(int i = 0; i < 16; i++)
        {
             dec->bits[hufftable_class][hufftable_dst_id][i] = get_ptr_8bits_data(*pCur);
             *pCur+=2;

             sum_temp+= dec->bits[hufftable_class][hufftable_dst_id][i];
        }
        // Value associated with each Huffman code
        delete[] dec->huffval[hufftable_class][hufftable_dst_id];
        dec->huffval[hufftable_class][hufftable_dst_id] = new uint8_t[sum_temp];
        for(int i = 0; i < sum_temp; i++)
        {
             dec->huffval[hufftable_class][hufftable_dst_id][i] = get_ptr_8bits_data(*pCur);
             *pCur+=2;
        }

        offset_byte_from_Lh = 17 + sum_temp + offset_byte_from_Lh;    //class-4bits,id-4bits,huffcode-16bytes
    }

}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_arithtable(
    char **pCur,
    Decode *dec)
{

}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_restart(
    char **pCur,
    Decode *dec)
{
    //RSTi = get_ptr_16bits_data(pCur+8);
    *pCur+=12;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_comment(
    char **pCur,
    Decode *dec)
{
    //COM
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_tableORmis_app(
    char **pCur,
    Decode *dec)
{
    //APP
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_SOI(
    char **pCur,
    Decode *dec)
{
    //parse SOI
    if((get_ptr_16bits_data(*pCur) & 0xffff) != 0xffd8)
    { return JPEG_SOI_NOT_FOUND; }
    *pCur+=4;

    return JPEG_OK;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_DHP(
    char * *pCur,
    Decode *dec)
{
    if(JPEG_MARKER_DHP != get_ptr_16bits_data(*pCur)){ return JPEG_OK; }
    //APP
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_EOI(
    char **pCur,
    Decode *dec)
{
    //APP
    if(JPEG_MARKER_EOI != get_ptr_16bits_data(*pCur)){ return JPEG_EOI_NOT_FOUND; }
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_EXP(
    char **pCur,
    Decode *dec)
{
    if(JPEG_MARKER_EXP != get_ptr_16bits_data(*pCur)){ return JPEG_OK; }
    //APP
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_SOF(
    char **pCur,
    Decode *dec)
{
    //SOF
    uint16_t length;

    *pCur+=4;
    //frame header length
    length = get_ptr_16bits_data(*pCur);
    *pCur+=4;
    //sample precision
    dec->sample_precision = get_ptr_8bits_data(*pCur);
    *pCur+=2;
    //number of lines
    dec->height = get_ptr_16bits_data(*pCur);
    *pCur+=4;
    //number of samples per line
    dec->width = get_ptr_16bits_data(*pCur);
    *pCur+=4;
    //number of image components in frame
    dec->frame_component_num = get_ptr_8bits_data(*pCur);
    if(dec->frame_component_num != 3) { return JPEG_ONLY_SUPPORT_THREE_COM; }
    *pCur+=2;
    dec->frame_components = new FrameComponent[3];
    for(int i = 0; i < dec->frame_component_num; i++)
    {
        //component id
        (dec->frame_components[i]).id = get_ptr_8bits_data(*pCur);
        *pCur+=2;
        //hor sampling factors
        (dec->frame_components[i]).h = (H_FACTOR)get_ptr_4bits_data(*pCur);
        (*pCur)++;
        //ver sampling factors
        (dec->frame_components[i]).v = (V_FACTOR)get_ptr_4bits_data(*pCur);
        (*pCur)++;
        //quantization table destination selector
        (dec->frame_components[i]).qtable = (QTABLE_ID)get_ptr_8bits_data(*pCur);
        *pCur+=2;

        dec->block8x8_per_comp[i] = (dec->frame_components[i]).h * (dec->frame_components[i]).v;
    }
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_SOS(
    char **pCur,
    Decode *dec)
{
    //SOS
    uint16_t length;
    uint8_t sample_precision,
            image_component;

    *pCur+=4;
    // Scan header length
    length = get_ptr_16bits_data(*pCur);
    *pCur+=4;
    // Number of image components in scan
    dec->scan_component_num = get_ptr_8bits_data(*pCur);
    *pCur+=2;

    delete dec->scan_components;
    dec->scan_components = new ScanComponent[dec->scan_component_num];
    for(int i = 0; i < dec->scan_component_num; i++)
    {
        //Scan component selector
        (dec->scan_components[i]).id = get_ptr_8bits_data(*pCur);
        *pCur+=2;
        //DC entropy coding table destination selector
        (dec->scan_components[i]).DC_dst_id = (HUFFTABLE_ID)get_ptr_4bits_data(*pCur);
        (*pCur)++;
        //AC entropy coding table destination selector
        (dec->scan_components[i]).AC_dst_id = (HUFFTABLE_ID)get_ptr_4bits_data(*pCur);
        (*pCur)++;
    }

    //Start of spectral or predictor selection
    //spectral_start = get_ptr_8bits_data(pCur);
    *pCur+=2;
    //End of spectral selection
    //spectral_end = get_ptr_8bits_data(pCur);
    *pCur+=2;
    //Successive approximation bit position high
    //approx_high = get_ptr_4bits_data(pCur);
    (*pCur)++;
    //Successive approximation bit position low or point transform
    //approx_low = get_ptr_4bits_data(pCur);
    (*pCur)++;
}


void
ma_jpeg_codec::jpeg_decode_MCU(
        char ** pCur,
        Decode *dec)
{
    int MCU_Ns_sum = 0;
    int MCU_Ns_cop[dec->frame_component_num];
    int MCU_cop_id[16];

    for(int k = 0, i = 0; k < dec->frame_component_num; k++)
    {
        MCU_Ns_cop[k] = (dec->frame_components[k]).h * (dec->frame_components[k]).v;
        MCU_Ns_sum+=MCU_Ns_cop[k];
        MCU_Ns_cop[k] = MCU_Ns_sum;
        for( ; i < MCU_Ns_sum; i++)
        {
            dec->MCU_comp_id[i] = k;
            MCU_cop_id[i] = k;
        }
    }

    int i = 0;
    int sum = 0;
    dec->hor_MCU_num = (dec->width)/16 + 1;
    dec->ver_MCU_num = (dec->height)/16 + 1;
    const int stored_width = dec->hor_MCU_num * 16;
    const int stored_height = dec->ver_MCU_num * 16;
    dec->decodedata = new uint8_t[stored_width*stored_height*3];        //not sufficient here, need to make imporvement
    memset(dec->block8X8, 0, sizeof(dec->block8X8[0][0]) * 64 * 12);

    while(true)
    {
        if(JPEG_ECS_OK == jpeg_decode_DC((dec->scan_components[MCU_cop_id[i]]).DC_dst_id,pCur,dec))
        {
            break;
        }
        if(JPEG_ECS_OK == jpeg_decode_AC((dec->scan_components[MCU_cop_id[i]]).AC_dst_id,pCur,dec))
        {
            break;
        }

        i++;
        (dec->comp_count)++;
        if(i == MCU_Ns_sum)
        {
            switch (dec->block8x8_per_comp[1])
            {
                case 4:
                    decode8x8(dec->decodedata,dec->block8X8,0,4,8,dec);
                    decode8x8(dec->decodedata,dec->block8X8,1,5,9,dec);
                    decode8x8(dec->decodedata,dec->block8X8,2,6,10,dec);
                    decode8x8(dec->decodedata,dec->block8X8,3,7,11,dec);
                    break;
                case 2:
                    decode8x8(dec->decodedata,dec->block8X8,0,4,6,dec);
                    decode8x8(dec->decodedata,dec->block8X8,1,4,6,dec);
                    decode8x8(dec->decodedata,dec->block8X8,2,5,7,dec);
                    decode8x8(dec->decodedata,dec->block8X8,3,5,7,dec);
                    break;
                case 1:
                    decode8x8(dec->decodedata,dec->block8X8,0,4,5,dec);
                    decode8x8(dec->decodedata,dec->block8X8,1,4,5,dec);
                    decode8x8(dec->decodedata,dec->block8X8,2,4,5,dec);
                    decode8x8(dec->decodedata,dec->block8X8,3,4,5,dec);
                    break;
                default:
                    break;
            }
            sum++;
            dec->hor_MCU_count = sum % dec->hor_MCU_num;
            dec->ver_MCU_count = sum / dec->hor_MCU_num;
        }
        i = i % MCU_Ns_sum;
        dec->comp_count = i;
    }
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_decode_DC(
        HUFFTABLE_ID id,
        char ** pCur,
        Decode *dec)
{
#define iC (dec->comp_count)
#define idC (dec->MCU_comp_id)
    uint8_t t;
    uint16_t diff;
    int16_t result;

    if(JPEG_ECS_OK == jpeg_decode_DECODE(HUFFCLASS_DC,id,pCur,dec))
    {
        return JPEG_ECS_OK;
    }
    t = dec->decoded_temp_val;
    diff = jpeg_decode_RECEIVE(t,pCur,dec);
    result = jpeg_decode_EXTEND(diff,t);

    dec->block8X8[iC][0] = result + dec->preDC[idC[iC]];
    dec->preDC[idC[iC]] = dec->block8X8[iC][0];
    return JPEG_OK;
#undef iC
#undef idC
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_decode_AC(
        HUFFTABLE_ID id,
        char ** pCur,
        Decode *dec)
{
#define iC (dec->comp_count)
    uint8_t RS,SSSS,RRRR,R;
    int16_t ZZ_k;
    int k = 1;

    while(true)
    {
        if(JPEG_ECS_OK == jpeg_decode_DECODE(HUFFCLASS_AC,id,pCur,dec))
        {
            return JPEG_ECS_OK;
        }
        RS = dec->decoded_temp_val;
        SSSS = RS % 16;
        RRRR = RS >> 4;
        R = RRRR;

        if(SSSS != 0)
        {
            k+=R;
            ZZ_k = jpeg_decode_ZZ(SSSS,pCur,dec);
            dec->block8X8[iC][k] = ZZ_k;
            if(k == 63){ return JPEG_OK; }
            else
            {
                k++;
                continue;
            }
        }
        else
        {
            if(R == 15)
            {
                k+=16;
                continue;
            }
            else { return JPEG_OK; }
        }
    }
#undef iC
}

void
ma_jpeg_codec::jpeg_decode_decodeTableGeneration(
        HUFFCLASS huffclass,
        HUFFTABLE_ID id,
        Decode *dec)
{
    std::fill_n(dec->MINCODE[huffclass][id], 16, -1);
    std::fill_n(dec->MAXCODE[huffclass][id], 16, -1);

    int8_t bitcount = 0;
    for(int i=0; i < 16 ; i++)
    {
        for(int j = 0; j < dec->bits[huffclass][id][i]; j++)
        {
            if((dec->huffcode[huffclass][id][j+bitcount] < dec->MINCODE[huffclass][id][i])
                    || (dec->MINCODE[huffclass][id][i] == -1))
            {
                dec->MINCODE[huffclass][id][i] = dec->huffcode[huffclass][id][j+bitcount];
            }

            if(dec->huffcode[huffclass][id][j+bitcount] > dec->MAXCODE[huffclass][id][i])
            {
                dec->MAXCODE[huffclass][id][i] = dec->huffcode[huffclass][id][j+bitcount];
            }
        }
        dec->valptr[huffclass][id][i] = bitcount;
        bitcount = bitcount + dec->bits[huffclass][id][i];
    }

}

void
ma_jpeg_codec::jpeg_decode_huffsizeGeneration(
        HUFFCLASS huffclass,
        HUFFTABLE_ID id,
        Decode *dec)
{
    dec->huffsize[huffclass][id] = new uint8_t[256];

    uint8_t index = 0;
    for(int i = 0; i < 16; i++)
    {
        for(int j = 0; j < dec->bits[huffclass][id][i]; j++)
        {
            dec->huffsize[huffclass][id][index] = (uint8_t) (i+1);
            index++;
        }
    }
}

void
ma_jpeg_codec::jpeg_decode_huffcodeGeneration(
        HUFFCLASS huffclass,
        HUFFTABLE_ID id,
        Decode *dec)
{
    uint16_t si = dec->huffsize[huffclass][id][0];
    uint16_t code = 0;

    dec->huffcode[huffclass][id] = new uint16_t[256];

    for(int i = 0, k = 0; i < 16; i++)
    {
        while(si == dec->huffsize[huffclass][id][k])
        {
            dec->huffcode[huffclass][id][k] = code;
            code++;
            k++;
        }
        code <<= 1;
        si++;
    }
}

int16_t
ma_jpeg_codec::jpeg_decode_ZZ(
        uint8_t size,
        char ** pCur,
        Decode *dec)
{
    uint16_t ZZ_k;
    int16_t result;

    ZZ_k = jpeg_decode_RECEIVE(size,pCur,dec);
    result = jpeg_decode_EXTEND(ZZ_k,size);

    return result;
}

uint8_t
ma_jpeg_codec::jpeg_decode_NEXTBYTE(
        char * pCur)
{
    uint8_t data;
    QByteArray tempData;

    tempData = tempData + *pCur;
    tempData = tempData + *(pCur+1);
    data = strtoul(tempData.data(), NULL, 16);

    return data;
}

int8_t
ma_jpeg_codec::jpeg_decode_NEXTBIT(
        char ** pCur,
        Decode *dec)
{
    uint8_t b,b2,bit;

    if(dec->count == 0)
    {
        b = jpeg_decode_NEXTBYTE(*pCur);
        dec->count = 8;

        if(b == 0xFF)
        {
            b2 = jpeg_decode_NEXTBYTE(*pCur+2);
            if(b2 == 0)
            {
                dec->is_stuff = true;
                goto return_BIT;
            }
            else
            {
                if(b2 == 0xDC)
                {
                    //jpeg_parse_DNL();
                }
                else
                {
                    return -1;//error
                }
            }
        }
        else
        {
            goto return_BIT;
        }
    }
    else
    {
        b = jpeg_decode_NEXTBYTE(*pCur);
    }
return_BIT:
    (dec->count)--;
   //qDebug() << "b" <<b;
   //qDebug() << "count" << dec->count;
    bit = (b & ( 1 << (dec->count) )) >> (dec->count);
    //qDebug() << "bit" <<bit;
    if(dec->count == 0)
    {
        if(dec->is_stuff == true)
        {
            //qDebug() << "1111111111";
            (*pCur)+=4;
            dec->is_stuff = false;
        }
        else
        {
            (*pCur)+=2;
        }
    }
    //qDebug() << "16bits:" << get_ptr_16bits_data(*pCur);;
     //qDebug() << "----";
    return bit;
}

uint16_t
ma_jpeg_codec::jpeg_decode_RECEIVE(
        uint8_t ssss,
        char ** pCur,
        Decode *dec)
{
    uint16_t V = 0;
    for(int i = 0; i != ssss; i++)
    {
        V = (V << 1) + (uint16_t)jpeg_decode_NEXTBIT(pCur,dec);
    }
    //qDebug() << "receive:" << V;

    return V;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_decode_DECODE(
        HUFFCLASS huffclass,
        HUFFTABLE_ID id,
        char ** pCur,
        Decode *dec)
{
    int i = 0;
    uint16_t J;
    int8_t nextbit;
    uint16_t code;

    nextbit = jpeg_decode_NEXTBIT(pCur,dec);
    if(-1 == nextbit){ return JPEG_ECS_OK; }
    code = (uint16_t)nextbit;
    while(code > dec->MAXCODE[huffclass][id][i])
    {
        nextbit = jpeg_decode_NEXTBIT(pCur,dec);
        if(-1 == nextbit) { return JPEG_ECS_OK; }
        code = (code << 1) + (uint16_t)nextbit;
        i++;
    }

    J = dec->valptr[huffclass][id][i];
    J = J + code - dec->MINCODE[huffclass][id][i];
    dec->decoded_temp_val = dec->huffval[huffclass][id][J];

    return JPEG_OK;
}

int16_t
ma_jpeg_codec::jpeg_decode_EXTEND(
        uint16_t diff,
        uint8_t t)
{
    int16_t Vt = (int)qPow(2,(t-1));
    int16_t V = diff;

    if(V < Vt)
    {
        Vt = (-1 << t) + 1;
        V = V + Vt;
    }
    return V;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_ECS(
    char **pCur,
    Decode *dec)
{
    //APP
    uint16_t length;
    for(int i = 0; i < dec->scan_component_num; i++)
    {
        jpeg_decode_huffsizeGeneration(HUFFCLASS_DC,(dec->scan_components[i]).DC_dst_id,dec);
        jpeg_decode_huffcodeGeneration(HUFFCLASS_DC,(dec->scan_components[i]).DC_dst_id,dec);
        jpeg_decode_decodeTableGeneration(HUFFCLASS_DC,(dec->scan_components[i]).DC_dst_id,dec);

        jpeg_decode_huffsizeGeneration(HUFFCLASS_AC,(dec->scan_components[i]).AC_dst_id,dec);
        jpeg_decode_huffcodeGeneration(HUFFCLASS_AC,(dec->scan_components[i]).AC_dst_id,dec);
        jpeg_decode_decodeTableGeneration(HUFFCLASS_AC,(dec->scan_components[i]).AC_dst_id,dec);
    }

    jpeg_decode_MCU(pCur,dec);
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_parse_DNL(
    char ** pCur,
    Decode *dec)
{
    //APP
    uint16_t length;

    length = get_ptr_16bits_data(*pCur+4);
    length+=2;
    *pCur+=(2 * length);

}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_reserve_ZIG(
    int16_t * src,
    int * des)
{
    for (int i = 0; i < 64; i++)
    {
        des[zigzad_reverse_order[i]] = src[i];
    }

    return JPEG_OK;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_dequantize(
    int16_t * block,
    uint16_t * qtable)
{
    for (int i = 0; i < 64; i++)
    {
        block[i]*=qtable[i];
    }

    return JPEG_OK;
}

ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::decode8x8(uint8_t * result,
    int16_t block[12][64],int i,int j,int k,
    Decode *dec)
{
#define y (dec->ver_MCU_count)
#define x (dec->hor_MCU_count)
#define h (dec->ver_MCU_num*16)
#define w (dec->hor_MCU_num*16)
    int temp[6][64];

    for (int a = 0; a < 64; a++)
    {
        block[i][a]*=dec->qtable[(dec->frame_components[dec->MCU_comp_id[i]]).qtable][a];
        block[j][a]*=dec->qtable[(dec->frame_components[dec->MCU_comp_id[j]]).qtable][a];
        block[k][a]*=dec->qtable[(dec->frame_components[dec->MCU_comp_id[k]]).qtable][a];
    }
    for (int b = 0; b < 64; b++)
    {
        temp[0][zigzad_reverse_order[b]] = block[i][b];
        temp[1][zigzad_reverse_order[b]] = block[j][b];
        temp[2][zigzad_reverse_order[b]] = block[k][b];
    }
    IDCT2(temp[0]);
    IDCT2(temp[1]);
    IDCT2(temp[2]);

//    for( size_t y = 0; y < W; ++y )
//    {
//        for( size_t x = 0; x < H; ++x )
//        {
//            data[0][x][y] = ( rand() % 255 ) * 255 * 255 * 255;
//            data[1][x][y] = ( rand() % 255 ) * 255 * 255 * 255;
//            data[2][x][y] = ( rand() % 255 ) * 255 * 255 * 255;
//        }
//    }
    int indexX = 0;
    int indexY = 0;
    switch (i) {
        case 0:
            indexX = 16*x;
            indexY = 16*y;
            break;
        case 1:
            indexX = 16*x + 8;
            indexY = 16*y;
            break;
        case 2:
            indexX = 16*x;
            indexY = 16*y + 8;
            break;
        case 3:
            indexX = 16*x + 8;
            indexY = 16*y + 8;
            break;
        default:
            break;
    }
    for(int a = 0, t = 0; a < 8; a++)
    {
        for(int b = 0; b < 8; b++)
        {
            t = a*8 + b;
            (dec->decodedata)[3 * ((indexY+a) * w + (indexX+b)) + 0] = (uint8_t)(temp[0][t]                      + 1.14*temp[2][t]   + 128);
            (dec->decodedata)[3 * ((indexY+a) * w + (indexX+b)) + 1] = (uint8_t)(temp[0][t] - 0.395*temp[1][t]   - 0.581*temp[2][t]  + 128);
            (dec->decodedata)[3 * ((indexY+a) * w + (indexX+b)) + 2] = (uint8_t)(temp[0][t] + 1.772*temp[1][t]                       + 128);
        }
    }

//    for(int a = 0; a < 64; a++)
//    {
//        temp[3][a] = temp[0][a]                      + 1.14*temp[2][a]   + 128;
//        temp[4][a] = temp[0][a] - 0.395*temp[1][a]   - 0.581*temp[2][a]  + 128;
//        temp[5][a] = temp[0][a] + 1.772*temp[1][a]                       + 128;
//    }

    return JPEG_OK;
#undef x
#undef y
#undef w
#undef h
}

void ma_jpeg_codec::IDCT2(int *data)
{
    static const int c1=251 ; /* cos(pi/16)<<8 */
    static const int s1=50 ; /* sin(pi/16)<<8 */
    static const int c3=213 ; /* cos(3pi/16)<<8 */
    static const int s3=142 ; /* sin(3pi/16)<<8 */
    static const int r2c6=277; /* cos(6pi/16)*sqrt(2)<<9 */
    static const int r2s6=669;
    static const int r2=181; /* sqrt(2)<<7 */
    int i;
    int x0,x1,x2,x3,x4,x5,x6,x7,x8;
    /* Stage 4 */
    for(i=0;i<8;i++){
        x0=data[0]<<9;
        x1=data[1]<<7;
        x2=data[2];
        x3=data[3]*r2;
        x4=data[4]<<9;
        x5=data[5]*r2;
        x6=data[6];
        x7=data[7]<<7;
        x8=x7+x1;
        x1 -= x7;
        /* Stage 3 */
        x7=x0+x4;
        x0-=x4;
        x4=x1+x5;
        x1-=x5;
        x5=x3+x8;
        x8-=x3;
        x3=r2c6*(x2+x6);
        x6=x3+(-r2c6-r2s6)*x6;
        x2=x3+(-r2c6+r2s6)*x2;
        /* Stage 2 */
        x3=x7+x2;
        x7-=x2;
        x2=x0+x6;
        x0-= x6;
        x6=c3*(x4+x5);
        x5=(x6+(-c3-s3)*x5)>>6;
        x4=(x6+(-c3+s3)*x4)>>6;
        x6=c1*(x1+x8);
        x1=(x6+(-c1-s1)*x1)>>6;
        x8=(x6+(-c1+s1)*x8)>>6;
        /* Stage 1, rounding and output */
        x7+=512; x2+=512;x0+=512;x3+=512;
        *data++=(x3+x4)>>10; *data++=(x2+x8)>>10;
        *data++=(x0+x1)>>10; *data++=(x7+x5)>>10;
        *data++=(x7-x5)>>10; *data++=(x0-x1)>>10;
        *data++=(x2-x8)>>10; *data++=(x3-x4)>>10;
    }
    data -= 64;
    for(i=0;i<8;i++){
        x0=data[0]<<9, x1=data[8]<<7, x2=data[16],
        x3=data[24]*r2, x4=data[32]<<9, x5=data[40]*r2,
        x6=data[48], x7=data[56]<<7;
        x8=x7+x1; x1 -= x7;
        /* Stage 3 */
        x7=x0+x4; x0-=x4; x4=x1+x5; x1-=x5; x5=x3+x8; x8-=x3;
        x3=r2c6*(x2+x6);x6=x3+(-r2c6-r2s6)*x6;x2=x3+(-r2c6+r2s6)*x2;
        /* Stage 2 */
        x3=x7+x2; x7-=x2; x2=x0+x6; x0-= x6;
        x6=c3*(x4+x5);x5=(x6+(-c3-s3)*x5)>>6;x4=(x6+(-c3+s3)*x4)>>6;
        x6=c1*(x1+x8);x1=(x6+(-c1-s1)*x1)>>6;x8=(x6+(-c1+s1)*x8)>>6;
        /* Stage 1, rounding and output */
        x7+=512; x2+=512;x0+=512;x3+=512;
        data[0]=(x3+x4)>>11; data[8]=(x2+x8)>>11;
        data[16]=(x0+x1)>>11; data[24]=(x7+x5)>>11;
        data[32]=(x7-x5)>>11; data[40]=(x0-x1)>>11;
        data[48]=(x2-x8)>>11; data[56]=(x3-x4)>>11;
        data ++;
    }
    data -=8;
}
ma_jpeg_codec::JPEG_CODE
ma_jpeg_codec::jpeg_decode(
        QByteArray *src,
        uint8_t *dst)
{
    Decode * dec = new Decode();
    dec->count = 0;
    char * pCur = src->data();
    uint16_t marker;

    if(JPEG_OK != jpeg_parse_SOI(&pCur,dec))
    { return JPEG_SOI_ERROR; }
    jpeg_parse_tableORmis(&pCur,dec);

    marker = get_ptr_16bits_data(pCur);
    // parse frame/SOF
    if(true == isMarkerFrameStart(marker))
    {
        jpeg_parse_DHP(&pCur,dec);
        goto jpeg_parse_frame_header;
    }
    //Parse DHP.Hierarchical mode
    else if(JPEG_MARKER_DHP == marker)
    {
        goto jpeg_parse_frame_loop_start;
    }
    //Parse EOI.only has SOI,table,EOI. Abbreviated format for table-specification data
    else if(JPEG_MARKER_EOI == (marker & 0xffff))
    {
        jpeg_parse_EOI(&pCur,dec);
        goto jpeg_parse_complete;
    }

jpeg_parse_frame_loop_start:
    jpeg_parse_tableORmis(&pCur,dec);
    jpeg_parse_EXP(&pCur,dec);
jpeg_parse_frame_header:
    jpeg_parse_SOF(&pCur,dec);
jpeg_parse_scan_loop_start:
    jpeg_parse_tableORmis(&pCur,dec);
    jpeg_parse_SOS(&pCur,dec);
    jpeg_parse_ECS(&pCur,dec);
    //jpeg_parse_DNL(&pCur,dec);
//--jpeg_parse_scan_loop_end:
//--jpeg_parse_frame_loop_end:

    marker = get_ptr_16bits_data(pCur);
    // parse next frame/Multiframe
    if(true == isMarkerFrameStart(marker))
    {
        goto jpeg_parse_frame_loop_start;
    }
    //parse next scan/Multiscan
    else if(true == isMarkerScanStart(marker))
    {
        goto jpeg_parse_frame_loop_start;
    }
    else if(JPEG_MARKER_EOI == (marker & 0xffff))
    {
        jpeg_parse_EOI(&pCur,dec);
        goto jpeg_parse_complete;
    }
    dst = dec->decodedata;

jpeg_parse_complete:
    return JPEG_OK;
}

void ma_jpeg_codec::jpeg_encode(
        QByteArray *src,
        QByteArray *dst)
{

}

ma_jpeg_codec::ma_jpeg_codec()
    : file(new QByteArray)
{
}

void ma_jpeg_codec::setFile(QByteArray *file)
{
    this->file = file;
}

void ma_jpeg_codec::sl_in_file(QByteArray *file)
{
    this->setFile(file);
    this->decode();
}

void ma_jpeg_codec::decode()
{
    uint8_t* decodeData;

    jpeg_decode(this->file,decodeData);

    emit this->sg_decode_complete(decodeData);
}

// By Thomas G. Lane.
// http://developer.download.nvidia.com/SDK/9.5/Samples/vidimaging_samples.html#gpgpu_dct
// float data[64]
void ma_jpeg_codec::jpeg_fdct(float *data)
{
    float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
    float tmp10, tmp11, tmp12, tmp13;
    float z1, z2, z3, z4, z5, z11, z13;
    float *dataptr;
    int ctr;

    /* Pass 1: process rows. */

    dataptr = data;
    for ( ctr = 7; ctr >= 0; ctr-- ) {
        tmp0 = dataptr[0] + dataptr[7];
        tmp7 = dataptr[0] - dataptr[7];
        tmp1 = dataptr[1] + dataptr[6];
        tmp6 = dataptr[1] - dataptr[6];
        tmp2 = dataptr[2] + dataptr[5];
        tmp5 = dataptr[2] - dataptr[5];
        tmp3 = dataptr[3] + dataptr[4];
        tmp4 = dataptr[3] - dataptr[4];

        /* Even part */

        tmp10 = tmp0 + tmp3;    /* phase 2 */
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        dataptr[0] = tmp10 + tmp11; /* phase 3 */
        dataptr[4] = tmp10 - tmp11;

        z1 = (tmp12 + tmp13) * ((float) 0.707106781); /* c4 */
        dataptr[2] = tmp13 + z1;    /* phase 5 */
        dataptr[6] = tmp13 - z1;

        /* Odd part */

        tmp10 = tmp4 + tmp5;    /* phase 2 */
        tmp11 = tmp5 + tmp6;
        tmp12 = tmp6 + tmp7;

        /* The rotator is modified from fig 4-8 to avoid extra negations. */
        z5 = (tmp10 - tmp12) * ((float) 0.382683433); /* c6 */
        z2 = ((float) 0.541196100) * tmp10 + z5; /* c2-c6 */
        z4 = ((float) 1.306562965) * tmp12 + z5; /* c2+c6 */
        z3 = tmp11 * ((float) 0.707106781); /* c4 */

        z11 = tmp7 + z3;        /* phase 5 */
        z13 = tmp7 - z3;

        dataptr[5] = z13 + z2;  /* phase 6 */
        dataptr[3] = z13 - z2;
        dataptr[1] = z11 + z4;
        dataptr[7] = z11 - z4;

        dataptr += 8;     /* advance pointer to next row */
    }

    /* Pass 2: process columns. */

    dataptr = data;
    for ( ctr = 8-1; ctr >= 0; ctr-- ) {
        tmp0 = dataptr[8*0] + dataptr[8*7];
        tmp7 = dataptr[8*0] - dataptr[8*7];
        tmp1 = dataptr[8*1] + dataptr[8*6];
        tmp6 = dataptr[8*1] - dataptr[8*6];
        tmp2 = dataptr[8*2] + dataptr[8*5];
        tmp5 = dataptr[8*2] - dataptr[8*5];
        tmp3 = dataptr[8*3] + dataptr[8*4];
        tmp4 = dataptr[8*3] - dataptr[8*4];

        /* Even part */

        tmp10 = tmp0 + tmp3;    /* phase 2 */
        tmp13 = tmp0 - tmp3;
        tmp11 = tmp1 + tmp2;
        tmp12 = tmp1 - tmp2;

        dataptr[8*0] = tmp10 + tmp11; /* phase 3 */
        dataptr[8*4] = tmp10 - tmp11;

        z1 = (tmp12 + tmp13) * ((float) 0.707106781); /* c4 */
        dataptr[8*2] = tmp13 + z1; /* phase 5 */
        dataptr[8*6] = tmp13 - z1;

        /* Odd part */

        tmp10 = tmp4 + tmp5;    /* phase 2 */
        tmp11 = tmp5 + tmp6;
        tmp12 = tmp6 + tmp7;

        /* The rotator is modified from fig 4-8 to avoid extra negations. */
        z5 = (tmp10 - tmp12) * ((float) 0.382683433); /* c6 */
        z2 = ((float) 0.541196100) * tmp10 + z5; /* c2-c6 */
        z4 = ((float) 1.306562965) * tmp12 + z5; /* c2+c6 */
        z3 = tmp11 * ((float) 0.707106781); /* c4 */

        z11 = tmp7 + z3;        /* phase 5 */
        z13 = tmp7 - z3;

        dataptr[8*5] = z13 + z2; /* phase 6 */
        dataptr[8*3] = z13 - z2;
        dataptr[8*1] = z11 + z4;
        dataptr[8*7] = z11 - z4;

        dataptr++;          /* advance pointer to next column */
    }
}

void ma_jpeg_codec::jpeg_idct(long *src,
    long *dst
)
{

}



#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

/* global declarations */
//void init_idct _ANSI_ARGS_((void));
//void idct _ANSI_ARGS_((short *block));
void init_idct (void);
void idct (short *block);

/* private data */
static short iclip[1024]; /* clipping table */
static short *iclp;

/* private prototypes */
//static void idctrow _ANSI_ARGS_((short *blk));
//static void idctcol _ANSI_ARGS_((short *blk));
static void idctrow (short *blk);
static void idctcol (short *blk);

/* row (horizontal) IDCT
 *
 *           7                       pi         1
 * dst[k] = sum c[l] * src[l] * cos( -- * ( k + - ) * l )
 *          l=0                      8          2
 *
 * where: c[0]    = 128
 *        c[1..7] = 128*sqrt(2)
 */

static void idctrow(short *blk)
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* shortcut */
  if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |
        (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
  {
    blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;
    return;
  }

  x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

  /* first stage */
  x8 = W7*(x4+x5);
  x4 = x8 + (W1-W7)*x4;
  x5 = x8 - (W1+W7)*x5;
  x8 = W3*(x6+x7);
  x6 = x8 - (W3-W5)*x6;
  x7 = x8 - (W3+W5)*x7;

  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2);
  x2 = x1 - (W2+W6)*x2;
  x3 = x1 + (W2-W6)*x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;

  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;

  /* fourth stage */
  blk[0] = (x7+x1)>>8;
  blk[1] = (x3+x2)>>8;
  blk[2] = (x0+x4)>>8;
  blk[3] = (x8+x6)>>8;
  blk[4] = (x8-x6)>>8;
  blk[5] = (x0-x4)>>8;
  blk[6] = (x3-x2)>>8;
  blk[7] = (x7-x1)>>8;
}

/* column (vertical) IDCT
 *
 *             7                         pi         1
 * dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )
 *            l=0                        8          2
 *
 * where: c[0]    = 1/1024
 *        c[1..7] = (1/1024)*sqrt(2)
 */
static void idctcol(short *blk)
{
  int x0, x1, x2, x3, x4, x5, x6, x7, x8;

  /* shortcut */
  if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |
        (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))
  {
    blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]=blk[8*6]=blk[8*7]=
      iclp[(blk[8*0]+32)>>6];
    return;
  }

  x0 = (blk[8*0]<<8) + 8192;

  /* first stage */
  x8 = W7*(x4+x5) + 4;
  x4 = (x8+(W1-W7)*x4)>>3;
  x5 = (x8-(W1+W7)*x5)>>3;
  x8 = W3*(x6+x7) + 4;
  x6 = (x8-(W3-W5)*x6)>>3;
  x7 = (x8-(W3+W5)*x7)>>3;

  /* second stage */
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6*(x3+x2) + 4;
  x2 = (x1-(W2+W6)*x2)>>3;
  x3 = (x1+(W2-W6)*x3)>>3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;

  /* third stage */
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181*(x4+x5)+128)>>8;
  x4 = (181*(x4-x5)+128)>>8;

  /* fourth stage */
  blk[8*0] = iclp[(x7+x1)>>14];
  blk[8*1] = iclp[(x3+x2)>>14];
  blk[8*2] = iclp[(x0+x4)>>14];
  blk[8*3] = iclp[(x8+x6)>>14];
  blk[8*4] = iclp[(x8-x6)>>14];
  blk[8*5] = iclp[(x0-x4)>>14];
  blk[8*6] = iclp[(x3-x2)>>14];
  blk[8*7] = iclp[(x7-x1)>>14];
}

/* two dimensional inverse discrete cosine transform */
void idct(short *block)
{
  int i;

  for (i=0; i<8; i++)
    idctrow(block+8*i);

  for (i=0; i<8; i++)
    idctcol(block+i);

  for(i = 0;i < 64; i++)
  {
      qDebug("%d",block[i]);
  }
}

void init_idct()
{
  int i;

  iclp = iclip+512;
  for (i= -512; i<512; i++)
    iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);
}
