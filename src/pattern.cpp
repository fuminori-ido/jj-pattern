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

  child_1 -> child_2 -> child_3 -> child_N
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

  node_1 -> node_2 -> node_3 -> node_N

  node_1 -> node_1a   [dir=back]
  node_1 -> node_1b   [dir=back]

  node_1a -> node_1b -> node_1c
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

/*!
\class  Collect
\brief  define one-to-many relation between two classes.

@dot
digraph D {
  parent
  child_1 [label="child 1"]
  child_2 [label="child 2"]
  child_3 [label="..."      shape=plaintext]
  child_N [label="child N"]

  { rank=same parent }
  { rank=same
    child_1 -> child_2 -> child_3 -> child_N
    rankdir = LR
  }

  parent  -> child_N [ label="tail" ]
  child_N -> child_1 [ constraint=false ]
}
@enddot

### Data structure

Relation between Childlen is implemented by `ring`, which is:

* linked-list between children.
* last child's next is first.

Ring is a little better than linkd-list so that I chose this data structure.

### Example

Here the example of Publisher - Books collection (1:n) is.

1. define ex.cpp as:

        #include <jj/pattern.h>
        #include "ex.b"

        class Publisher : INHERIT_Publisher {...};
        class Book      : INHERIT_Book      {...};

        jjCollect (books, Publisher,  Book);

2. generate "ex.b" which defines the relations between classes by:

        $ bgen ex.cpp >ex.b

3. link with jj/pattern library and run!:

        $ g++ -Wall ex.cpp -ljj && a.out

See [collect_test.cpp](../test/pattern/collect_test.cpp) source as actual sample.
*/

/*!
\class  Collect::Parent
\brief  Parent base class for jj::Collect pattern.
*/

/*!
\class  Collect::Child
\brief  Child base class for jj::Collect pattern.
*/

/*!
\class  Collect::Iter
\brief  Iterator class for jj::Collect pattern.
*/

Collect::Parent::Parent(){
  _tail = NULL;
  _num  = 0;
}

/*! add child to parent.
*/
void Collect::add(Parent* p, Child* c){
  /* require */
  if( p==NULL || c==NULL ) return;

  /* check */
  if( c->_next != NULL ) return;

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
Collect::Child *Collect::child(Parent* p){
  if( p==NULL ) return NULL;
  Child* c = p->_tail;
  if(c){
    c = c->_next;
    return c;
  }else
    return NULL;
}

/*! get last child of parent */
Collect::Child *Collect::last(Parent* p){
  if( p==NULL ) return NULL;
  Child *c = p->_tail;
  if(c){
    return c;
  }else
    return NULL;
}

/*! get child next to the child */
Collect::Child *Collect::next(Collect::Child* c){
   if( c == NULL ) return NULL;
   return c->_next;
}

/*! delete child from the collection */
void Collect::del(Collect::Parent* parent, Collect::Child* c){
  /* require */
  if( c==NULL ) return;

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

    if(parent->_tail == c) parent->_tail = p;

    parent->_num--;
  }else
    ::jj::raise(g_eh, collect_del_internal_error);
}

/*! get number of children */
int Collect::num(Collect::Parent* p){
  if( p==NULL ) return 0;
  return p->_num;
}

/*! declare iterator */
void Collect::Iter::start(::jj::Collect::Parent* p){
  iffa(p, _last, p->_tail, NULL);
  if( _last ){
    iffa(p, _curr, _last->_next,          NULL);
  }else
    _curr = NULL;
}

/*! get child, then increment the iterator */
Collect::Child *Collect::Iter::operator++(){
  Child *result = _curr;
  if( _curr == _last )
    _curr = _last = NULL;
  else
    _curr = _curr->_next;
  return result;
}

