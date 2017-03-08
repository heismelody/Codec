#include "quantmatrixextension.h"

QuantMatrixExtension::QuantMatrixExtension()
{

}

bool QuantMatrixExtension::parse(StreamReader * reader)
{
    reader->readNBits_int(32);
    reader->readNBits_int(4);
    load_intra_quantiser_matrix                 = reader->readNBits_int(1);
    if(load_intra_quantiser_matrix)
    {
        for(int i = 0; i < 64; i++)
        {
            intra_quantiser_matrix[i]           = reader->readNBits_int(8);
        }
    }
    load_non_intra_quantiser_matrix             = reader->readNBits_int(1);
    if(load_non_intra_quantiser_matrix)
    {
        for(int i = 0; i < 64; i++)
        {
            non_intra_quantiser_matrix[i]       = reader->readNBits_int(8);
        }
    }
    load_chroma_intra_quantiser_matrix          = reader->readNBits_int(1);
    if(load_chroma_intra_quantiser_matrix)
    {
        for(int i = 0; i < 64; i++)
        {
            chroma_intra_quantiser_matrix[i]    = reader->readNBits_int(8);
        }
    }
    load_chroma_non_intra_quantiser_matrix      = reader->readNBits_int(1);
    if(chroma_non_intra_quantiser_matrix)
    {
        for(int i = 0; i < 64; i++)
        {
            chroma_non_intra_quantiser_matrix[i]= reader->readNBits_int(8);
        }
    }
    reader->next_start_code();

    return true;
}
