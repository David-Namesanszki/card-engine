#include "engine/components/transformComponent.h"
#include <gtest/gtest.h>

namespace {

constexpr float kEps = 1e-4f;

void expectVec(const Vector2& v, float x, float y) {
    EXPECT_NEAR(v.x, x, kEps);
    EXPECT_NEAR(v.y, y, kEps);
}

} // namespace

// ---- World accessors: identity / no parent ----

TEST(TransformComponent, DefaultsAreIdentity) {
    TransformComponent t;
    expectVec(t.worldPosition(), 0.0f, 0.0f);
    EXPECT_NEAR(t.worldRotation(), 0.0f, kEps);
    EXPECT_NEAR(t.worldScale(), 1.0f, kEps);
}

TEST(TransformComponent, NoParentReturnsOwnValues) {
    TransformComponent t;
    t.position = {12.0f, -7.0f};
    t.rotation = 1.5f;
    t.scale = 3.0f;
    expectVec(t.worldPosition(), 12.0f, -7.0f);
    EXPECT_NEAR(t.worldRotation(), 1.5f, kEps);
    EXPECT_NEAR(t.worldScale(), 3.0f, kEps);
}

// ---- Composition through a parent ----

TEST(TransformComponent, WorldRotationAddsThroughHierarchy) {
    TransformComponent parent;
    parent.rotation = 0.5f;
    TransformComponent child;
    child.rotation = 0.2f;
    parent.addChild(&child);
    EXPECT_NEAR(child.worldRotation(), 0.7f, kEps);
}

TEST(TransformComponent, WorldScaleMultipliesThroughHierarchy) {
    TransformComponent parent;
    parent.scale = 2.0f;
    TransformComponent child;
    child.scale = 3.0f;
    parent.addChild(&child);
    EXPECT_NEAR(child.worldScale(), 6.0f, kEps);
}

TEST(TransformComponent, WorldPositionParentTranslationOnly) {
    TransformComponent parent;
    parent.position = {100.0f, 50.0f};
    TransformComponent child;
    child.position = {10.0f, 5.0f};
    parent.addChild(&child);
    expectVec(child.worldPosition(), 110.0f, 55.0f);
}

TEST(TransformComponent, WorldPositionParentRotationRotatesOffset) {
    TransformComponent parent;
    parent.rotation = 90.0f * DEG2RAD; // local +x -> world +y
    TransformComponent child;
    child.position = {10.0f, 0.0f};
    parent.addChild(&child);
    expectVec(child.worldPosition(), 0.0f, 10.0f);
}

TEST(TransformComponent, WorldPositionParentScaleScalesOffset) {
    TransformComponent parent;
    parent.scale = 2.0f;
    TransformComponent child;
    child.position = {10.0f, 0.0f};
    parent.addChild(&child);
    expectVec(child.worldPosition(), 20.0f, 0.0f);
}

TEST(TransformComponent, WorldPositionCombinedParentTransform) {
    TransformComponent parent;
    parent.position = {100.0f, 0.0f};
    parent.rotation = 90.0f * DEG2RAD;
    parent.scale = 2.0f;
    TransformComponent child;
    child.position = {10.0f, 0.0f};
    parent.addChild(&child);
    // {10,0} -> scale2 {20,0} -> rot90 {0,20} -> +pos {100,20}
    expectVec(child.worldPosition(), 100.0f, 20.0f);
}

TEST(TransformComponent, ThreeLevelNesting) {
    TransformComponent a; // grandparent
    a.scale = 2.0f;
    TransformComponent b; // parent
    b.scale = 3.0f;
    TransformComponent c; // child
    c.scale = 4.0f;
    a.addChild(&b);
    b.addChild(&c);
    EXPECT_NEAR(c.worldScale(), 24.0f, kEps);
    EXPECT_NEAR(c.worldRotation(), 0.0f, kEps);
}

// ---- Parent/child bookkeeping ----

TEST(TransformComponent, AddChildSetsParentAndRegisters) {
    TransformComponent parent;
    TransformComponent child;
    parent.addChild(&child);
    EXPECT_EQ(child.parent, &parent);
    ASSERT_EQ(parent.children.size(), 1u);
    EXPECT_EQ(parent.children[0], &child);
}

TEST(TransformComponent, AddChildReparentsDetachingFromOldParent) {
    TransformComponent oldParent;
    TransformComponent newParent;
    TransformComponent child;

    oldParent.addChild(&child);
    newParent.addChild(&child);

    EXPECT_EQ(child.parent, &newParent);
    EXPECT_TRUE(oldParent.children.empty());
    ASSERT_EQ(newParent.children.size(), 1u);
    EXPECT_EQ(newParent.children[0], &child);
}

TEST(TransformComponent, RemoveChildClearsParentAndUnregisters) {
    TransformComponent parent;
    TransformComponent child;
    parent.addChild(&child);
    parent.removeChild(&child);
    EXPECT_EQ(child.parent, nullptr);
    EXPECT_TRUE(parent.children.empty());
}

// ---- Destructor safety ----

TEST(TransformComponent, DestroyingChildUnregistersFromParent) {
    TransformComponent parent;
    {
        TransformComponent child;
        parent.addChild(&child);
        ASSERT_EQ(parent.children.size(), 1u);
    } // child destroyed here
    EXPECT_TRUE(parent.children.empty());
}

TEST(TransformComponent, DestroyingParentOrphansChildren) {
    TransformComponent child; // outlives the parent below
    {
        TransformComponent parent;
        parent.addChild(&child);
        EXPECT_EQ(child.parent, &parent);
    } // parent destroyed here
    EXPECT_EQ(child.parent, nullptr);
}
