#include <bits/stdc++.h>
#include "gtest/gtest.h"

#include "test/ArtIteratorTest.cpp"
#include "test/ArtTest.cpp"
#include "test/Node4Test.cpp"
#include "test/NodeAllocatorTest.cpp"
#include "test/Node256Test.cpp"
#include "art/Node4.h"

using namespace std;

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
