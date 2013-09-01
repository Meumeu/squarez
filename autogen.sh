
aclocal || exit 1
autoheader || exit 2
autoconf  || exit 3
libtoolize --copy || exit 4
automake --add-missing --copy || exit 5
