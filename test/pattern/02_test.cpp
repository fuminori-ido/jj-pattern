/*
NAME
  02  - Tree by Aggregate pattern

DESCRIPTION
  Basic sample of pattern
*/

#include <stdio.h>
#include <string.h>
#include "gtest/gtest.h"
#include "jj/pattern.h"
#include "02_test.b" /* include Part-B */

// define models
class Node : INHERIT_Node {
public:
  char *name;
  Node(const char *n) {name=strdup(n);}
};

// define pattern between models
jjAggregate (tree, Node, Node);
tree_class tree;

void print_tree(int depth, Node *node){
  for(int i=0; i<depth; i++){
    printf(" |");
  }
  printf("%s\n", node->name);

  tree_class::Iter  i;
  Node*             child;
  i.start(node);
  while( (child = ++i) ){
    print_tree(depth + 1, child);
  }
}

TEST(Tree, aggregate_as_tree){
// create objects for test
  Node  root("root"),
        n1("n1"),                                   n2("n2"),
        n1a("n1a"),     n1b("n1b"),                 n2a("n2a"),   n2b("n2b"),
                        n1bA("n1bA"), n1bB("n1bB"), n2aA("n2aA");

// define relation
  tree.add(&root, &n1);
  tree.add(&root, &n2);
  tree.add(&n1,   &n1a);    tree.add(&n1,   &n1b);
  tree.add(&n2,   &n2a);    tree.add(&n2,   &n2b);
  tree.add(&n1b,  &n1bA);   tree.add(&n1b,  &n1bB);
  tree.add(&n2a,  &n2aA);

  print_tree(0, &root);

// check children of root
  tree_class::Iter  i;
  Node*             child;
  i.start(&root);
  child = ++i;  ASSERT_EQ(&n1,    child);
  child = ++i;  ASSERT_EQ(&n2,    child);
  child = ++i;  ASSERT_EQ(NULL,   child);

// check children of n1
  i.start(&n1);
  child = ++i;  ASSERT_EQ(&n1a,   child);
  child = ++i;  ASSERT_EQ(&n1b,   child);
  child = ++i;  ASSERT_EQ(NULL,   child);

// check children of n1a
  i.start(&n1a);
  child = ++i;  ASSERT_EQ(NULL,   child);
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
