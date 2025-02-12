#pragma once

#include <math.h>

#include "BM25.h"

class TF_IDF {
public:
    TF_IDF() = default;
    TF_IDF(size_t dl_avg, size_t n);
    double Score(size_t tf, size_t dl, size_t df);

    TF_IDF& operator=(TF_IDF&& other) noexcept;
private:
    BM25 bm25;
    size_t n;
};