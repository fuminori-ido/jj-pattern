/*!
\file   pattern.cpp
\brief  Pattern library based on "Taming C++"
*/

#ifdef JJDEBUG
# include <stdio.h>     /* for printf() for debug */
#endif
#include <malloc.h>
#include <stdlib.h>       /* for abs() */
#include <string.h>
#include "jj/errno.h"
#include "jj/pattern.h"


namespace jj {

static Errno      g_eh;

enum Error {
  aggregate_del_internal_error    = 1,
  collect_del_internal_error,
  hash_del_internal_error,
  graph_del_internal_error
};

/*!
\class  Aggregate
\brief  define one-to-many relation between two classes.

@dot
digraph D {
  parent
  child_1 [label="child 1"]
  child_2 [label="child 2"]
  child_3 [label="..."      shape=plaintext]
  child_N [label="child N"]

  { rank=same parent }
  { rank=same child_1 child_2 child_3 child_N }

  parent  -> child_1 [dir=back]
  parent  -> child_2 [dir=back]
  parent  -> child_N [dir=back]

  child_1 -> child_2
  child_2 -> child_3
  child_3 -> child_N
}
@enddot

### Data structure

Relation between Childlen is implemented by `ring`, which is:

* linked-list between children.
* last child's next is first.

Ring is a little better than linkd-list so that I chose this data structure.

### Example

Here the example of Publisher - Books aggregation (1:n) is.

1. define ex.cpp as:

        #include <jj/pattern.h>
        #include "ex.b"

        class Publisher : INHERIT_Publisher {...};
        class Book      : INHERIT_Book      {...};

        jjAggregate (books, Publisher,  Book);

2. generate "ex.b" which defines the relations between classes by:

        $ bgen ex.cpp >ex.b

3. link with jj/pattern library and run!:

        $ g++ -Wall ex.cpp -ljj && a.out

See [01_test.cpp](../test/pattern/01_test.cpp) source as actual sample.

### Tree

`Aggregate` pattern can be used as tree for a class as follows:

@dot
digraph D {
  root
  node_1  [label="node 1"]
  node_1a [label="node 1a"]
  node_1b [label="node 1b"]
  node_1c [label="..."      shape=plaintext]
  node_2  [label="node 2"]
  node_3  [label="..."      shape=plaintext]
  node_N  [label="node N"]

  { rank=same root }
  { rank=same node_1 node_2 node_3 node_N }
  { rank=same node_1a node_1b node_1c }

  root  -> node_1 [dir=back]
  root  -> node_2 [dir=back]
  root  -> node_N [dir=back]

  node_1 -> node_2
  node_2 -> node_3
  node_3 -> node_N

  node_1 -> node_1a   [dir=back]
  node_1 -> node_1b   [dir=back]

  node_1a -> node_1b
  node_1b -> node_1c
}
@enddot

Tree can be implemented by:

    #include <jj/pattern.h>
    #include "ex.b"

    class Node : INHERIT_Node {...};
    jjAggregate (tree, Node, Node);

See [02_test.cpp](../test/pattern/02_test.cpp) source as actual sample.
*/

/*!
\class  Aggregate::Parent
\brief  Parent base class for jj::Aggregate pattern.
*/

/*!
\class  Aggregate::Child
\brief  Child base class for jj::Aggregate pattern.
*/

/*!
\class  Aggregate::Iter
\brief  Iterator class for jj::Aggregate pattern.
*/

Aggregate::Parent::Parent(){
  _tail = NULL;
  _num  = 0;
}

/*! add child to parent.
*/
void Aggregate::add(Parent* p, Child* c){
  /* require */
  if( p==NULL || c==NULL ) return;

  /* check */
  if( c->_parent != NULL || c->_next != NULL ) return;

  c->_parent = p;
  if( p->_tail ){
    c->_next        = p->_tail->_next;
    p->_tail->_next = c;
  }else{
    c->_next    = c;
  }
  p->_tail = c;
  p->_num++;
}


/*! get 1st child of parent */
Aggregate::Child *Aggregate::child(Parent* p){
  if( p==NULL ) return NULL;
  Child* c = p->_tail;
  if(c){
    c = c->_next;
    return c;
  }else
    return NULL;
}

/*! get last child of parent */
Aggregate::Child *Aggregate::last(Parent* p){
  if( p==NULL ) return NULL;
  Child *c = p->_tail;
  if(c){
    return c;
  }else
    return NULL;
}

/*! get parent of the child */
Aggregate::Parent* Aggregate::parent(Aggregate::Child* c){
   if( c == NULL ) return NULL;
   Parent *p = c->_parent;
   if( p == NULL ) return NULL;
   return p;
}

/*! get child next to the child */
Aggregate::Child *Aggregate::next(Aggregate::Child* c){
   if( c == NULL ) return NULL;
   return c->_next;
}

/*! delete child from the aggregation */
void Aggregate::del(Aggregate::Child* c){
  /* require */
  if( c==NULL ) return;
  Parent* parent = c->_parent;

  if( c->_next == c ){            // last element?
    c->_next= parent->_tail = NULL;     // then emptify
    parent->_num = 0;
    return;
  }
  Child *p, *next;
  for(p=parent->_tail; p; p=next){  //find 'p' points to s
    next = p->_next;
    if( next == c ) break;
    if( next == parent->_tail ) next = NULL;
  }
  if(p){
    p->_next = c->_next;

    //set NULL for later add()
    c->_next    = NULL;
    c->_parent  = NULL;

    if(parent->_tail == c) parent->_tail = p;

    parent->_num--;
  }else
    ::jj::raise(g_eh, aggregate_del_internal_error);
}

/*! get number of children */
int Aggregate::num(Aggregate::Parent* p){
  if( p==NULL ) return 0;
  return p->_num;
}

#define iffa(cond, lval, t, f) if(cond){ (lval)=t; }else{ (lval)=f; }

/*! declare iterator */
void Aggregate::Iter::start(::jj::Aggregate::Parent* p){
  iffa(p, _last, p->_tail, NULL);
  if( _last ){
    iffa(p, _curr, _last->_next,          NULL);
  }else
    _curr = NULL;
}

/*! get child, then increment the iterator */
Aggregate::Child *Aggregate::Iter::operator++(){
  Child *result = _curr;
  if( _curr == _last )
    _curr = _last = NULL;
  else
    _curr = _curr->_next;
  return result;
}
}; // jj
