#include <gtest/gtest.h>
#include <bits/stdc++.h>
#include "../art/Art.h"

using namespace std;

class ArtTest : public ::testing::Test{
protected:
  void SetUp() override { srand(time(0)); }
  void TearDown() override {}
};

const int N = 1000000;
char value[N+5];
char* key[N+5];
unordered_map<char*, char*> ht;
map<char*, char*> rb;

TEST_F(ArtTest, SpeedTest)
{
  for (int i = 0; i < N; i++) {
    key[i] = (char*)malloc(5);
    for (int j = 0; j < 4; j++) {
      key[i][j] = rand()%26 + 'a';
    }
    key[i][4] = '\0';
    value[i] = 'a';
  }

  clock_t s, t;
  
  s = clock();
  Art<char> art;
  for (int i = 0; i < N; i++) {
    art.Insert(key[i], &value[i]);
  }
  t = clock();
  printf("art inesrt time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);

  s = clock();
  for (int i = 0; i < N; i++) {
    char* ans = art.Find(key[i]);
  }
  t = clock();
  printf("art find time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);
  
  s = clock();
  for (int i = 0; i < N; i++) {
    ht.insert(make_pair(key[i], &value[i]));
  }  
  t = clock();
  printf("ht insert time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);

  s = clock();
  for (int i = 0; i < N; i++) {
    auto ans = ht.find(key[i]);
  }  
  t = clock();
  printf("ht find time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);  

  s = clock();
  for (int i = 0; i < N; i++) {
    rb.insert(make_pair(key[i], &value[i]));
  }  
  t = clock();
  printf("rb insert time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);

  s = clock();
  for (int i = 0; i < N; i++) {
    auto ans = rb.find(key[i]);
  }  
  t = clock();
  printf("rb find time=%f\n",(float)(t-s)*1000/CLOCKS_PER_SEC);
  
}

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
