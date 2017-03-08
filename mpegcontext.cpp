#include "mpegcontext.h"

MPEGContext::MPEGContext()
{

}

bool MPEGContext::reset()
{
    vertical_size = 0;
    horizontal_size = 0;
    seq_scala_ext_present = false;
    scalable_mode = 0;
}