/*!
\class  Hash
\brief  define holder-element relation with hash-search.

@dot
digraph D {
  node [
    shape = box
  ];

  holder
  holder_0  [label="0"]
  holder_1  [label=":" shape=plaintext]
  holder_i  [label="i"]
  holder_2  [label=":" shape=plaintext]
  holder_N  [label="size-1"]

  holder -> holder_0 -> holder_1 -> holder_i -> holder_2 -> holder_N

  entry_00  [label="entry(0,0)"]
  entry_01  [label="..." shape=plaintext]
  entry_0M  [label="entry(0,M)"]

  entry_i0  [label="entry(i,0)"]
  entry_i1  [label="..." shape=plaintext]
  entry_iM  [label="entry(i,M')"]

  entry_N0  [label="entry(N,0)"]
  entry_N1  [label="..." shape=plaintext]
  entry_NM  [label="entry(i,M\")"]

  holder_0  -> entry_00 -> entry_01 -> entry_0M
  holder_i  -> entry_i0 -> entry_i1 -> entry_iM
  holder_N  -> entry_N0 -> entry_N1 -> entry_NM

  { rank=same holder_0 entry_00 entry_01 entry_0M }
  { rank=same holder_i entry_i0 entry_i1 entry_iM }
  { rank=same holder_N entry_N0 entry_N1 entry_NM }
}
@enddot
*/

/*!
\class  Hash::Holder
\brief  Holder base class for jj::Hash pattern.
*/

/*!
\class  Hash::Entry
\brief  Entry base class for jj::Hash pattern.
*/

/*!
\class  Hash::Iter
\brief  Iterator class for jj::Hash pattern.
*/
static const int
  hash_init_size      = 16,   /* initial hash array size */
  hash_inc_magnitude  =  2;   /* magnitude for each increasing timing */

void Hash::Holder::init(int size){
  _size       = size;
  _num        = 0;
  if( size > 0 )
    _tail   = (Hash::Entry **)calloc(sizeof(Hash::Entry *), size);
  else
    _tail   = NULL;
}

Hash::Holder::Holder(int size){ init(size); }

Hash::Holder::Holder(){ init(0); }

Hash::Holder::~Holder(){
  free(_tail);
}

/*
expand() belongs to Hash rather than Hash::Holder is because to use
hash_base() and add().
*/
void Hash::expand(Holder *h, int new_size){
#ifdef JJDEBUG
  printf("Hash::expand(%d) this=%lx\n", new_size, this);
#endif
/* require */
  if( h==NULL ) return;

#ifdef JJDEBUG
printf("Hash::expand(%d -> %d)\n", h->_size, new_size);
#endif

  /*
  Implementation Note: create new_holder by Iter & add() and replace
  This avoids recalculation of hash % _size and _tail.
  */
  Holder  new_holder(new_size);
  Iter    i;
  Entry*  e2;

  i.start(h);
  while( (e2 = (Entry *)++i) ){
#ifdef JJDEBUG
    printf("Hash::expand() new_holder._size=%d\n", new_holder._size);
#endif
    e2->_next = NULL;       /* necessary for add() */
    this->add(&new_holder, e2);
  }

/* re-birth! */
  free(h->_tail);
  h->_size    = new_holder._size;
  h->_tail    = new_holder._tail;
  new_holder._tail = NULL;    /* to avoid free() at destructor */
#ifdef JJDEBUG
  printf("Hash::expand() end\n");
#endif
}

void Hash::add(Holder* h, Entry* e){
/* require */
  if( h==NULL || e==NULL ) return;

/* check */
  if( e->_next != NULL ) return;

/* initial? */
  if( h->_tail==NULL ) expand(h, hash_init_size);

/* need to expand?
  --logic is to expand when num is twice as array size
*/
  if( h->_num > h->_size * 2 ){
  expand(h, h->_size*hash_inc_magnitude);
  }

/* _jjCollect::add() と同様 */
#ifdef JJDEBUG
  printf("  Hash::add(%lx) ix is ...\n", h);
#endif
  int ix = hash_base(e) % h->_size;
#ifdef JJDEBUG
printf("  Hash::add(%lx) ix=%d\n", h, ix);
#endif
  if( h->_tail[ix] ){
    e->_next              = h->_tail[ix]->_next;
    h->_tail[ix]->_next   = e;
  }else{
    e->_next              = e;
  }
  h->_tail[ix] = e;
  h->_num++;
#ifdef JJDEBUG
  printf("Hash::add(%lx) end\n", h);
#endif
}

