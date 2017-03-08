#include "mpegpicture.h"

MPEGPicture::MPEGPicture()
{

}

bool MPEGPicture::parseHeader(StreamReader * reader)
{
    hdr.parse(reader);
    return true;
}

bool MPEGPicture::parseUserNExtData(StreamReader * reader)
{
    while ( reader->nextbits(32,MPEGConst::EXTENSION_START_CODE)
            || reader->nextbits(32,MPEGConst::USER_DATA_START_CODE) )
    {
        if( reader->nextbits(32,MPEGConst::EXTENSION_START_CODE) )
        {
            parseExt(reader);
        }
        if( reader->nextbits(32,MPEGConst::USER_DATA_START_CODE) )
        {
            parseUserData(reader);
        }
    }
}

bool MPEGPicture::parseUserData(StreamReader * reader)
{
    reader->readNBits_int(32);      //user_data_start_code
    while( !reader->nextbits(24,MPEGConst::START_CODE_PREFIX))
    {
        userdata.append(reader->readNBits_int(8));
    }

    return true;
}

bool MPEGPicture::parseExt(StreamReader * reader)
{
    int extnsionId = reader->getNBits_int(36) & 0xF;
    if(extnsionId == MPEGConst::QUANT_MATRIX_EXTENSION_ID)
    {
        parseQuantExt(reader);
    }
    if(extnsionId == MPEGConst::PICTURE_DISPLAY_EXTENSION_ID)
    {
        parseDisplayExt(reader);
    }
    if(extnsionId == MPEGConst::PICTURE_SPATIAL_SCALABLE_EXTENSION_ID)
    {
        parseSpatialExt(reader);
    }
    if(extnsionId == MPEGConst::PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID)
    {
        parseTemporalExt(reader);
    }
    if(extnsionId == MPEGConst::PICTURE_CODING_EXTENSION_ID)
    {
        parseCodingExt(reader);
    }

    return true;
}

bool MPEGPicture::parseDisplayExt(StreamReader * reader)
{
    displayext.parse(reader);
    return true;
}

bool MPEGPicture::parseCodingExt(StreamReader * reader)
{
    codeext.parse(reader);
    return true;
}

bool MPEGPicture::parseSpatialExt(StreamReader * reader)
{
    spatialext.parse(reader);
    return true;
}

bool MPEGPicture::parseTemporalExt(StreamReader * reader)
{
    temporalext.parse(reader);
    return true;
}

bool MPEGPicture::parseQuantExt(StreamReader * reader)
{
    quanmatrixext.parse(reader);
    return true;
}

bool MPEGPicture::parseData(StreamReader * reader,MPEGContext * ctx)
{
    int i = -1;
    do
    {
        i++;
        slices.append(new MPEGSlice());
        slices.at(i)->parse(reader,ctx);
    }
    while ( reader->getNBits_int(32) != 0x00000100);
//    while ( (reader->getNBits_int(32) >= MPEGConst::SLICE_START_CODE_FIRST)
//            && (reader->getNBits_int(32) <= MPEGConst::SLICE_START_CODE_LAST) );
    qDebug() << "slices i:" << i;

    return true;
}

