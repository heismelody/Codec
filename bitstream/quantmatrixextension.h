#ifndef QUANTMATRIXEXTENSION_H
#define QUANTMATRIXEXTENSION_H

#include "basebitstream.h"
#include "../streamreader.h"

class QuantMatrixExtension : public BaseBitStream
{
public:
    int extension_start_code_identifier;
    int load_intra_quantiser_matrix;
    int intra_quantiser_matrix[64];
    int load_non_intra_quantiser_matrix;
    int non_intra_quantiser_matrix[64];
    int load_chroma_intra_quantiser_matrix;
    int chroma_intra_quantiser_matrix[64];
    int load_chroma_non_intra_quantiser_matrix;
    int chroma_non_intra_quantiser_matrix[64];

public:
    QuantMatrixExtension();
    bool parse(StreamReader * reader);
};

#endif // QUANTMATRIXEXTENSION_H
