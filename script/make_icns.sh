#!/bin/bash

mkdir -p bitcoin.iconset
cp src/qt/res/icons/Graphcoin-16.png bitcoin.iconset/icon_16x16.png
cp src/qt/res/icons/Graphcoin-32.png bitcoin.iconset/icon_32x32.png
cp src/qt/res/icons/Graphcoin-128.png bitcoin.iconset/icon_128x128.png
cp src/qt/res/icons/Graphcoin.png bitcoin.iconset/icon_256x256.png
iconutil -c icns bitcoin.iconset
mv bitcoin.icns src/qt/res/icons/bitcoin.icns 
rm -rf bitcoin.iconset/
