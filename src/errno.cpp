/*C ------------------------------------------------------------------
= NAME
  jj::Errno  - jj Error handling library

= SYNOPSIS

= DESCRIPTION

jj:Errno represents current process's error status by 32bit global variable
'g_errno' just similar to errno(3).

  b31 b30 b29 ... b16 b15 ... b0
   |   |   |       |
   |---+   |-------+
   |       |
   |       0      System error
   |    (other)   sub module ID
   |
   00 normal
   01 warning
   1* error

MLB b31==1 indicates an error. b30 could be any value(0 or 1).

b29..b16 14bit represents total 1,6384 submodules.
b29..b16 == 0 is system module.

b15..b0 indicates any error code which can be defined by each sub module.

= AUTHOR
Fuminori Ido, ido-gh@wtech.jp

= SEE ALSO

= BUGS
--------------------------------------------------------------------*/

#include "jj/errno.h"

namespace jj {

/*C ------------------------------------------------------------------
Local Type definitions
--------------------------------------------------------------------*/
class _Errno {
public:
  short   _errno;
  short   _eh;
};


/*C ------------------------------------------------------------------
Pattern class implementation
  some global variable constructor may use pattern class so
  that pattern class implementation is necessary before global
  variable definitions.
--------------------------------------------------------------------*/

/*C ------------------------------------------------------------------
external variable declarations
--------------------------------------------------------------------*/
const char*   g_err_file = 0;
unsigned int  g_err_line = 0;

static const unsigned short
  ERR     = 0x8000;

/*C ------------------------------------------------------------------
default callback for onerr
--------------------------------------------------------------------*/
#include <stdio.h>

static void default_cb(){
  if( g_err_file ){
    fprintf(stderr, "jjError(%x) at %s(%d)\n"
        ,errno()
        ,g_err_file, g_err_line);
  }else{
    fprintf(stderr, "jjError(%x)\n", errno());
  }
}

/*C ------------------------------------------------------------------
static global variable definitions
--------------------------------------------------------------------*/
static unsigned int g_errno   = 0;
static _Errno&    _errno  = (_Errno &)g_errno;
static void         (*g_cb)() = default_cb;

/*C ------------------------------------------------------------------
jjErrno class
--------------------------------------------------------------------*/
static short jjerr_init(){
  static short count = 1;
  return count++;
}

Errno::Errno(){
  _eh = jjerr_init();
}

Errno::~Errno(){
  //jjerr_term(_eh);
}

void Errno::set(short errno){
  _errno._eh    = _eh | ERR;
  _errno._errno = errno;
}


/*C ------------------------------------------------------------------
APIs
--------------------------------------------------------------------*/
unsigned int errno() { return g_errno; }

void raise(Errno &eh, short errno){
  eh.set(errno);
}

void raise2_(Errno &eh, short errno, const char *f, unsigned int l){
  eh.set(errno);
  g_err_file  = f;
  g_err_line  = l;
  (*g_cb)();
}

void errok() { g_errno = 0; }

void set_onerr(void (*cb)()){
  g_cb = cb;
}

}; // jj
