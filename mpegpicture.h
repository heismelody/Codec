#ifndef MPEGPICTURE_H
#define MPEGPICTURE_H

#include "./bitstream/pictureheader.h"
#include "./bitstream/picturedisplayextension.h"
#include "./bitstream/picturecodingextension.h"
#include "./bitstream/picturespatialscalableextension.h"
#include "./bitstream/picturetemporalscalableextension.h"
#include "./bitstream/quantmatrixextension.h"

#include "streamreader.h"
#include "mpegslice.h"
#include "mpegconst.h"

class MPEGPicture
{
public:
    MPEGPicture();
    bool parseUserNExtData(StreamReader * reader);
    bool parseUserData(StreamReader * reader);
    bool parseHeader(StreamReader * reader);
    bool parseExt(StreamReader * reader);
    bool parseDisplayExt(StreamReader * reader);
    bool parseCodingExt(StreamReader * reader);
    bool parseSpatialExt(StreamReader * reader);
    bool parseTemporalExt(StreamReader * reader);
    bool parseQuantExt(StreamReader * reader);

    bool parseData(StreamReader * reader,MPEGContext * ctx);

//var
    PictureHeader hdr;
    PictureDisplayExtension displayext;
    PictureCodingExtension codeext;
    PictureSpatialScalableExtension spatialext;
    PictureTemporalScalableExtension temporalext;
    QuantMatrixExtension quanmatrixext;

    QList<MPEGSlice *> slices;
    QList<int> userdata;
};

#endif // MPEGPICTURE_H
