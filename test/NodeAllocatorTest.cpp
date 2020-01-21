#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/NodeAllocator.h"

using namespace std;
class NodeAllocatorTest : public ::testing::Test
{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(NodeAllocatorTest, Test1)
{
  NodeAllocator<char> na;
  auto tem = na.NewNode(NODE4);
  
}
