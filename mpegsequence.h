#ifndef MPEGSEQUENCE_H
#define MPEGSEQUENCE_H

#include "./bitstream/sequenceheader.h"
#include "./bitstream/sequenceextension.h"
#include "./bitstream/sequencedisplayextension.h"
#include "./bitstream/sequencescalableextension.h"

#include "streamreader.h"
#include "mpeggop.h"
#include "mpegcontext.h"

#include <QList>
class MPEGSequence
{
public:
    MPEGSequence();
    bool parseUserNExtData(StreamReader * reader,MPEGContext * ctx);
    bool parseUserData(StreamReader * reader);
    bool parseHeader(StreamReader * reader,MPEGContext * ctx);
    bool parseExt(StreamReader * reader,MPEGContext * ctx);
    bool parseSequenceExt(StreamReader * reader);
    bool parseDisplayExt(StreamReader * reader);
    bool parseScalableExt(StreamReader * reader);

    MPEGGOP *getGOPAt(int gop_index);
    bool addNewGOP();

//variable
    SequenceHeader hdr;
    SequenceExtension ext;
    SequenceDisplayExtension displayext;
    SequenceScalableExtension scalext;
    QList<int> userdata;

private:
    QList<MPEGGOP *> gops;
};

#endif // MPEGSEQUENCE_H
