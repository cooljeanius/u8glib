#!/bin/sh

set -ex

for auxfile in $0 autogen.sh config.guess config.rpath config.status config.sub configure depcomp install-sh missing; do
  if test -e ${auxfile}; then chmod 755 ${auxfile}; fi
done

if test -e Makefile; then make clean; fi
if test -e Makefile; then make distclean; fi

rm -f *.o

rm -f config.log
rm -f config.status
rm -f config.h
rm -f Makefile
rm -rf ./autom4te.cache/ || rmdir autom4te.cache
rm -f gmon.out

