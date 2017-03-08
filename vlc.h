#ifndef VLC_H
#define VLC_H

#include <QtWidgets>

#include "mpegconst.h"
namespace VLC
{
    #define UBITS(bit_buf,num) ((bit_buf) >> (32 - (num)))

    //-------------macroblock address increment
    typedef struct
    {
        int mba;
        int len;
    } MBAtab;
    static const MBAtab MBA_5_1 [] = {
        {7, 5}, {6, 5},
        {5, 4}, {5, 4}, {4, 4}, {4, 4},
        {3, 3}, {3, 3}, {3, 3}, {3, 3}, {2, 3}, {2, 3}, {2, 3}, {2, 3},
        {1, 1}, {1, 1}, {1, 1}, {1, 1},
        {1, 1}, {1, 1}, {1, 1}, {1, 1},
        {1, 1}, {1, 1}, {1, 1}, {1, 1},
        {1, 1}, {1, 1}, {1, 1}, {1, 1}
    };
    static const MBAtab MBA_11_7 [] = {
        {33, 11}, {32, 11}, {31, 11}, {30, 11},
        {29, 11}, {28, 11}, {27, 11}, {26, 11},
        {25, 11}, {24, 11}, {23, 11}, {22, 11},
        {21, 10}, {21, 10}, {20, 10}, {20, 10},
        {19, 10}, {19, 10}, {18, 10}, {18, 10},
        {17, 10}, {17, 10}, {16, 10}, {16, 10},
        {15,  8}, {15,  8}, {15,  8}, {15,  8},
        {15,  8}, {15,  8}, {15,  8}, {15,  8},
        {14,  8}, {14,  8}, {14,  8}, {14,  8},
        {14,  8}, {14,  8}, {14,  8}, {14,  8},
        {13,  8}, {13,  8}, {13,  8}, {13,  8},
        {13,  8}, {13,  8}, {13,  8}, {13,  8},
        {12,  8}, {12,  8}, {12,  8}, {12,  8},
        {12,  8}, {12,  8}, {12,  8}, {12,  8},
        {11,  8}, {11,  8}, {11,  8}, {11,  8},
        {11,  8}, {11,  8}, {11,  8}, {11,  8},
        {10,  8}, {10,  8}, {10,  8}, {10,  8},
        {10,  8}, {10,  8}, {10,  8}, {10,  8},
        { 9,  7}, { 9,  7}, { 9,  7}, { 9,  7},
        { 9,  7}, { 9,  7}, { 9,  7}, { 9,  7},
        { 9,  7}, { 9,  7}, { 9,  7}, { 9,  7},
        { 9,  7}, { 9,  7}, { 9,  7}, { 9,  7},
        { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
        { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
        { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7},
        { 8,  7}, { 8,  7}, { 8,  7}, { 8,  7}
    };
    static const MBAtab MBA_escape[] = {{12,0}};
    //-------------macroblock address increment

    //-----------------mb type-----
    typedef struct {
        uint8_t type;
        uint8_t len;
    } MBTtab;
    #define INTRA MACROBLOCK_INTRA
    #define QUANT MACROBLOCK_QUANT
    static const MBTtab MBT_I [] = {
        {INTRA|QUANT, 2}, {INTRA, 1}
    };

    #define MC MACROBLOCK_MOTION_FORWARD
    #define CODED MACROBLOCK_PATTERN
    static const MBTtab MBT_P [] = {
        {INTRA|QUANT, 6}, {CODED|QUANT, 5}, {MC|CODED|QUANT, 5}, {INTRA,    5},
        {MC,          3}, {MC,          3}, {MC,             3}, {MC,       3},
        {CODED,       2}, {CODED,       2}, {CODED,          2}, {CODED,    2},
        {CODED,       2}, {CODED,       2}, {CODED,          2}, {CODED,    2},
        {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
        {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
        {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1},
        {MC|CODED,    1}, {MC|CODED,    1}, {MC|CODED,       1}, {MC|CODED, 1}
    };

    #define FWD MACROBLOCK_MOTION_FORWARD
    #define BWD MACROBLOCK_MOTION_BACKWARD
    #define INTER MACROBLOCK_MOTION_FORWARD|MACROBLOCK_MOTION_BACKWARD
    static const MBTtab MBT_B [] = {
        {0,                 6}, {INTRA|QUANT,       6},
        {BWD|CODED|QUANT,   6}, {FWD|CODED|QUANT,   6},
        {INTER|CODED|QUANT, 5}, {INTER|CODED|QUANT, 5},
        {INTRA,       5}, {INTRA,       5},
        {FWD,         4}, {FWD,         4}, {FWD,         4}, {FWD,         4},
        {FWD|CODED,   4}, {FWD|CODED,   4}, {FWD|CODED,   4}, {FWD|CODED,   4},
        {BWD,         3}, {BWD,         3}, {BWD,         3}, {BWD,         3},
        {BWD,         3}, {BWD,         3}, {BWD,         3}, {BWD,         3},
        {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3},
        {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3}, {BWD|CODED,   3},
        {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
        {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
        {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
        {INTER,       2}, {INTER,       2}, {INTER,       2}, {INTER,       2},
        {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
        {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
        {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2},
        {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}, {INTER|CODED, 2}
    };
    #undef INTRA
    #undef QUANT
    #undef MC
    #undef CODED
    #undef FWD
    #undef BWD
    #undef INTER
    //--------------------mb type ------------

    //----------coded block pattern-------------
    typedef struct {
        int cbp;
        int len;
    } CBPtab;
    static const CBPtab CBP_7_3 [] = {
        {34, 7}, {18, 7}, {10, 7}, { 6, 7},
        {33, 7}, {17, 7}, { 9, 7}, { 5, 7},
        {63, 6}, {63, 6}, { 3, 6}, { 3, 6},
        {36, 6}, {36, 6}, {24, 6}, {24, 6},
        {62, 5}, {62, 5}, {62, 5}, {62, 5},
        { 2, 5}, { 2, 5}, { 2, 5}, { 2, 5},
        {61, 5}, {61, 5}, {61, 5}, {61, 5},
        { 1, 5}, { 1, 5}, { 1, 5}, { 1, 5},
        {56, 5}, {56, 5}, {56, 5}, {56, 5},
        {52, 5}, {52, 5}, {52, 5}, {52, 5},
        {44, 5}, {44, 5}, {44, 5}, {44, 5},
        {28, 5}, {28, 5}, {28, 5}, {28, 5},
        {40, 5}, {40, 5}, {40, 5}, {40, 5},
        {20, 5}, {20, 5}, {20, 5}, {20, 5},
        {48, 5}, {48, 5}, {48, 5}, {48, 5},
        {12, 5}, {12, 5}, {12, 5}, {12, 5},
        {32, 4}, {32, 4}, {32, 4}, {32, 4},
        {32, 4}, {32, 4}, {32, 4}, {32, 4},
        {16, 4}, {16, 4}, {16, 4}, {16, 4},
        {16, 4}, {16, 4}, {16, 4}, {16, 4},
        { 8, 4}, { 8, 4}, { 8, 4}, { 8, 4},
        { 8, 4}, { 8, 4}, { 8, 4}, { 8, 4},
        { 4, 4}, { 4, 4}, { 4, 4}, { 4, 4},
        { 4, 4}, { 4, 4}, { 4, 4}, { 4, 4},
        {60, 3}, {60, 3}, {60, 3}, {60, 3},
        {60, 3}, {60, 3}, {60, 3}, {60, 3},
        {60, 3}, {60, 3}, {60, 3}, {60, 3},
        {60, 3}, {60, 3}, {60, 3}, {60, 3}
    };
    static const CBPtab CBP_9_8 [] = {
        {0,  9}, {39, 9}, {27, 9}, {59, 9},{55, 9}, {47, 9}, {31, 9},
        {58, 8}, {58, 8}, {54, 8}, {54, 8},
        {46, 8}, {46, 8}, {30, 8}, {30, 8},
        {57, 8}, {57, 8}, {53, 8}, {53, 8},
        {45, 8}, {45, 8}, {29, 8}, {29, 8},
        {38, 8}, {38, 8}, {26, 8}, {26, 8},
        {37, 8}, {37, 8}, {25, 8}, {25, 8},
        {43, 8}, {43, 8}, {23, 8}, {23, 8},
        {51, 8}, {51, 8}, {15, 8}, {15, 8},
        {42, 8}, {42, 8}, {22, 8}, {22, 8},
        {50, 8}, {50, 8}, {14, 8}, {14, 8},
        {41, 8}, {41, 8}, {21, 8}, {21, 8},
        {49, 8}, {49, 8}, {13, 8}, {13, 8},
        {35, 8}, {35, 8}, {19, 8}, {19, 8},
        {11, 8}, {11, 8}, { 7, 8}, { 7, 8}
    };
    //----------coded block pattern-------------

    //----------motion_code-------------
    typedef struct
    {
        int code;
        int len;
    } MVtab;
    static const MVtab MV_1[] = {
        {0 , 1}
    };
    static const MVtab MV_5_3 [] = {
        { 3, 5}, { -3, 5},
        { 2, 4}, { 2, 4},{ -2, 4}, { -2, 4},
        { 1, 3}, { 1, 3},{  1, 3}, {  1, 3},
        {-1, 3}, {-1, 3},{ -1, 3}, { -1, 3},
    };
    static const MVtab MV_8_7 [] = {
        { 7, 8}, { -7, 8}, { 6, 8},{ -6, 8},{ 5, 8}, { -5, 8},
        { 4, 7}, {  4, 7}, {-4, 7},{ -4, 7},
    };
    static const MVtab MV_11_10 [] = {
        { 16,11}, { -16,11}, { 15,11}, { -15,11},
        { 14,11}, { -14,11}, { 13,11}, { -13,11},
        { 12,11}, { -12,11}, { 11,11}, { -11,11},
        { 10,10}, {  10,10}, {-10,10}, { -10,10},
        {  9,10}, {   9,10}, { -9,10}, {  -9,10},
        {  8,10}, {   8,10}, { -8,10}, {  -8,10},
    };
    //----------motion_code-------------

    //----------DMVector-------------
    typedef struct
    {
        int dmv;
        int len;
    } DMVtab;
    static const DMVtab DMV_2_1[] = {
        { 0, 1},{ 0, 1}, { 1, 2},{ -1, 2}
    };
    //----------DMVector-------------

    //----------DC size---------
    typedef struct
    {
        int size;
        int len;
    } DCtab;
    static const DCtab DC_lum_2_5 [] = {
        {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2},
        {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2},
        {0, 3}, {0, 3}, {0, 3}, {0, 3},
        {3, 3}, {3, 3}, {3, 3}, {3, 3},
        {4, 3}, {4, 3}, {4, 3}, {4, 3},
        {5, 4}, {5, 4},
        {6, 5}
    };
    static const DCtab DC_lum_5_9 [] = {
        {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, { 6, 5}, { 6, 5},
        {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, {6, 5}, { 6, 5}, { 6, 5},
        {7, 6}, {7, 6}, {7, 6}, {7, 6}, {7, 6}, {7, 6}, { 7, 6}, { 7, 6},
        {8, 7}, {8, 7}, {8, 7}, {8, 7},
        {9, 8}, {9, 8},
        {10,9}, {11,9}
    };
    static const DCtab DC_chrom_2_5 [] = {
        {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2}, {0, 2},
        {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2},
        {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2}, {2, 2},
        {3, 3}, {3, 3}, {3, 3}, {3, 3},
        {4, 4}, {4, 4},
        {5, 5}
    };
    static const DCtab DC_chrom_5_10 [] = {
        {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, { 6, 6}, { 6, 6},
        {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, {6, 6}, { 6, 6}, { 6, 6},
        {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, {7, 7}, { 7, 7}, { 7, 7},
        {8, 8}, {8, 8}, {8, 8}, {8, 8},
        {9, 9}, {9, 9},
        {10,10},{11,10}
    };
    //---------------DC size-------------

    //----------------DCT cofficients table--------
    typedef struct
    {
        int run;
        int level;
        int len;
    } DCTtab;
    //Used in table zero and one
    static const DCTtab DCT_16 [] = {
        {1, 18, 16}, {1, 17, 16}, {1, 16, 16}, {1, 15, 16},
        {6,  3, 16}, {16, 2, 16}, {15, 2, 16}, {14, 2, 16},
        {13, 2, 16}, {12, 2, 16}, {11, 2, 16}, {31, 1, 16},
        {30, 1, 16}, {29, 1, 16}, {28, 1, 16}, {27, 1, 16}
    };
    static const DCTtab DCT_15_14 [] = {
        {0, 40, 15}, {0, 39, 15}, {0, 38, 15}, {0, 37, 15},
        {0, 36, 15}, {0, 35, 15}, {0, 34, 15}, {0, 33, 15},
        {0, 32, 15}, {1, 14, 15}, {1, 13, 15}, {1, 12, 15},
        {1, 11, 15}, {1, 10, 15}, {1,  9, 15}, {1,  8, 15},

        {0, 31, 14}, {0, 31, 14}, {0, 30, 14}, {0, 30, 14},
        {0, 29, 14}, {0, 29, 14}, {0, 28, 14}, {0, 28, 14},
        {0, 27, 14}, {0, 27, 14}, {0, 26, 14}, {0, 26, 14},
        {0, 25, 14}, {0, 25, 14}, {0, 24, 14}, {0, 24, 14},
        {0, 23, 14}, {0, 23, 14}, {0, 22, 14}, {0, 22, 14},
        {0, 21, 14}, {0, 21, 14}, {0, 20, 14}, {0, 20, 14},
        {0, 19, 14}, {0, 19, 14}, {0, 18, 14}, {0, 18, 14},
        {0, 17, 14}, {0, 17, 14}, {0, 16, 14}, {0, 16, 14}
    };

    //Used in table zero
    static const DCTtab DCT_zero_13_12 [] = {
        {10, 2, 13}, {9,  2, 13}, {5,  3, 13}, {3,  4, 13},
        {2,  5, 13}, {1,  7, 13}, {1,  6, 13}, {0, 15, 13},
        {0, 14, 13}, {0, 13, 13}, {0, 12, 13}, {26, 1, 13},
        {25, 1, 13}, {24, 1, 13}, {23, 1, 13}, {22, 1, 13},

        {0, 11, 12}, {0, 11, 12}, {8,  2, 12}, {8,  2, 12},
        {4,  3, 12}, {4,  3, 12}, {0, 10, 12}, {0, 10, 12},
        {2,  4, 12}, {2,  4, 12}, {7,  2, 12}, {7,  2, 12},
        {21, 1, 12}, {21, 1, 12}, {20, 1, 12}, {20, 1, 12},
        {0,  9, 12}, {0,  9, 12}, {19, 1, 12}, {19, 1, 12},
        {18, 1, 12}, {18, 1, 12}, {1,  5, 12}, {1,  5, 12},
        {3,  3, 12}, {3,  3, 12}, {0,  8, 12}, {0,  8, 12},
        {6,  2, 12}, {6,  2, 12}, {17, 1, 12}, {17, 1, 12}
    };
    static const DCTtab DCT_zero_10 [] = {
        {16, 1, 10}, {5,  2, 10}, {0,  7, 10}, {2, 3, 10},
        {1,  4, 10}, {15, 1, 10}, {14, 1, 10}, {4, 2, 10}
    };
#define ESCAPE 120
#define EOB 110
#define XXX 119
    static const DCTtab DCT_zero_6_8 [] = {
        {ESCAPE, ESCAPE, 6}, {ESCAPE, ESCAPE, 6},{ESCAPE, ESCAPE, 6}, {ESCAPE, ESCAPE, 6},
        {2, 2, 7}, {2, 2, 7}, {9, 1, 7}, {9, 1, 7},
        {0, 4, 7}, {0, 4, 7}, {8, 1, 7}, {8, 1, 7},
        {7, 1, 6}, {7, 1, 6}, {7, 1, 6}, {7, 1, 6},
        {6, 1, 6}, {6, 1, 6}, {6, 1, 6}, {6, 1, 6},
        {1, 2, 6}, {1, 2, 6}, {1, 2, 6}, {1, 2, 6},
        {5, 1, 6}, {5, 1, 6}, {5, 1, 6}, {5, 1, 6},
        {13,1, 8}, {0, 6, 8}, {12,1, 8}, {11,1, 8},
        {3, 2, 8}, {1, 3, 8}, {0, 5, 8}, {10,1, 8}
    };
    static const DCTtab DCT_zero_other_5_1 [] = {
        {0, 3, 5},{4, 1, 5},{3, 1, 5},
        {0, 2, 4},{0, 2, 4},{2, 1, 4},{2, 1, 4},
        {1, 1, 3},{1, 1, 3},{1, 1, 3},{1, 1, 3},
        {EOB, EOB, 2},{EOB, EOB, 2},{EOB, EOB, 2},{EOB, EOB, 2},
        {EOB, EOB, 2},{EOB, EOB, 2},{EOB, EOB, 2},{EOB, EOB, 2},
        {0, 1, 2},{0, 1, 2},{0, 1, 2},{0, 1, 2},
        {0, 1, 2},{0, 1, 2},{0, 1, 2},{0, 1, 2}
    };
    static const DCTtab DCT_zero_DC_5_1 [] = {
        {0, 3, 5},{4, 1, 5},{3, 1, 5},
        {0, 2, 4},{0, 2, 4},{2, 1, 4},{2, 1, 4},
        {1, 1, 3},{1, 1, 3},{1, 1, 3},{1, 1, 3},
        {0, 1, 1},{0, 1, 1},{0, 1, 1},{0, 1, 1},
        {0, 1, 1},{0, 1, 1},{0, 1, 1},{0, 1, 1},
        {0, 1, 1},{0, 1, 1},{0, 1, 1},{0, 1, 1},
        {0, 1, 1},{0, 1, 1},{0, 1, 1},{0, 1, 1},
    };

    //used in table one
    static const DCTtab DCT_one_13_12 [] = {
        { 10,   2, 13},   {  9,  2, 13},   { 5,   3, 13},   {  3,   4, 13},
        {  2,   5, 13},   {  1,  7, 13},   { 1,   6, 13},   {XXX, XXX, 13},
        {XXX, XXX, 13},   {XXX,XXX, 13},   {XXX,XXX, 13},   { 26,   1, 13},
        { 25,   1, 13},   { 24,  1, 13},   { 23,  1, 13},   { 22,   1, 13},

        {XXX, XXX, 12},  {XXX, XXX, 12},  {  8,   2, 12},  {  8,   2, 12},
        {  4,   3, 12},  {  4,   3, 12},  {XXX, XXX, 12},  {XXX, XXX, 12},
        {XXX, XXX, 12},  {XXX, XXX, 12},  {  7,   2, 12},  {  7,   2, 12},
        { 21,   1, 12},  { 21,   1, 12},  { 20,   1, 12},  { 20,   1, 12},
        {XXX, XXX, 12},  {XXX, XXX, 12},  { 19,   1, 12},  { 19,   1, 12},
        { 18,   1, 12},  { 18,   1, 12},  {XXX, XXX, 12},  {XXX, XXX, 12},
        {  3,   3, 12},  {  3,   3, 12},  {XXX, XXX, 12},  {XXX, XXX, 12},
        {  6,   2, 12},  {  6,   2, 12},  { 17,   1, 12},  {  17,  1, 12},
    };
    static const DCTtab DCT_one_10_9 [] = {
        {5, 2,  9}, {5,  2,  9}, {14, 1, 9}, {14, 1, 9},
        {2, 4, 10}, {16, 1, 10}, {15, 1, 9}, {15, 1, 9},
    };
    static const DCTtab DCT_one_6_8 [] = {
        {ESCAPE, ESCAPE, 6}, {ESCAPE, ESCAPE, 6},{ESCAPE, ESCAPE, 6},{ESCAPE, ESCAPE, 6},
        {7, 1, 7},  {7, 1, 7},  {8, 1, 7},  {8, 1, 7},
        {6, 1, 7},  {6, 1, 7},  {2, 2, 7},  {2, 2, 7},
        {0, 7, 6},  {0, 7, 6},  {0, 7, 6},  {0, 7, 6},
        {0, 6, 6},  {0, 6, 6},  {0, 6, 6},  {0, 6, 6},
        {4, 1, 6},  {4, 1, 6},  {4, 1, 6},  {4, 1, 6},
        {5, 1, 6},  {5, 1, 6},  {5, 1, 6},  {5, 1, 6},
        {1, 5, 8},  {11,1, 8},  {0, 11,8},  {0,10, 8},
        {13,1, 8},  {12,1, 8},  {3, 2, 8},  {1, 4, 8},
    };
    static const DCTtab DCT_one_2_5 [] = {
        {2, 1, 5},     {1, 2, 5},     {3, 1, 5},
        {1, 1, 3},     {1, 1, 3},     {1, 1, 3}, {1, 1, 3},
        {EOB, EOB, 4}, {EOB, EOB, 4}, {0, 3, 4}, {0, 3, 4},
        {0, 1, 2},     {0, 1, 2},     {0, 1, 2}, {0, 1, 2},
        {0, 1, 2},     {0, 1, 2},     {0, 1, 2}, {0, 1, 2}
    };
    static const DCTtab DCT_one_3_8 [] = {
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},
        {0, 2, 3}, {0, 2, 3}, {0, 2, 3}, {0, 2, 3},

        {0, 4, 5}, {0, 4, 5}, {0, 4, 5}, {0, 4, 5},
        {0, 4, 5}, {0, 4, 5}, {0, 4, 5}, {0, 4, 5},
        {0, 5, 5}, {0, 5, 5}, {0, 5, 5}, {0, 5, 5},
        {0, 5, 5}, {0, 5, 5}, {0, 5, 5}, {0, 5, 5},

        {9, 1, 7},  {9, 1, 7},  {1,  3, 7}, {1,  3, 7},
        {10,1, 7},  {10,1, 7},  {0,  8, 7}, {0,  8, 7},
        {0, 9, 7},  {0, 9, 7},  {0, 12, 8}, {0, 13, 8},
        {2, 3, 8},  {4, 2, 8},  {0, 14, 8}, {0, 15, 8}
    };
    //--------------DCT cofficients table--------
    const MBAtab* readMbAddress(uint bits_32);
    const MBTtab* readMbType_I(uint bits_32);
    const MBTtab* readMbType_B(uint bits_32);
    const MBTtab *readMbType_P(uint bits_32);
    const CBPtab* readMbPattern(uint bits_32);
    const MVtab* readMVCode(uint bits_32);
    const DMVtab* readDMVector(uint bits_32);
    const DCtab* readLumDCSize(uint bits_32);
    const DCtab* readChromDCSize(uint bits_32);
    const DCTtab* readDctTableZero(uint bits_32, bool isDC);
    const DCTtab* readDctTableOne(uint bits_32);

    void test();

}

#endif // VLC_H
