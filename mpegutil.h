#ifndef MPEGUTIL_H
#define MPEGUTIL_H

#include <QtWidgets>

namespace MpegUtil
{
    bool equalQbitQbit(QBitArray b1, QBitArray b2);
    bool equalQbitInt(QBitArray b1, int b2);
    int truncate(int src, int size, int n);
}

#endif // MPEGUTIL_H
