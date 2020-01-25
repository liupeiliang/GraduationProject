#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/LeafNode.h"

using namespace std;
class LeafNodeTest : public ::testing::Test
{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(LeafNodeTest, TestSize)
{
  ASSERT_EQ(sizeof(LeafNode<int>), 12);
}
