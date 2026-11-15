#pragma once
#include <vector>
#include "AABB.h"
#include "AlgorithmMetrics.h"

// Пара пересекающихся объектов: номера в списке коробок
struct CollisionPair { int a, b; };

// Общий интерфейс всех алгоритмов Broad Phase
class CollisionDetector {
public:
    virtual ~CollisionDetector() = default;
    virtual const char* Name() const = 0;
    virtual std::vector<CollisionPair> Detect(const std::vector<AABB>& boxes, AlgorithmMetrics& m) = 0;
};
