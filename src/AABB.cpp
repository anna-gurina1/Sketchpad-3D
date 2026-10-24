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
