/*
= NAME
multiple-inheritance  - check multiple inheritance twice from the same base class

= SYNOPSIS
make check

= DESCRIPTION

= SEE ALSO
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

/*

duplicate inheritance from the same class is invalid:

class Derived : public Base, public Base {
  int c;

public:
  Derived(){ c = 1000; }

  void hello(){
    printf("address=%p hello Derived! c = %d\n", this, c);
  }
};
*/

class Mid1 : public Base {};
class Mid2 : public Base {};

class Derived : public Mid1, public Mid2 {
  int c;

public:
  Derived(){ c = 1000; }

  void hello(){
    printf("address=%p hello Derived! c = %d\n", this, c);
  }
};



TEST(MultipleInheritance, multiple_inheritance){
  Derived d;

  d.hello();

  Mid1* m = &d;
  m->hello();

  Derived*  dp = static_cast<Derived*>(m);
  dp->hello();

  Mid2* m2 = &d;
  m2->hello();

  Derived*  dp2= static_cast<Derived*>(m2);
  dp2->hello();
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
