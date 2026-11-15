#pragma once

// Показатели работы алгоритма
struct AlgorithmMetrics {
    long long aabbTests = 0;       // сколько раз вызван Intersects
    long long candidatePairs = 0;  // сколько пар передано на проверку
    double timeMs = 0;             // время работы, мс
    void Reset() { *this = AlgorithmMetrics{}; }
};
