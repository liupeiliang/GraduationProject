#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/Art.h"

using namespace std;

class ArtTest : public ::testing::Test{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ArtTest, SimpleInsertAndFindTest)
{
  Art<int> art;
  int value = 123;
  art.Insert("aaaaabbaa", &value);
  int* ans = art.Find("aaaaabbaa");
//  cout << (*ans) << endl;
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, value);

  ans = art.Find("aaaaabba");
  ASSERT_TRUE(ans == nullptr);
  
  int v2 = 456;
  art.Insert("aaaaabccc", &v2);

  ans = art.Find("aaaaab");
  ASSERT_TRUE(ans == nullptr);

  ans = art.Find("aaaaabcc");
  ASSERT_TRUE(ans == nullptr);
  
  ans = art.Find("aaaaabccc");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v2);

  ans = art.Find("aaaaabbaa");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, value);

  int v3 = 789;
  art.Insert("aabzzzzzz", &v3);

  ans = art.Find("aaaaab");
  ASSERT_TRUE(ans == nullptr);

  ans = art.Find("aaaaabcc");
  ASSERT_TRUE(ans == nullptr);

  ans = art.Find("aa");
  ASSERT_TRUE(ans == nullptr);

  ans = art.Find("aabzz");
  ASSERT_TRUE(ans == nullptr);
  
  ans = art.Find("aaaaabccc");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v2);

  ans = art.Find("aaaaabbaa");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, value);

  ans = art.Find("aabzzzzzz");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v3);  

  int v4 = 999;
  art.Insert("aabzzzzzz", &v4);
  ans = art.Find("aabzzzzzz");
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v4);
  
}

int v[256];

TEST_F(ArtTest, GrowTest)
{
  
  Art<int> art;
  

  for (int i = -128; i < 128; i++) {
    if (i == 0) continue;
    char key[] = "aaaaaaa";
    key[3] = i;
    v[i+128] = i;

    art.Insert(key, &v[i+128]);
  }

  for (int i = -128; i < 128; i++) {
    if (i == 0) continue;
    
    char key[] = "aaaaaaa";
    key[3] = i;

    int* ans = art.Find(key);
    ASSERT_TRUE(ans != nullptr);
    ASSERT_EQ(*ans, v[i+128]);
  }

}

TEST_F(ArtTest, PrefixPessimisticTest)
{
  char k1[] = "1234567890123456789";
  char k2[] = "1234567890123456888";
  int v1 = 123;
  int v2 = 456;
  
  Art<int> art;
  art.Insert(k1, &v1);
  art.Insert(k2, &v2);

  int* ans = art.Find(k1);
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v1);

  ans = art.Find(k2);
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v2);

  char k3[] = "1234567890123450789";

  ans = art.Find(k3);
  ASSERT_TRUE(ans == nullptr);
  
  int v3 = 789;
  art.Insert(k3, &v3);

  ans = art.Find(k1);
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v1);

  ans = art.Find(k2);
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v2);

  ans = art.Find(k3);
  ASSERT_TRUE(ans != nullptr);
  ASSERT_EQ(*ans, v3);  
}
