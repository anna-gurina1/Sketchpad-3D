#include <string>
#include <raylib.h>
#include <vector>

class AlgorithmMetrics {
};
class Scene {
};


struct AABB {
    Vector3 min, max;
    bool Intersects(const AABB& o) const;   
    Vector3 Center() const;
    static AABB Merge(const AABB& a, const AABB& b);   
};

class SceneObject {
public:
    int id; std::string name;
    Model model; Vector3 position{}; Vector3 scale{1,1,1};
    AABB worldBox;
    void UpdateAABB();  
};

struct CollisionPair { int a, b; };
class CollisionDetector {
public:
    virtual ~CollisionDetector() = default;
    virtual const char* Name() const = 0;
    virtual std::vector<CollisionPair> Detect(const Scene& s, AlgorithmMetrics& m) = 0;
};