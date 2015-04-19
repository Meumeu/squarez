#!/bin/sh

set -e

rm -rf build_web
mkdir build_web
cd build_web
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/usr/lib/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_INSTALL_PREFIX=/
make -j8
cpack -G "TGZ"

scp squarez-*.tar.gz meumeu.org:squarez-current.tar.gz
ssh meumeu.org "rm -rf /data/www/squarez/* ; tar xzf squarez-current.tar.gz -C /data/www/squarez"
