#include "mpeggop.h"

MPEGGOP::MPEGGOP()
{

}

bool MPEGGOP::parseHeader(StreamReader * reader)
{
    hdr.parse(reader);
    return true;
}

bool MPEGGOP::parseUserNExtData(StreamReader * reader)
{
    //Notice:GOP don't follow any extension_data immediately, so we ignore this
    //extension_start_code and only deal with user_data_start_code;

    if(MPEGConst::USER_DATA_START_CODE == reader->getNBits_int(32))
    {
        reader->readNBits_int(32);      //user_data_start_code
        while( !reader->nextbits(24,MPEGConst::START_CODE_PREFIX))
        {
            userdata.append( reader->readNBits_int(8) );
        }
    }

    return true;
}

MPEGPicture* MPEGGOP::getPicAt(int pic_index)
{
    return this->pics.at(pic_index);
}

bool MPEGGOP::addNewPic()
{
    this->pics.append(new MPEGPicture());

    return true;
}

