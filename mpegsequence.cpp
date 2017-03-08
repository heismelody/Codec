#include "mpegsequence.h"

MPEGSequence::MPEGSequence()
{

}

bool MPEGSequence::parseHeader(StreamReader * reader,MPEGContext * ctx)
{
    hdr.parse(reader);
    ctx->vertical_size = hdr.vertical_size_value;
    ctx->horizontal_size = hdr.horizontal_size_value;
    return true;
}

bool MPEGSequence::parseUserNExtData(StreamReader * reader,MPEGContext * ctx)
{
    while ( reader->nextbits(32,MPEGConst::EXTENSION_START_CODE)
            || reader->nextbits(32,MPEGConst::USER_DATA_START_CODE) )
    {
        if( reader->nextbits(32,MPEGConst::EXTENSION_START_CODE) )
        {
            parseExt(reader,ctx);
        }
        if( reader->nextbits(32,MPEGConst::USER_DATA_START_CODE) )
        {
            parseUserData(reader);
        }
    }

    return true;
}

bool MPEGSequence::parseExt(StreamReader * reader,MPEGContext * ctx)
{
    int extnsionId = reader->getNBits_int(36) & 0xF;

    if(extnsionId == MPEGConst::SEQUENCE_DISPLAY_EXTENSION_ID)
    {
        parseDisplayExt(reader);
    }
    if(extnsionId == MPEGConst::SEQUENCE_SCALABLE_EXTENSION_ID)
    {
        ctx->seq_scala_ext_present = true;
        parseScalableExt(reader);
    }
    if(extnsionId == MPEGConst::SEQUENCE_EXTENSION_ID)
    {
        parseSequenceExt(reader);
        ctx->vertical_size = hdr.vertical_size_value + (ext.vertical_size_extension << 12);
        ctx->horizontal_size = hdr.horizontal_size_value + (ext.horizontal_size_extension << 12);
    }

    return true;
}

bool MPEGSequence::parseSequenceExt(StreamReader * reader)
{
    ext.parse(reader);
    return true;
}

bool MPEGSequence::parseDisplayExt(StreamReader * reader)
{
    displayext.parse(reader);
    return true;
}

bool MPEGSequence::parseScalableExt(StreamReader * reader)
{
    scalext.parse(reader);
    return true;
}

bool MPEGSequence::parseUserData(StreamReader * reader)
{
    reader->readNBits_int(32);      //user_data_start_code
    while( !reader->nextbits(24,MPEGConst::START_CODE_PREFIX) )
    {
        userdata.append(reader->readNBits_int(8));
    }

    return true;
}

MPEGGOP* MPEGSequence::getGOPAt(int gop_index)
{
    return this->gops.at(gop_index);
}

bool MPEGSequence::addNewGOP()
{
    this->gops.append(new MPEGGOP());

    return true;
}
