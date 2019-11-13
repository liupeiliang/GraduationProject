#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/Art.h"

using namespace std;

class ArtTest : public ::testing::Test{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ArtTest, TestCalc)
{
  Art t;
  ASSERT_EQ(11, t.calc(5, 6));
  ASSERT_EQ(333, t.calc(111, 222));
}

