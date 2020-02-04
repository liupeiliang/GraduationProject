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
//  cout << (*ans) << endl;
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, value);

  ans = art.Find("aabba");
  ASSERT_TRUE(ans == nullptr);
  
  int v2 = 456;
  art.Insert("aabccc", &v2);

  ans = art.Find("aab");
  ASSERT_TRUE(ans == nullptr);
  
  ans = art.Find("aabccc");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v2);

  ans = art.Find("aabbaa");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, value);
  
}