void Hash::del(Holder* h, Entry* e){
/* require */
  if( h==NULL || e==NULL ) return;

  int     ix = hash_base(e) % h->_size;
  Entry*  p,
       *  n;

  if( e->_next == e ){                // last entry?
    e->_next = h->_tail[ix] = NULL;   // then emptify
    h->_num--;
    return;
  }
  for(p=h->_tail[ix]; p; p=n){        //find 'p' points to s
    n = p->_next;
    if( n==e ) break;
    if( n==h->_tail[ix] ) n = NULL;
  }
  if(p){
    p->_next = e->_next;
    e->_next = NULL;
    if(h->_tail[ix] == e) h->_tail[ix] = p;
    h->_num--;
  }else
    jj::raise(g_eh, hash_del_internal_error);
}

jj::Hash::Entry* Hash::sel(Holder* h, Entry* key){
  if( h == NULL || key == NULL ) return NULL;

/* initial? */
  if( h->_size == 0 ) return NULL;

  int     ix  = hash_base(key) % h->_size;
#ifdef JJDEBUG
printf("Hash::sel() 2 ix=%d\n", ix);
#endif
  Entry*  beg = h->_tail[ix],
       *  nxt,
       *  e;

  if( !beg ) return NULL;
  for(nxt = beg->_next; nxt;){
    e = nxt;
    if(nxt == beg)
      nxt = beg = NULL;           /* end of list */
    else
      nxt = e->_next;
    if( cmp_base(e, key)==0 ){
      return e;
    }
  }
  return NULL;
}

int Hash::num(Holder* h){
  if( h==NULL ) return 0;
  return h->_num;
}

#include <stdio.h>      /* just for put_stat */

void Hash::put_stat(Holder* h){
  int   num       = 0,
        conflicts = 0,
        i;

  for(i=0; i<h->_size; i++){
    bool    first = true;
    Entry*  beg   = h->_tail[i],
         *  nxt,
         *  e;
    if( !beg ) continue;
    for(nxt = beg->_next; nxt;){
      e = nxt;
      if(nxt == beg)
        nxt = beg = NULL;
      else
        nxt = e->_next;
  /* action on item */
      num++;
      if( !first ) conflicts++;
      first = false;
    }
  }
  printf("array size= %d\n"
         "num       = %d\n"
         "conflicts = %d\n",
         h->_size, num, conflicts);
}

void Hash::put_stat2(Holder* h){
  int*  conflicts = (int *)malloc(sizeof(int)*h->_size);
  int   num       = 0,
        i;

  for(i=0; i<h->_size; i++){
    bool    first = true;
    Entry*  beg   = h->_tail[i],
         *  nxt,
         *  e;
    conflicts[i] = 0;
    if( !beg ) continue;
    for(nxt = beg->_next; nxt;){
      e = nxt;
      if(nxt == beg)
          nxt = beg = NULL;
      else
          nxt = e->_next;
  /* action on item */
      num++;
      if( !first ) conflicts[i]++;
      first = false;
    }
    printf("conflicts[%d] = %d\n", i, conflicts[i]);
  }
  free(conflicts);
}


void Hash::Iter::start(Holder* h){
  _h          = h;
  _ix         = 0;
  _beg        = NULL;
}

jj::Hash::Entry* Hash::Iter::operator++(){
  if( _beg == NULL ){
    /* find next non-empty slot */
    for(;;){
      if( _ix >= _h->_size )
        return NULL;            /* end of hash array */

      _beg = _h->_tail[_ix++];
      if(_beg){
        _nxt = _beg->_next;
        break;
      }
    }
  }
/* next entry */
  Entry* e = _nxt;
  if(_nxt == _beg)
    _nxt = _beg = NULL;             /* end of list */
  else{
    _nxt = e->_next;
  }
  return e;
}


/* convenient hash functions */
int hash_str(const char *s){
  int hash = 0;

  for(; *s; s++){
    hash = hash*131 + int((unsigned char)*s);
  }
  return abs(hash);   /* to avoid minus value */
}

}; // jj
