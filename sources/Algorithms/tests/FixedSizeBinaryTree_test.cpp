//
// Created by dawid on 26.02.17.
//

#include <gtest/gtest.h>
#include "../sources/FixedSizeBinaryTree.hpp"


class FixedSizeBinaryTreeTest : public ::testing::Test {
public:

    void testNode(PiOS::NodeId node, bool vaild, PiOS::NodeId::RankType rank, PiOS::NodeId::RankType indexInRank,
                  size_t absoluteIdx) {
        ASSERT_TRUE(vaild);
        ASSERT_EQ(node.rank(), rank);
        ASSERT_EQ(node.indexInRank(), indexInRank);
        ASSERT_EQ(node.absoluteIndex(), absoluteIdx);
    }

    PiOS::NodeId findRightMostNode(PiOS::NodeId node, PiOS::FixedSizeBinaryTree<size_t> &tree) {
        using namespace PiOS;

        while (tree.rightChild(node).isValid()) {
            node = tree.rightChild(node);
        }

        return node;
    }
};


TEST_F(FixedSizeBinaryTreeTest, NodeId_calculations) {
    using namespace PiOS;

    NodeId node(0, 0);
    testNode(node, true, 0, 0, 0);

    node = NodeId(0);
    testNode(node, true, 0, 0, 0);

    auto maxValue = std::numeric_limits<NodeId::RankType>::max();
    node = NodeId(maxValue, maxValue);
    ASSERT_FALSE(node.isValid());

    auto maxValueIdx = std::numeric_limits<size_t>::max();
    node = NodeId(maxValueIdx);
    ASSERT_FALSE(node.isValid());

    node = NodeId(3, 1);
    testNode(node, true, 3, 1, 8);

    node = NodeId(8);
    testNode(node, true, 3, 1, 8);
}

TEST_F(FixedSizeBinaryTreeTest, constructorTest) {
    using namespace PiOS;

    using Element = size_t;
    const size_t elementsToManage = PiOS::pow(2, 5) - 1;
    Element *managedSpace = new Element[elementsToManage];

    FixedSizeBinaryTree<Element> testTree(managedSpace, elementsToManage, 0);
    ASSERT_EQ(testTree.depth(), 5);
    ASSERT_EQ(testTree.maxTransitions(), testTree.depth()-1);
    auto node = testTree.root();
    testNode(node, true, 0, 0, 0);

    ASSERT_EQ(testTree.depth(), 5);

    //detected size of tree should be fine
    node = findRightMostNode(node, testTree);
    testNode(node, true, 4, 15, 30);

    // Windows sucks and is a fuckin' lame
//    Element a;
//    EXPECT_DEATH(FixedSizeBinaryTree<Element>(&a, 0, 0), ".*");

    free(managedSpace);
}

TEST_F(FixedSizeBinaryTreeTest, treeWalkingTest) {
    using namespace PiOS;
    using Element = size_t;

    Element a;

    FixedSizeBinaryTree<Element> rootOnlyTree(&a, 1, 0);
    const NodeId &rootNode = rootOnlyTree.root();
    ASSERT_TRUE(rootNode.isValid());
    ASSERT_FALSE(rootOnlyTree.parent(rootNode).isValid());
    ASSERT_FALSE(rootOnlyTree.rightChild(rootNode).isValid());
    ASSERT_FALSE(rootOnlyTree.leftChild(rootNode).isValid());

    const size_t elementsToManage = PiOS::pow(2, 5) - 1;
    Element *managedSpace = new Element[elementsToManage];

    FixedSizeBinaryTree<Element> tree(managedSpace, elementsToManage, 0);
    ASSERT_EQ(tree.depth(), 5);
    ASSERT_EQ(tree.maxTransitions(), tree.depth()-1);
    auto root = tree.root();
    testNode(root, true, 0, 0, 0);

    auto rightChild = tree.rightChild(root);
    testNode(rightChild, true, 1, 1, 2);
    auto leftChild = tree.leftChild(root);
    testNode(leftChild, true, 1, 0, 1);

    ASSERT_EQ(tree.parent(rightChild), root);
    ASSERT_EQ(tree.parent(leftChild), root);
    ASSERT_EQ(tree.parent(leftChild), tree.parent(rightChild));

    free(managedSpace);
}

TEST_F(FixedSizeBinaryTreeTest, settingValues) {
    using namespace PiOS;
    using Element = size_t;

    const size_t elementsToManage = PiOS::pow(2, 5) - 1;
    Element *managedSpace = new Element[elementsToManage];

    FixedSizeBinaryTree<Element> tree(managedSpace, elementsToManage, 0);
    ASSERT_EQ(tree.depth(), 5);
    ASSERT_EQ(tree.maxTransitions(), tree.depth()-1);
    auto root = tree.root();

    auto node = root;
    for (size_t currentRank = 0; currentRank < tree.depth(); currentRank++) {
        tree.setValue(node, currentRank);
        node = tree.rightChild(node);
    }

    node = root;
    for (size_t currentRank = 0; currentRank < tree.depth(); currentRank++) {
        ASSERT_EQ(tree.value(node), currentRank);
        node = tree.rightChild(node);
    }

    tree.initializeAllElements(42);
    ASSERT_EQ(tree.value(tree.root()), 42);
    auto child = tree.leftChild(tree.root());
    ASSERT_EQ(tree.value(child), 42);
    child = tree.rightChild(tree.root());
    ASSERT_EQ(tree.value(child), 42);
    child = tree.rightChild(child);
    ASSERT_EQ(tree.value(child), 42);

    free(managedSpace);
}