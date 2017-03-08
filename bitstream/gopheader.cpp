#include "gopheader.h"

GOPHeader::GOPHeader()
{

}

bool GOPHeader::parse(StreamReader * reader)
{
    group_start_code = reader->readNBits_int(32);
    time_code        = reader->readNBits_int(25);
    closed_gop       = reader->readNBits_int(1);
    broken_link      = reader->readNBits_int(1);
    reader->next_start_code();

    qDebug() << "------------------gop header------";
    qDebug() << "time_code" << time_code;
    qDebug() << "closed_gop" << closed_gop;
    qDebug() << "broken_link" << broken_link;
}
