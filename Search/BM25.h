#pragma once

#include <iostream>

class BM25 {
public:
    double Score(size_t tf, size_t dl);

    BM25& operator=(BM25&& other) noexcept;

    void SetDlAvg(size_t dl_avg);
private:
    size_t dl_avg_;
    const double k = 1.2;
    const double b = 0.75;
};