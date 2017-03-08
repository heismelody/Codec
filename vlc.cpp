#include "vlc.h"

namespace VLC
{
    const MBAtab* readMbAddress(uint bits_32)
    {
        const MBAtab* mba;

        if (bits_32 >= 0x10000000)
        {
            mba = MBA_5_1 + (UBITS (bits_32, 5) - 2);
        }
        else if (bits_32 >= 0x03000000)
        {
            mba = MBA_11_7 + (UBITS (bits_32, 11) - 24);
        }
        else
        {
            mba = MBA_escape;   //  macroblock_escape   UBITS (bit_buf, 12);
        }

        return mba;
    }

    const MBTtab* readMbType_I(uint bits_32)
    {
        const MBTtab* tab;

        tab = MBT_I + UBITS (bits_32, 1);

        return tab;
    }

    const MBTtab* readMbType_P(uint bits_32)
    {
        const MBTtab* tab;

        tab = MBT_P + UBITS (bits_32, 5);

        return tab;
    }

    const MBTtab* readMbType_B(uint bits_32)
    {
        const MBTtab* tab;

        tab = MBT_B + UBITS (bits_32, 6);

        return tab;
    }

    const CBPtab* readMbPattern(uint bits_32)
    {
        const CBPtab* tab;

        if (bits_32 >= 0x20000000)
        {
            tab = CBP_7_3 + (UBITS (bits_32, 7) - 16);
        }
        else
        {
            tab = CBP_9_8 + UBITS (bits_32, 9);
        }

        return tab;
    }

    const MVtab *readMVCode(uint bits_32)
    {
        const MVtab * tab;

        if (bits_32 & 0x80000000)
        {
            tab = MV_1;
        }
        else if (bits_32 >= 0x10000000)
        {
            tab = MV_5_3 + (UBITS (bits_32, 5) - 2);
        }
        else if (bits_32 >= 0x06000000)
        {
            tab = MV_8_7 + (UBITS (bits_32, 8) - 6);
        }
        else
        {
            tab = MV_11_10 + (UBITS (bits_32, 11) - 24);
        }

        return tab;
    }

    const DMVtab* readDMVector(uint bits_32)
    {
        const DMVtab* tab;

        tab = DMV_2_1 + UBITS (bits_32, 2);

        return tab;
    }

    const DCtab* readLumDCSize(uint bits_32)
    {
        const DCtab* tab;

        if (bits_32 < 0xf8000000)
        {
            tab = DC_lum_2_5 + UBITS (bits_32, 5);
        }
        else
        {
            tab = DC_lum_5_9 + (UBITS (bits_32, 9) - 0x1e0);
        }

        return tab;
    }

    const DCtab* readChromDCSize(uint bits_32)
    {
        const DCtab* tab;

        if (bits_32 < 0xf8000000)
        {
            tab = DC_chrom_2_5 + UBITS (bits_32, 5);
        }
        else
        {
            tab = DC_chrom_5_10 + (UBITS (bits_32, 10) - 0x3e0);
        }

        return tab;
    }

    const DCTtab* readDctTableZero(uint bits_32,bool isDC)
    {
        const DCTtab* tab;

        if (bits_32 >= 0x28000000)
        {

            if(isDC)
            {
                tab = DCT_zero_DC_5_1 + (UBITS (bits_32, 5) - 5);
            }
            else
            {
                tab = DCT_zero_other_5_1 + (UBITS (bits_32, 5) - 5);
            }
        }
        else if (bits_32 >= 0x04000000)
        {
            tab = DCT_zero_6_8 + (UBITS (bits_32, 8) - 4);
        }
        else if (bits_32 >= 0x02000000)
        {
            tab = DCT_zero_10 + (UBITS (bits_32, 10) - 8);
        }
        else if (bits_32 >= 0x00800000)
        {
            tab = DCT_zero_13_12 + (UBITS (bits_32, 13) - 16);
        }
        else if (bits_32 >= 0x00200000)
        {
            tab = DCT_15_14 + (UBITS (bits_32, 15) - 16);
        }
        else
        {
            tab = DCT_16 + (UBITS (bits_32, 16) - 16);
        }

        return tab;
    }

    const DCTtab* readDctTableOne(uint bits_32)
    {
        const DCTtab* tab;

        if (bits_32 >= 0xC0000000)
        {
            tab = DCT_one_3_8 + (UBITS (bits_32, 8) - 192);
        }
        else if (bits_32 >= 0x28000000)
        {
            tab = DCT_one_2_5 + (UBITS (bits_32, 5) - 5);
        }
        else if (bits_32 >= 0x04000000)
        {
            tab = DCT_one_6_8 + (UBITS (bits_32, 8) - 4);
        }
        else if (bits_32 >= 0x02000000)
        {
            tab = DCT_one_10_9 + (UBITS (bits_32, 10) - 8);
        }
        else if (bits_32 >= 0x00800000)
        {
            tab = DCT_one_13_12 + (UBITS (bits_32, 13) - 16);
        }
        else if (bits_32 >= 0x00200000)
        {
            tab = DCT_15_14 + (UBITS (bits_32, 15) - 16);
        }
        else
        {
            tab = DCT_16 + (UBITS (bits_32, 16) - 16);
        }

        return tab;
    }

    void test()
    {
        const DCTtab* tab;
        uint bit_buf;

        bit_buf = 0xF1FFFFFF;
        tab = readDctTableOne(bit_buf);
        qDebug() << ":9:" << tab->run;
        qDebug() << ":1:" << tab->level;
    }

}
