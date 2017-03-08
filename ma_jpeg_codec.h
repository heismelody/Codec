#ifndef MA_JPEG_CODEC_H
#define MA_JPEG_CODEC_H


#include <QObject>

QT_BEGIN_NAMESPACE
class QFile;
QT_END_NAMESPACE
typedef struct Decode Decode;

class ma_jpeg_codec : public QObject
{
    Q_OBJECT
public:
    //---static variable----
    static const uint8_t zigzad_order[64];
    static const uint8_t zigzad_reverse_order[64];
    enum JPEG_CODE
    {
      JPEG_OK = 0,
      JPEG_SOI_ERROR,
      JPEG_SOI_NOT_FOUND,
      JPEG_EOI_NOT_FOUND,
      JPEG_ONLY_SUPPORT_THREE_COM,
      JPEG_ECS_OK,
    };
    //quantization table
    static const uint8_t default_quantization_table_luma[64];
    static const uint8_t default_quantization_table_chroma[64];
    //huffman table,DC difference coding
    static const uint8_t default_huffman_table_dc_len_luma[16];
    static const uint8_t default_huffman_table_dc_len_chroma[16];
    static const uint8_t default_huffman_table_dc_val_luma[12];
    static const uint8_t default_huffman_table_dc_val_chroma[12];
    //huffman table,AC coefficient coding
    static const uint8_t default_huffman_table_ac_len_luma[16];
    static const uint8_t default_huffman_table_ac_len_chroma[16];
    static const uint8_t default_huffman_table_ac_val_luma[];
    static const uint8_t default_huffman_table_ac_val_chroma[];
    enum H_FACTOR{
        H_ONE = 1,
        H_TWO = 2,
        H_THREE = 3,
        H_FOUR = 4
    };

    enum V_FACTOR{
        V_ONE = 1,
        V_TWO = 2,
        V_THREE = 3,
        V_FOUR = 4
    };

    enum QTABLE_ID{
        QTABLE_ZERO = 0,
        QTABLE_ONE = 1,
        QTABLE_TWO = 2,
        QTABLE_THREE = 3
    };

    enum HUFFCLASS{
        HUFFCLASS_DC = 0,
        HUFFCLASS_AC = 1
    };

    enum HUFFTABLE_ID{
        HUFF_ZERO = 0,
        HUFF_ONE = 1
    };

    struct FrameComponent{
        uint8_t id;
        H_FACTOR h;
        V_FACTOR v;
        QTABLE_ID qtable;
    };

    struct ScanComponent{
        uint8_t id;
        HUFFTABLE_ID DC_dst_id;
        HUFFTABLE_ID AC_dst_id;
    };
    static struct Decode{
        FrameComponent * frame_components;
        ScanComponent * scan_components;

        uint8_t scan_component_num;
        uint8_t frame_component_num;

        uint16_t width,height;  //x,y smaples (pixel)
        uint8_t * decodedata;
        uint16_t qtable[4][64];

        //---need to calculate---
            uint8_t * huffsize[2][2];           //[dc/ac][table_id]
            uint16_t * huffcode[2][2];           //dc --- 0, ac ---- 1

            //used in decode
            int32_t MAXCODE[2][2][16];
            int32_t MINCODE[2][2][16];
            int32_t valptr[2][2][16];

            int count;  //byte count when read bit by bit.
            bool is_stuff = false; //0xff00 -> 0xff
            uint8_t decoded_temp_val;

            int preDC[3] = {0,0,0};
            int comp_count = 0;
            int x_MCU_count = 0;
            int y_MCU_count = 0;
            int hor_MCU_num = 0;
            int ver_MCU_num = 0;
            int hor_MCU_count = 0;
            int ver_MCU_count = 0;

            int block8x8_per_comp[3];
            int16_t block8X8[12][64];
            int MCU_comp_id[16];
        //---need to calculate---

        //contained in marker
        uint8_t bits[2][2][16];
        uint8_t * huffval[2][2];

        uint8_t sample_precision = 8;
    } * decod;
    //---static function----
    static void jpeg_encode(QByteArray *src,QByteArray *dst);
    static void jpeg_fdct(float *data);

    static JPEG_CODE jpeg_decode(QByteArray *src,uint8_t *dst);
    static JPEG_CODE jpeg_parse(QByteArray *src,Decode *dst);
    static void jpeg_entropy_decode(Decode *dst);
    static JPEG_CODE jpeg_reserve_ZIG(int16_t *src, int *des);
    static JPEG_CODE jpeg_dequantize(int16_t * block,uint16_t * qtable);
    static void jpeg_idct(long *src, long *dst);
    static void IDCT2(int *data);
    //static JPEG_CODE decode8x8(uint8_t * result, int16_t YBlock[], int i, int16_t CbBlock[], int j, int16_t CrBlock[], int k, Decode *dec);
    static JPEG_CODE decode8x8(uint8_t * result, int16_t block[12][64], int i, int j, int k, Decode *dec);

    static JPEG_CODE jpeg_parse_tableORmis(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_qtable(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_hufftable(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_arithtable(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_restart(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_comment(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_tableORmis_app(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_SOI(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_DHP(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_EOI(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_EXP(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_SOF(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_SOS(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_ECS(char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_parse_DNL(char **pCur, Decode *dec);

    static void jpeg_decode_MCU(char ** pCur, Decode *dec);
    static JPEG_CODE jpeg_decode_DC(HUFFTABLE_ID id,char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_decode_AC(HUFFTABLE_ID id,char ** pCur,Decode *dec);
    static void jpeg_decode_decodeTableGeneration(HUFFCLASS huffclass,HUFFTABLE_ID id,Decode *dec);
    static void jpeg_decode_huffsizeGeneration(HUFFCLASS huffclass,HUFFTABLE_ID id,Decode *dec);
    static void jpeg_decode_huffcodeGeneration(HUFFCLASS huffclass,HUFFTABLE_ID id,Decode *dec);
    static int16_t jpeg_decode_ZZ(uint8_t size, char ** pCur, Decode *dec);
    static uint8_t jpeg_decode_NEXTBYTE(char *pCur);
    static int8_t jpeg_decode_NEXTBIT(char ** pCur,Decode *dec);
    static uint16_t jpeg_decode_RECEIVE(uint8_t ssss,char ** pCur,Decode *dec);
    static JPEG_CODE jpeg_decode_DECODE(HUFFCLASS huffclass,HUFFTABLE_ID id,char ** pCur,Decode *dec);
    static int16_t jpeg_decode_EXTEND(uint16_t diff,uint8_t t);

public:
    ma_jpeg_codec();       //constructor
    void decode();
    void encode();
private:

private:
    QByteArray *file;        //file to handle
    QByteArray *decodedFile;
    QByteArray *encodedFile;
public:
    //setter
    void setFile(QByteArray *file);

    //getter

signals:
    //void sg_encode_complete(uint *data);
    void sg_decode_complete(uint8_t *data);

public slots:
    void sl_in_file(QByteArray *file);
};
#endif // MA_JPEG_CODEC_H
