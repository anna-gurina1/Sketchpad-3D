#pragma once
#include "CollisionDetector.h"

class BruteForceDetector : public CollisionDetector {
public:
    const char* Name() const override { return "Brute Force"; }
    std::vector<CollisionPair> Detect(const std::vector<AABB>& boxes,
                                      AlgorithmMetrics& m) override;
};
