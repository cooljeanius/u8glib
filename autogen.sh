#!/bin/sh

set -ex

PROGNAME="$0"
DN_EXEC=`dirname "${PROGNAME}"`
if [ ! "${DN_EXEC}" = "" ]; then
    DN_EXEC="${DN_EXEC}/"
else
    DN_EXEC="./"
fi

${DN_EXEC}autoclean.sh

rm -f configure

rm -f Makefile.in

rm -f config.guess
rm -f config.sub
rm -f install-sh
rm -f missing
rm -f depcomp

if [ 0 = 1 ]; then
autoscan
else

touch NEWS
touch README
touch AUTHORS
if test -z "$(grep $(whoami) AUTHORS)"; then echo "`whoami`" >> AUTHORS; fi
touch THANKS
if test -z "$(grep $(whoami) THANKS)"; then
  echo "Thanks, `whoami`" >> THANKS;
fi
touch ChangeLog
touch config.h.in

LIBTOOLIZE=false
if test -x "`which libtoolize`"; then
  LIBTOOLIZE=libtoolize
elif test -x "`which glibtoolize`"; then
  LIBTOOLIZE=glibtoolize
fi
${LIBTOOLIZE} --force --copy --install --automake
aclocal
automake --copy --add-missing --gnits
autoconf

autoreconf -fvi -Wall
if [ $? -eq 2 ]; then
  autoreconf # run twice to get rid of 'ltmain.sh not found'
fi

if [ 1 = 1 ]; then
rm -rf build
mkdir -p build
cd build
fi

fi
