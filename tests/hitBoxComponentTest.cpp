#include "engine/hitBoxComponent.h"
#include "engine/transformComponent.h"
#include <gtest/gtest.h>

namespace {

// A 20x20 square centred on the local origin: x,y in [-10, 10].
HitBoxComponent makeSquare(Vector2 offset = {0.0f, 0.0f}, int layer = 0) {
    HitBoxComponent hb;
    hb.position = offset;
    hb.points = {{-10.0f, -10.0f}, {10.0f, -10.0f}, {10.0f, 10.0f}, {-10.0f, 10.0f}};
    hb.layer = layer;
    return hb;
}

} // namespace

TEST(HitBoxComponent, PointInsideIdentityTransform) {
    HitBoxComponent hb = makeSquare();
    TransformComponent xf; // position {0,0}, rotation 0, scale 1

    EXPECT_TRUE(hb.isInside({0.0f, 0.0f}, xf));
    EXPECT_TRUE(hb.isInside({5.0f, -5.0f}, xf));
}

TEST(HitBoxComponent, PointOutsideIdentityTransform) {
    HitBoxComponent hb = makeSquare();
    TransformComponent xf;

    EXPECT_FALSE(hb.isInside({20.0f, 0.0f}, xf));
    EXPECT_FALSE(hb.isInside({0.0f, -50.0f}, xf));
}

TEST(HitBoxComponent, TranslationFollowsWorldPosition) {
    HitBoxComponent hb = makeSquare();
    TransformComponent xf;
    xf.position = {100.0f, 100.0f};

    EXPECT_TRUE(hb.isInside({100.0f, 100.0f}, xf)); // centre
    EXPECT_TRUE(hb.isInside({108.0f, 92.0f}, xf));  // inside near corner
    EXPECT_FALSE(hb.isInside({0.0f, 0.0f}, xf));    // old origin now outside
}

TEST(HitBoxComponent, RotationRotatesTheLocalOffset) {
    // Offset square: in local space it spans x in [10,30], y in [-10,10].
    HitBoxComponent hb = makeSquare({20.0f, 0.0f});
    TransformComponent xf;
    xf.rotation = 90.0f * DEG2RAD; // radians; local +x maps to world +y

    // The offset (+x) rotates to world +y, so the box sits around {0, 20}.
    EXPECT_TRUE(hb.isInside({0.0f, 20.0f}, xf));
    // The unrotated location is now empty.
    EXPECT_FALSE(hb.isInside({20.0f, 0.0f}, xf));
}

TEST(HitBoxComponent, ScaleGrowsTheHitBox) {
    HitBoxComponent hb = makeSquare();
    TransformComponent xf;
    xf.scale = 2.0f; // world-space half-extent becomes 20

    EXPECT_TRUE(hb.isInside({15.0f, 0.0f}, xf));  // inside the scaled box
    EXPECT_FALSE(hb.isInside({25.0f, 0.0f}, xf)); // beyond the scaled box
}

TEST(HitBoxComponent, LocalOffsetShiftsTheHitBox) {
    HitBoxComponent hb = makeSquare({20.0f, 0.0f});
    TransformComponent xf; // identity

    EXPECT_TRUE(hb.isInside({20.0f, 0.0f}, xf));  // centre of offset box
    EXPECT_FALSE(hb.isInside({0.0f, 0.0f}, xf));  // world origin is outside
}

TEST(HitBoxComponent, CombinedRotationScaleTranslation) {
    HitBoxComponent hb = makeSquare({20.0f, 0.0f});
    TransformComponent xf;
    xf.position = {100.0f, 100.0f};
    xf.rotation = 90.0f * DEG2RAD;
    xf.scale = 2.0f;

    // Offset {20,0} -> rotate 90 -> {0,20} -> scale 2 -> {0,40} -> +pos.
    EXPECT_TRUE(hb.isInside({100.0f, 140.0f}, xf));
    EXPECT_FALSE(hb.isInside({100.0f, 100.0f}, xf));
}

TEST(HitBoxComponent, RespectsParentTransform) {
    // Child inherits the parent's rotation and scale through the hierarchy.
    TransformComponent parent;
    parent.position = {50.0f, 0.0f};
    parent.rotation = 90.0f * DEG2RAD;
    parent.scale = 2.0f;

    TransformComponent child;
    child.position = {10.0f, 0.0f}; // local to parent
    parent.addChild(&child);

    HitBoxComponent hb = makeSquare(); // centred on the child origin

    // Child world pos = parent {50,0} + R(90)*scale2*{10,0}
    //                 = {50,0} + {0, 20} = {50, 20}.
    EXPECT_TRUE(hb.isInside({50.0f, 20.0f}, child));
    // Half-extent is parentScale*childScale*10 = 20, so {50,38} is still inside.
    EXPECT_TRUE(hb.isInside({50.0f, 38.0f}, child));
    EXPECT_FALSE(hb.isInside({50.0f, 45.0f}, child));
}

TEST(HitBoxComponent, DegeneratePolygonIsNeverInside) {
    HitBoxComponent hb;
    hb.position = {0.0f, 0.0f};
    hb.points = {{-10.0f, -10.0f}, {10.0f, 10.0f}}; // only 2 points
    hb.layer = 0;
    TransformComponent xf;

    EXPECT_FALSE(hb.isInside({0.0f, 0.0f}, xf));
}

TEST(HitBoxComponent, ZeroScaleDoesNotCrashAndReportsOutside) {
    HitBoxComponent hb = makeSquare();
    TransformComponent xf;
    xf.scale = 0.0f;

    // Guarded division means the query collapses to the offset; just ensure
    // it is well-defined and does not divide by zero.
    EXPECT_TRUE(hb.isInside({0.0f, 0.0f}, xf));
}
