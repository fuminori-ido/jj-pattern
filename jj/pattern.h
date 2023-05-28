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


class Hash {
public:
  class Entry;
  class Holder {
    friend class Hash;
    friend class Iter;

  public:

    int       _size,        /* array size */
              _num;         /* element number */
    Entry**   _tail;

    void      init(int size);

    Holder();
    Holder(int size);
   ~Holder();
  };

  class Entry {
    friend class Hash;
    friend class Iter;

    Entry*  _next;

  public:
    Entry(){_next=NULL;}
  };

private:
  virtual int hash_base (Entry* e)              = 0;
  virtual int cmp_base  (Entry* e1, Entry* e2)  = 0;
  void        expand    (Holder* h, int new_size);

public:


  void        add       (Holder* h, Entry* e);
  void        del       (Holder* h, Entry* e);
  Entry*      sel       (Holder* h, Entry* e);
  void        put_stat  (Holder* h);
  void        put_stat2 (Holder* h);
  int         num       (Holder* h);

  class Iter {
    Holder*   _h;
    int       _ix;
    Entry*    _beg,
         *    _nxt;  /* status of list */
  public:
    void      start(Holder*);
    Entry*    operator++();
  };
};


/*----------------------------------------------------------------------
jjGraph Interface

TODO
----------------------------------------------------------------------*/

/* convenient hash functions */
int hash_str(const char *);

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

#define jjHash(id, _Holder, _Entry) \
class id##_class : public jj::Hash {  \
  int         hash_base (Entry *); \
  int         hash      (id##_##Entry *e) { return hash_base(e); } \
  int         cmp_base  (Entry *, Entry *);  \
  int         cmp       (id##_##Entry* e1, id##_##Entry* e2) { return cmp_base(e1, e2); }  \
                                        \
public: \
  void        add(_Holder *h, _Entry *e)  { jj::Hash::add((id##_##Holder *)h, (id##_##Entry *)e); } \
  void        del(_Holder *h, _Entry *e)  { jj::Hash::del((id##_##Holder *)h, (id##_##Entry *)e); } \
  _Entry*     sel(_Holder *h, _Entry *key){ return static_cast<_Entry* >(static_cast<id##_##Entry* >(jj::Hash::sel((id##_##Holder *)h, (id##_##Entry *)key))); } \
  void        put_stat(Holder* h)         { jj::Hash::put_stat((id##_##Holder *)h); }  \
  void        put_stat2(Holder* h)        { jj::Hash::put_stat((id##_##Holder *)h); }  \
  int         num(_Holder *h){ return jj::Hash::num((id##_##Holder *)h); }  \
                                                                            \
  class Iter : public jj::Hash::Iter {  \
  public: \
              Iter()            : jj::Hash::Iter() {} \
              Iter(_Holder* h)  { jj::Hash::Iter::start((id##_##Holder *)h); } \
    void      start(_Holder* h) { jj::Hash::Iter::start((id##_##Holder *)h); } \
    Entry*    operator++()      { return static_cast<_Entry *>(static_cast<id##_##Entry *>(jj::Hash::Iter::operator++())); } \
  };  \
};
//id##_class id;

#endif /* jj/pattern.h */
