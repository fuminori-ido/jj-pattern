
#ifndef jjerrno_h
#define jjerrno_h

namespace jj {

class Errno {
 short  _eh;
public:
        Errno();
       ~Errno();
  void  set(short errno);
};

extern const char*  g_err_file;
extern unsigned int g_err_line;

unsigned int  errno();
void          raise(Errno &eh, short errno);
void          raise2_(Errno &eh, short errno, const char *, unsigned int);
void          errok();
void          err_put();
void          set_onerr(void (*)());

#define jjonerr     if( errno() )
#define jjraise2(e,n) raise2_(e,n,__FILE__,__LINE__)

}; // jj

#endif /* jjerrno.h */