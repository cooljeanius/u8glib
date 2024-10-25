#!/bin/sh

set -ex

PROGNAME="$0"
DN_EXEC=$(dirname "${PROGNAME}")
if [ ! "${DN_EXEC}" = "" ]; then
    DN_EXEC="${DN_EXEC}/"
else
    DN_EXEC="./"
fi

# shellcheck disable=SC2086
${DN_EXEC}autoclean.sh

rm -f configure

rm -f Makefile.in

rm -f config.guess
rm -f config.sub
rm -f install-sh
rm -f missing
rm -f depcomp

# shellcheck disable=SC2050
if [ 0 = 1 ]; then
autoscan
else

touch NEWS
touch README
touch AUTHORS
if test -z "$(grep "$(whoami)" AUTHORS)"; then whoami >> AUTHORS; fi
touch THANKS
if test -z "$(grep "$(whoami)" THANKS)"; then
  echo "Thanks, $(whoami)" >> THANKS;
fi
touch ChangeLog
touch config.h.in

LIBTOOLIZE=false
if test -x "$(which libtoolize)"; then
  LIBTOOLIZE=libtoolize
elif test -x "$(which glibtoolize)"; then
  LIBTOOLIZE=glibtoolize
fi
${LIBTOOLIZE} --force --copy --install --automake
aclocal -I m4
automake --copy --add-missing --gnits -Wno-portability
autoconf -Wno-obsolete

autoreconf -fvi -Wall -Wno-obsolete
if [ $? -eq 2 ]; then
  autoreconf # run twice to get rid of 'ltmain.sh not found'
fi

# shellcheck disable=SC2050
if [ 1 = 1 ]; then
rm -rf build
mkdir -p build
cd build
fi

fi
