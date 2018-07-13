
Debian
====================
This directory contains files used to package graphcoind/graphcoin-qt
for Debian-based Linux systems. If you compile graphcoind/graphcoin-qt yourself, there are some useful files here.

## graphcoin: URI support ##


graphcoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install graphcoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your graphcoinqt binary to `/usr/bin`
and the `../../share/pixmaps/graphcoin128.png` to `/usr/share/pixmaps`

graphcoin-qt.protocol (KDE)

