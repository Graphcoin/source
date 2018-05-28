Graphcoin-qt: Qt5 GUI for Graphcoin
===============================

Linux
-------
https://github.com/Graphcoin/Source/blob/master/doc/build-unix.md 

Windows
--------
https://github.com/Graphcoin/Source/blob/master/doc/build-msw.md

Mac OS X
--------
https://github.com/Graphcoin/Source/blob/master/doc/build-osx.md

make sure qmake is installed.  Then:

$ qmake "RELEASE=1"
$ make

To bundle linked .dylib libraries into app and build .dmg
$ macdeployqt Graphcoin-Qt.app/ -dmg

check correct libs linked:
otool -L Graphcoin-Qt.app/Contents/MacOS/Graphcoin-Qt

