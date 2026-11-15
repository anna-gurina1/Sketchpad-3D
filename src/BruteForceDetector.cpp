#include "BruteForceDetector.h"

std::vector<CollisionPair> BruteForceDetector::Detect(const std::vector<AABB>& boxes,
                                                      AlgorithmMetrics& m)
{
    std::vector<CollisionPair> collisions;                // 1  collisions <- empty list
    const int n = (int)boxes.size();
    for (int i = 0; i <= n - 2; i++)                      // 2  FOR i <- 0 TO n-2
    {
        for (int j = i + 1; j <= n - 1; j++)              // 3  FOR j <- i+1 TO n-1
        {
            m.candidatePairs++;
            m.aabbTests++;
            if (boxes[i].Intersects(boxes[j]))            // 4  IF IntersectsAABB(...)
                collisions.push_back({ i, j });           // 5  ADD (i, j) TO collisions
        }
    }
    return collisions;                                    // 6  RETURN collisions
}
