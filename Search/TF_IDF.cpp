#include "TF_IDF.h"

TF_IDF::TF_IDF(size_t dl_avg, size_t n) {
    this->bm25.SetDlAvg(dl_avg);
    this->n = n;
}

TF_IDF& TF_IDF::operator=(TF_IDF&& other) noexcept {
    if (this != &other) {
        bm25 = std::move(other.bm25);
        n = other.n;
        other.n = 0;
    }
    return *this;
}

double TF_IDF::Score(size_t tf, size_t dl, size_t df) {
    double bm = this->bm25.Score(tf, dl);
    double res = (double)(bm * (double)log((double)((double)(this->n) / (double)(df))));
    return res;
}