#ifndef MPEGSLICE_H
#define MPEGSLICE_H

#include "streamreader.h"
#include "mpegcontext.h"
class MPEGSlice
{
    class Macroblock
    {
        public:
            int macroblock_escape;
            int macroblock_address_increment;

    };

    class MotionVector
    {

    };

    class Block
    {

    };

public:
    MPEGSlice();
    bool parse(StreamReader * reader,MPEGContext * ctx);
    bool parseMacroblock(StreamReader * reader, Macroblock * mb);
    bool parseMacroModes(StreamReader * reader, Macroblock * mb);
    bool parseMacroVectorS(StreamReader * reader, Macroblock * mb);
    bool parseMacroVector(StreamReader * reader, Macroblock * mb);
    bool parseCodedBlockPattern(StreamReader * reader, Macroblock * mb);
    bool parseBlock(StreamReader * reader, Block * mb);

private:
    QList<Macroblock *> mbs;

    int slice_vertical_position_extension;
    int priority_breakpoint;
    int quantiser_scale_mode;
    int intra_slice_flag;
    int intra_slice;
    int reserved_bits;
    int extra_bit_slice;
    int extra_information_slice;

};

#endif // MPEGSLICE_H
