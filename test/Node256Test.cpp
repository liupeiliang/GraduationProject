#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/Node256.h"

using namespace std;
class Node256Test : public ::testing::Test
{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(Node256Test, TestSize)
{
  ASSERT_EQ(sizeof(Node256<int>), 2064);
}
