#ifndef MPEGCONTEXT_H
#define MPEGCONTEXT_H


class MPEGContext
{
public:
    MPEGContext();
    bool reset();

    int vertical_size = 0;
    int horizontal_size = 0;
    bool seq_scala_ext_present = false;
    int scalable_mode;
};

#endif // MPEGCONTEXT_H
