/*
NAME
  hash_test  - Hash pattern test
*/

#include <stdio.h>
#include <string.h>
#include "gtest/gtest.h"
#include "jj/pattern.h"
#include "hash_test.b" /* include Part-B */

// define models
class App : INHERIT_App {
};

class Atom : INHERIT_Atom {
  char *_str;
public:
  Atom(const char *str);
 ~Atom();

  char* str(){ return _str; }
};

// define pattern between models
jjHash(atom_hash, App, Atom);

int atom_hash_class::hash_base(Entry *e){
  return jj::hash_str(((Atom*)e)->str());
}

int atom_hash_class::cmp_base(Entry *e1, Entry *e2){
  return strcmp(((Atom*)e1)->str(), ((Atom*)e2)->str());
}

atom_hash_class atom_hash;

/*----------------------------------------------------------------------------
Implementation Section
----------------------------------------------------------------------------*/
Atom::Atom(const char *str){
  _str = strdup(str);
}

Atom::~Atom(){
  free(_str);
}


/*----------------------------------------------------------------------------
Test Section
----------------------------------------------------------------------------*/
TEST(Hash, hash){
// create objects for test
  App   app;
  Atom  atom_hello  = Atom("hello"),
        atom_world  = Atom("world"),
        atom_foo    = Atom("foo"),
        atom_bar    = Atom("bar");

// define relation
  atom_hash.add(&app, &atom_hello);
  atom_hash.add(&app, &atom_world);
  atom_hash.add(&app, &atom_foo);
  atom_hash.add(&app, &atom_bar);

  Atom key = Atom("hello");
  ASSERT_EQ(&atom_hello, atom_hash.sel(&app, &key));
}

int main(int argc, char **argv){
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
