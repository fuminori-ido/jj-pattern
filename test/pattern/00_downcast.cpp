/*
= NAME
downcast  - check c++ downcast

= SYNOPSIS
make check

= DESCRIPTION
check multiple inheritance address

= SEE ALSO
https://stackoverflow.com/questions/11855018/c-inheritance-downcasting
*/

#include <stdio.h>
#include "gtest/gtest.h"

class Base {
  int a;

public:
  Base(){ a = 10; }

  void hello(){
    printf("address=%p hello Base! a = %d\n", this, a);
  }
};

class Base2 {
  int b;

public:
  Base2(){ b = 100; }

  void hello(){
    printf("address=%p hello Base2! b = %d\n", this, b);
  }
};


class Derived : public Base, public Base2 {
  int c;

public:
  Derived(){ c = 1000; }

  void hello(){
    printf("address=%p hello Derived! c = %d\n", this, c);
  }
};

TEST(Downcast, downcast){
  Derived d;

  d.hello();

  Base*     b = &d;
  b->hello();

  Base2*    b2= &d;
  b2->hello();

  Derived*  dp = static_cast<Derived*>(b);
  dp->hello();

  Derived*  dp2= static_cast<Derived*>(b2);
  dp2->hello();
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
