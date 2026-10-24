#pragma once

struct AlgorithmMetrics {
    long long aabbTests = 0;       
    long long candidatePairs = 0; 
    double timeMs = 0;            

    void Reset() { *this = AlgorithmMetrics{}; }   
};