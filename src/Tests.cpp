#include "Tests.h"
#include <iostream>
#include <vector>
#include "AABB.h"
#include "BruteForceDetector.h"

static int failed = 0;

static void Check(const char* what, bool ok)
{
    std::cout << (ok ? "[ OK ]  " : "[FAIL]  ") << what << "\n";
    if (!ok) failed++;
}

// тестовая сцена A–F из главы 2 (z = [0; 1] у всех объектов)
static std::vector<AABB> TestBoxes()
{
    return {
        { {1, 1, 0}, {3, 3, 1} },   // A
        { {2, 2, 0}, {4, 4, 1} },   // B
        { {6, 1, 0}, {7, 2, 1} },   // C
        { {6, 4, 0}, {8, 6, 1} },   // D
        { {7, 5, 0}, {9, 7, 1} },   // E
        { {1, 6, 0}, {2, 7, 1} },   // F
    };
}

static void TestAABB()
{
    auto b = TestBoxes();
    const AABB &A = b[0], &B = b[1], &C = b[2], &E = b[4];

    Check("A и B пересекаются",                  A.Intersects(B));
    Check("B и A пересекаются (наоборот)",       B.Intersects(A));
    Check("A и C не пересекаются",               !A.Intersects(C));
    Check("C и E не пересекаются (по Y далеко)", !C.Intersects(E));

    AABB p{ {0, 0, 0}, {1, 1, 1} }, q{ {1, 0, 0}, {2, 1, 1} };
    Check("касание гранью считается пересечением", p.Intersects(q));

    Vector3 c = A.Center();
    Check("центр A = (2; 2; 0,5)", c.x == 2 && c.y == 2 && c.z == 0.5f);

    float v[] = { 0,0,0,   2,0,0,   2,3,0,   0,3,1 };
    AABB box = ComputeAABB(v, 4, { 5, 0, 0 }, { 1, 1, 1 });
    Check("ComputeAABB: min = (5; 0; 0)", box.min.x == 5 && box.min.y == 0 && box.min.z == 0);
    Check("ComputeAABB: max = (7; 3; 1)", box.max.x == 7 && box.max.y == 3 && box.max.z == 1);
    AABB big = ComputeAABB(v, 4, { 0, 0, 0 }, { 2, 2, 2 });
    Check("ComputeAABB, масштаб 2: max = (4; 6; 2)", big.max.x == 4 && big.max.y == 6 && big.max.z == 2);

    AABB m = AABB::Merge(A, B);
    Check("Merge(A, B) = [1;4]×[1;4]×[0;1]", m.min.x == 1 && m.min.y == 1 && m.max.x == 4 && m.max.y == 4);
}

static void TestBruteForce()
{
    BruteForceDetector bf;
    AlgorithmMetrics m;
    auto pairs = bf.Detect(TestBoxes(), m);

    bool ok = pairs.size() == 2
           && pairs[0].a == 0 && pairs[0].b == 1      // (A, B)
           && pairs[1].a == 3 && pairs[1].b == 4;     // (D, E)
    Check("Brute Force нашёл {(A,B), (D,E)}", ok);
    Check("Brute Force: 15 тестов AABB (6·5/2)", m.aabbTests == 15);
}

void RunAllTests()
{
    std::cout << "=== Самопроверка ===\n";
    failed = 0;
    TestAABB();
    TestBruteForce();
    std::cout << (failed == 0 ? "Все тесты пройдены\n\n" : "ЕСТЬ ОШИБКИ!\n\n");
}
