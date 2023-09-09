#include <stdio.h>
#include <string.h>
#include "gtest/gtest.h"
#include "jj/pattern.h"
#include "dcollect_test.b" /* include Part-B */

// define models
class Book : INHERIT_Book {
public:
  char *isbn;
  char *name;
  Book(const char *i, const char *n) {isbn=strdup(i); name=strdup(n);}
};

class Publisher : INHERIT_Publisher {
public:
  char *name;
  Publisher(const char *n) {name=strdup(n);}
};

// define pattern between models
jjDCollect (books,       Publisher,  Book);

TEST(Simplest, dcollect){
// create objects for test
  Publisher p1("P1");
  Book      oosc("1-123", "Object Oriented S/W Construction"),
            itpl("2-111", "Introduction to the Theory of Programming Lanugages");

// define relation
  ASSERT_EQ(0, books.num(&p1));
  books.add(&p1, &oosc);  ASSERT_EQ(1, books.num(&p1));
  books.add(&p1, &itpl);  ASSERT_EQ(2, books.num(&p1));

// Print books
  books_class::Iter i;
  Book*             b;
  i.start(&p1);
  b = ++i;
  ASSERT_STREQ("Object Oriented S/W Construction",                     b->name);
  b = ++i;
  ASSERT_STREQ("Introduction to the Theory of Programming Lanugages",  b->name);
  b = ++i;
  ASSERT_EQ(NULL, b);

// Print books in reverse order
  i.start(&itpl, &oosc);
  b = --i;
  ASSERT_STREQ("Introduction to the Theory of Programming Lanugages",  b->name);
  b = --i;
  ASSERT_STREQ("Object Oriented S/W Construction",                     b->name);
  b = --i;
  ASSERT_EQ(NULL, b);

  books.del(&p1, &itpl);
  ASSERT_EQ(1, books.num(&p1));
  i.start(&p1);
  b = ++i;
  ASSERT_STREQ("Object Oriented S/W Construction",                     b->name);
  b = ++i;
  ASSERT_EQ(NULL, b);

  books.del(&p1, &oosc);
  ASSERT_EQ(0, books.num(&p1));
  i.start(&p1);
  b = ++i;
  ASSERT_EQ(NULL, b);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
