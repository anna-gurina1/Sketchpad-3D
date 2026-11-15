#include "AABB.h"
#include <cfloat>
#include <algorithm>

AABB ComputeAABB(const float* vertices, int vertexCount, Vector3 position, Vector3 scale)
{
    AABB box;
    box.min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
    box.max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
    for (int i = 0; i < vertexCount; i++)
    {
        float px = vertices[3 * i + 0] * scale.x + position.x;
        float py = vertices[3 * i + 1] * scale.y + position.y;
        float pz = vertices[3 * i + 2] * scale.z + position.z;

        box.min.x = std::min(box.min.x, px);
        box.min.y = std::min(box.min.y, py);
        box.min.z = std::min(box.min.z, pz);

        box.max.x = std::max(box.max.x, px);
        box.max.y = std::max(box.max.y, py);
        box.max.z = std::max(box.max.z, pz);
    }
    return box;
}

AABB AABB::Merge(const AABB& a, const AABB& b)
{
    AABB r;
    r.min = { std::min(a.min.x, b.min.x), std::min(a.min.y, b.min.y), std::min(a.min.z, b.min.z) };
    r.max = { std::max(a.max.x, b.max.x), std::max(a.max.y, b.max.y), std::max(a.max.z, b.max.z) };
    return r;
}
