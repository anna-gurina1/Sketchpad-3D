#pragma once
#include "raylib.h"

struct AABB {
    Vector3 min{0, 0, 0};
    Vector3 max{0, 0, 0};

    inline bool Intersects(const AABB& o) const {
        if (max.x < o.min.x || min.x > o.max.x) return false;
        if (max.y < o.min.y || min.y > o.max.y) return false;
        if (max.z < o.min.z || min.z > o.max.z) return false;
        return true;
    }

    inline Vector3 Center() const {
        return { (min.x + max.x) * 0.5f,
                 (min.y + max.y) * 0.5f,
                 (min.z + max.z) * 0.5f };
    }

    static AABB Merge(const AABB& a, const AABB& b);
};

AABB ComputeAABB(const float* vertices, int vertexCount, Vector3 position, Vector3 scale);
