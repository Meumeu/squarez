#!/bin/sh

set -e

git pull

rm -rf build
mkdir build
cd build

cmake .. -DBUILD_QML_CLIENT=OFF -DBUILD_TEST=OFF -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release 
cpack -G DEB

sudo dpkg -i squarez-*-server.deb
sudo systemctl daemon-reload
sudo systemctl restart squarezd
