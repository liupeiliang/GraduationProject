#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/Art.h"

using namespace std;

class ArtTest : public ::testing::Test{
protected:
  void SetUp() override {}
  void TearDown() override {}
};


TEST_F(ArtTest, Test1)
{
  Art<int> art;
  int value = 123;
  art.Insert("aabbaa", &value);
  int* ans = art.Find("aabbaa");
  cout << (*ans) << endl;
  ASSERT_EQ(*ans, value);
}

