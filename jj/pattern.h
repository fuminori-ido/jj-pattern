#ifndef jjpattern_h
#define jjpattern_h

namespace jj {

class Aggregate {
public:
  class Parent;      //forward
  class Iter;

  class Child {
    friend class Aggregate;
    friend class Aggregate::Iter;

    Child*  _next;
    Parent* _parent;

  public:
    Child(){_next=NULL; _parent=NULL;}
  };

  class Parent {
    friend class Aggregate;
    friend class Aggregate::Iter;

    Child*  _tail;
    int     _num;

  public:
    Parent();
  };

  void    add   (Parent* p, Child* c);
  Child*  child (Parent* p);
  Child*  last  (Parent* p);
  void    del   (Child*  c);
  Parent* parent(Child*  c);
  Child*  next  (Child*  c);
  int     num   (Parent* p);

  class Iter {
    Child*  _curr;
    Child*  _last;
  public:
            Iter        (){_curr = _last = NULL;}
            Iter        (Parent* p){ start(p); }
    void    start       (Parent* p);
    Child*  operator++  ();
  };
};


/*----------------------------------------------------------------------
Collect Interface

TODO
----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
Double linked Collect Interface

TODO
----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
jjHash Interface

TODO
----------------------------------------------------------------------*/

/*----------------------------------------------------------------------
jjGraph Interface

TODO
----------------------------------------------------------------------*/
}; // jj

/*
*/
#define jjAggregate(id, _Parent, _Child)    \
class id##_class :  public jj::Aggregate {  \
public:                                     \
  void      add   (_Parent* p, _Child* c){ jj::Aggregate::add((id##_##Parent *)p, (id##_##Child *)c); }  \
  _Child*   child (_Parent* p)  { return static_cast<_Child* >(static_cast<id##_##Child* >(jj::Aggregate::child((id##_##Parent *)p))); }  \
  _Child*   last  (_Parent* p)  { return static_cast<_Child* >(static_cast<id##_##Child* >(jj::Aggregate::last((id##_##Parent *)p))); }   \
  void      del   (_Child* c)   { jj::Aggregate::del((id##_##Child *)c); }  \
  _Parent*  parent(_Child* c)   { return static_cast<_Parent*>(static_cast<id##_##Parent*>(jj::Aggregate::parent((id##_##Child *)c))); }  \
  _Child*   next  (_Child* c)   { return static_cast<_Child* >(static_cast<id##_##Child* >(jj::Aggregate::next((id##_##Child *)c))); }    \
  int       num   (_Parent* p)  { return jj::Aggregate::num((id##_##Parent *)p); }  \
                                            \
  class Iter : public jj::Aggregate::Iter { \
  public:                                   \
            Iter()           : jj::Aggregate::Iter()  {}  \
            Iter(_Parent* p) : jj::Aggregate::Iter((id##_##Parent *)p)  {}  \
    void    start(_Parent* p) { jj::Aggregate::Iter::start((id##_##Parent *)p); } \
    _Child* operator++()      { return static_cast<_Child* >(static_cast<id##_##Child*>(jj::Aggregate::Iter::operator++())); }      \
  };  \
};    \
id##_class id;

#endif /* jj/pattern.h */
