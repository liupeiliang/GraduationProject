#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/ArtIterator.h"
#include "../art/Art.h"

using namespace std;
class ArtIteratorTest : public ::testing::Test
{
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ArtIteratorTest, SearchPrefixTest)
{
  Art<int> art;
  int v1 = 1;
  art.Insert("1234567", &v1);
  int v2 = 2;
  art.Insert("1234444", &v2);
  int v3 = 3;
  art.Insert("1234555", &v3);
  
  ArtIterator<int>* it = art.SearchPrefix("1234");
  ASSERT_TRUE(it != nullptr);
  
  int* tem = it->GetValue();
//  printf("%s\n", it->GetKey());
  ASSERT_EQ(*tem, 2);
  
  bool nx = it->HasNext();
  ASSERT_EQ(nx, true);
  it->GoNext();
//  printf("%s\n", it->GetKey());
  tem = it->GetValue();
  ASSERT_EQ(*tem, 3);

  nx = it->HasNext();
  ASSERT_EQ(nx, true);
  it->GoNext();
//  printf("%s\n", it->GetKey());
  tem = it->GetValue();
  ASSERT_EQ(*tem, 1);

  nx = it->HasNext();
  ASSERT_EQ(nx, false);
}
