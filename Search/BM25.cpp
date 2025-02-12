#include "BM25.h"

BM25& BM25::operator=(BM25&& other) noexcept {
    if (this != &other) {
        dl_avg_ = other.dl_avg_;
        other.dl_avg_ = 0;
    }
    return *this;
}

double BM25::Score(size_t tf, size_t dl) {
    double res = (double)((tf * (this->k + 1) / (tf + this->k * (1 - this->b + this->b * (double)(dl / this->dl_avg_)))));
    return res;
}

void BM25::SetDlAvg(size_t dl_avg) {
    this->dl_avg_ = dl_avg;
}

