#include "mpegutil.h"

namespace MpegUtil
{
    bool equalQbitQbit(QBitArray b1, QBitArray b2)
    {
        if(b1.size() == b2.size())
        {
            for(int i = 0; i < b1.size(); i++)
            {
                if(b1[i] != b2[i]) { return false; }
            }
            return true;
        }
        else
        {
            return false;
        }
    }

    bool equalQbitInt(QBitArray b1, int b2)
    {
        for(int i = 0; i < b1.size(); i++)
        {
            if( ((b2 >> (b1.size() - i - 1)) & 0x1) != b1[i] )
            {
                return false;
            }
        }
        return true;
    }

    int truncate(int src,int size,int n)
    {
        return (src >> (size - n));
    }
}
