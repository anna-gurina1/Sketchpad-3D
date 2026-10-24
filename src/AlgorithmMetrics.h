
struct AlgorithmMetrics {
    long long aabbTests = 0;       // сколько раз вызвали Intersects
    long long candidatePairs = 0;  // сколько пар попали на проверку
    double timeMs = 0;             // время работы Detect, мс

    void Reset() { *this = AlgorithmMetrics{}; }
};