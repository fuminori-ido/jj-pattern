#!/bin/bash

  rm -rf \
      .libs/ \
      aclocal.m4 autom4te.cache/ \
      autoscan.log \
      config.h.in config.log configure configure.scan \
      COPYING INSTALL \
      depcomp install-sh \
      libjj.la \
      ltmain.sh \
      m4/ \
      missing
  aclocal
  autoconf
  autoreconf -i
  touch NEWS README AUTHORS ChangeLog
  automake --add-missing
  ./configure
  make
