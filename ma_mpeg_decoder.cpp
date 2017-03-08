#include "ma_mpeg_decoder.h"

using namespace MpegUtil;
using namespace VLC;

void ma_mpeg_decoder::sl_open_file()
{
    emit this->sg_need_file();
}

void ma_mpeg_decoder::sl_mpg_file_opened(QString& filename)
{
    this->filename = filename;
    main_procdure();
}

ma_mpeg_decoder::ma_mpeg_decoder()
    :fileloader(new FileLoader)
{
    connect(this,SIGNAL(sg_need_file()),
            this->fileloader,SLOT(sl_open_file()));
    connect(this->fileloader,SIGNAL(sg_opened_mpg_file(QString &)),
            this,SLOT(sl_mpg_file_opened(QString &)));
}

ma_mpeg_decoder::MPEG_CODE
ma_mpeg_decoder::main_procdure()
{
    StreamReader* reader = new StreamReader(filename);

//    //qDebug() << (reader->readNBits_int(12));
//    SequenceExtension seqExt;

//    seqExt.parse(reader);
    video_sequences(reader);
}

ma_mpeg_decoder::MPEG_CODE
ma_mpeg_decoder::video_sequences(StreamReader * reader)
{
    int startcode = 0;
    QList<MPEGSequence> seq;
    MPEGContext * ctx = new MPEGContext();
    int seq_i = 0;
    int gop_i = -1;
    int pic_i = 0;

    seq.append(MPEGSequence());
    //if(startcode != MPEGConst::SEQUENCE_HEADER_CODE) { return MPEG_SEQ_HDR_NOT_FOUND; }
    while(reader->skip_to_next_start_code() != 0x000001b3)
    {
        reader->readNBits_int(32);
    }
    seq[0].parseHeader(reader,ctx);
    if(  reader->nextbits(32,MPEGConst::EXTENSION_START_CODE) )
    {
        seq[0].parseExt(reader,ctx);
        do
        {
            seq[seq_i].parseUserNExtData(reader,ctx);
            do
            {
                if( reader->nextbits(32,MPEGConst::GROUP_START_CODE) )
                {
                    gop_i++;
                    pic_i = 0;
                    seq[seq_i].addNewGOP();
                    seq[seq_i].getGOPAt(gop_i)->parseHeader(reader);
                    seq[seq_i].getGOPAt(gop_i)->parseUserNExtData(reader);
                }
                seq[seq_i].getGOPAt(gop_i)->addNewPic();
                seq[seq_i].getGOPAt(gop_i)->getPicAt(pic_i)->parseHeader(reader);
                seq[seq_i].getGOPAt(gop_i)->getPicAt(pic_i)->parseCodingExt(reader);
                seq[seq_i].getGOPAt(gop_i)->getPicAt(pic_i)->parseUserNExtData(reader);
                seq[seq_i].getGOPAt(gop_i)->getPicAt(pic_i)->parseData(reader,ctx);
                qDebug() << "V size:" << (ctx->vertical_size);
                qDebug() << "h size:" << (ctx->horizontal_size);
                pic_i++;
                break;
            }
            while( reader->nextbits(32,MPEGConst::PICTURE_START_CODE)
                   || reader->nextbits(32,MPEGConst::GROUP_START_CODE) );
            if( !reader->nextbits(32,MPEGConst::SEQUENCE_END_CODE) )
            {
                seq_i++;
                seq.append(MPEGSequence());
                //initContext();
                seq[seq_i].parseHeader(reader,ctx);
                seq[seq_i].parseExt(reader,ctx);
            }
        }
        while( !reader->nextbits(32,MPEGConst::SEQUENCE_END_CODE) );
        qDebug() << "sdfs" << seq_i;
    }
    else
    {
        //ISO/ IEC 11172-2
    }
//    sequence_end_code();
}
