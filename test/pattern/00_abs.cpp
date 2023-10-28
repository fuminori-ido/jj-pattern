/*
= NAME
abs  - check abs(3)

= SYNOPSIS
make check

= DESCRIPTION
check abs(3)
*/

#include <stdio.h>
#include "gtest/gtest.h"

TEST(Abs, abs){
  ASSERT_EQ(1,            abs( 1));
  ASSERT_EQ(1,            abs(-1));
  ASSERT_EQ(641116157,    abs(-641116157));

  const char* s = "Hello,";
  int hash  = 0;
  for(; *s; s++){
    int saved_hash = hash;

    printf("  '%c'    hash =    %12d %8x\n", *s, hash, hash);
  //hash = hash*131 + int((unsigned char)*s);
    hash = hash<<7;
    printf("         hash<<7   %12d %8x\n", hash, hash);

    hash++;
    printf("         hash++    %12d %8x\n", hash, hash);

    hash += int((unsigned char)*s);
    printf("         hash+=... %12d %8x\n", hash, hash);

    hash &= 0x7fffffff;
    printf("         hash &=.. %12d %8x\n", hash, hash);

    hash = ((saved_hash<<7) + saved_hash +  int((unsigned char)*s)) & 0x7fffffff;
    printf("         hash'     %12d %8x\n", hash, hash);
  }
  printf(  "  RESULT hash:     %12d %8x\n", hash, hash);
  printf(  "  abs(hash_str):   %12d\n",     abs(hash));
  ASSERT_EQ(1807369632,    hash);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
