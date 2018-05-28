#!/bin/bash
# Requirements: Ubuntu (tested on 14.04)
#				Access to Graphcoin repository	
#
# Tested with MXE commit # 430de95
#
MXE_PATH="$PWD/mxe"
SRC_PATH="$PWD/graphcoin-source"

while getopts ":lh" option; do
	case $option in
		l ) exec > >(tee -i build-windows.log);;
		h ) echo "Usage: windows-build.sh [OPTION]"
			echo "-l: Log output to build-windows.log"
			echo "-h: Display this message"
			;;
	esac
done

function main {
	if [[ $PWD == "graphcoin-source" ]]; then
		cd ..
		SKIP_SRC=true
	else
		SKIP_SRC=false
	fi

	# Call necessary functions
	# TODO keep track of what has been done so script can be resumed if anything fails
	install_deps

	get_mxe

	if [[ "$SKIP_SRC" != true ]]; then
		get_source
	fi

	if [[ -d mxe ]]; then
		read -p "*** Mxe is already downloaded. Do you want to make MXE targets anyway [y/n]? " -n 1 -r

		if [[ $REPLY =~ ^[Yy]$ ]]; then
			make_mxe_targets
		fi
	else
		make_mxe_targets
	fi

	export PATH=$PATH:$MXE_PATH/usr/bin

	make_leveldb

	make_bdb

	make_miniupnpc

	get_secp

	qmake

	make_release

	if [[ "$SKIP_SRC" == true ]]; then
		cd graphcoin-src
	fi
}

function install_deps  {
	echo "Checking for dependencies..."
	DEPENDENCIES=(p7zip-full autoconf automake autopoint bash bison bzip2 cmake flex gettext git g++ gperf intltool libffi-dev libtool libltdl-dev libssl-dev libxml-parser-perl make openssl patch perl pkg-config python ruby scons sed unzip wget xz-utils libgtk2.0-dev)


	for dep in "${DEPENDENCIES[@]}"; do
		INSTALLED=$(dpkg-query -W --showformat='${Status}\n' $dep|grep "install ok installed")

		if [[ "" == "$INSTALLED" ]]; then
			TO_INSTALL+=($dep)
		else
			echo "Already installed: $dep"
		fi
	done

	if [[ 0 -ne ${#TO_INSTALL[@]} ]]; then
		sudo apt-get install "${TO_INSTALL[@]}"
	else
		echo "Dependencies already installed, skipping.."
	fi
}

function get_mxe  {
	echo -e "\n[ RETRIEVING mxe ]\n"
	echo "Attempting to clone MXE"
	if [[ ! -d "mxe" ]]; then
		git clone https://github.com/mxe/mxe

		if [[ $? -ne 0 ]]; then
			echo "Cloning failed."
			exit 1
		fi
	else
		echo -e "\nIt appears that MXE may already be installed in this directory."
		read -p " Would you like to attempt to clone any way? [y/n]: " -n 1 -r
		if [[ $REPLY =~ ^[Yy]$ ]]; then
			git clone https://github.com/mxe/mxe
		fi
		echo -e "\n"
	fi
	echo -e "\n[ RETRIEVED mxe ]\n"
}

function get_source  {
	echo -e "\n[ RETRIEVING graphcoin ]\n"
	echo "Attempting to clone Graphcoin"
	if [[ ! -d "graphcoin-source" ]]; then
		git clone ssh://git@159.65.99.219:/home/git/graphcoin-source.git

		if [[ $? -ne 0 ]]; then
			echo "Cloning failed. Do you have access to this repository? (check SSH keys)"
			exit1
		fi

	else
		echo -e "\nIt appears that Graphcoin may already be installed in this directory."
		read -p " Would you like to attempt to clone any way? [y/n]: " -n 1 -r

		if [[ $REPLY =~ ^[Yy]$ ]]; then
			git clone https://github.com/mxe/mxe
		fi
		echo -e "\n"
	fi
	echo -e "\n[ RETRIEVED graphcoin ]\n"
}

function make_mxe_targets  {
	echo -e "\n[ MAKING mxe targets ]\n"
	# Modify openssl.mk
	cd $MXE_PATH
	sed -i 's/$(PKG)_VERSION .*/$(PKG)_VERSION  := 1.0.1p/' src/openssl.mk
	sed -i 's/$(PKG)_CHECKSUM .*/$(PKG)_CHECKSUM := bd5ee6803165c0fb60bbecbacacf244f1f90d2aa0d71353af610c29121e9b2f1/' src/openssl.mk
	sed -i '/--with-zlib/d' src/postgresql.mk
	sed -i 's/CFLAGS="-DSSL_library_init=OPENSSL_init_ssl" \\/CFLAGS="-DSSL_library_init=SSL_library_init" \\/' src/postgresql.mk

	make MXE_TARGETS="i686-w64-mingw32.static" openssl
	make MXE_TARGETS="i686-w64-mingw32.static" boost
	make MXE_TARGETS="i686-w64-mingw32.static" qttools
	export PATH=$PATH:$MXE_PATH/usr/bin
	echo -e "\n[ MADE mxe targets ]\n"

	cd -
}

function make_leveldb  {
	echo -e "\n[ MAKING leveldb ]\n"

	cd $SRC_PATH/src/leveldb
	TARGET_OS=NATIVE_WINDOWS make libleveldb.a libmemenv.a CC=$MXE_PATH/usr/bin/i686-w64-mingw32.static-gcc CXX=$MXE_PATH/usr/bin/i686-w64-mingw32.static-g++
	cd -

	echo -e "\n[ MADE leveldb ]\n"
}

function make_bdb {
	echo -e "\n[ MAKING berkeleydb ]\n"
	if [[ ! -d db-5.3.28 ]]; then
		wget http://download.oracle.com/berkeley-db/db-5.3.28.tar.gz
		tar zxvf db-5.3.28.tar.gz
	else
		echo "db-5.3.28 appears to be downloaded..."
		read -p " Would you like to build it? [y/n]: " -n 1 -r
		if [[ $REPLY =~ ^[Nn]$ ]]; then
			return
		fi
	fi


	cd db-5.3.28
	sed -i "s/WinIoCtl.h/winioctl.h/g" src/dbinc/win_db.h
	mkdir build_mxe
	cd build_mxe
	CC=$MXE_PATH/usr/bin/i686-w64-mingw32.static-gcc \
	CXX=$MXE_PATH/usr/bin/i686-w64-mingw32.static-g++ \
		../dist/configure \
		--disable-replication \
		--enable-mingw \
		--enable-cxx \
		--host x86 \
		--prefix=$MXE_PATH/usr/i686-w64-mingw32.static
	make
	make install
	cd -

	echo -e "\n[ MADE berkeleydb ]\n"
}

function make_miniupnpc {
	echo -e "\n[ MAKING miniupnpc ]\n"
	if [[ ! -d  miniupnpc-1.6.20120509 ]]; then
		wget http://miniupnp.free.fr/files/miniupnpc-1.6.20120509.tar.gz
		tar zxvf miniupnpc-1.6.20120509.tar.gz
	else
		echo -e "miniupnpc appears to be downloaded...\n"
		read -p " Would you like to build it? [y/n]: " -n 1 -r
		if [[ $REPLY =~ ^[Nn]$ ]]; then
			return [0]
		fi
	fi

	cd miniupnpc-1.6.20120509
	CC=$MXE_PATH/usr/bin/i686-w64-mingw32.static-gcc \
	AR=$MXE_PATH/usr/bin/i686-w64-mingw32.static-ar \
	CFLAGS="-DSTATICLIB -I$MXE_PATH/usr/i686-w64-mingw32.static/include" \
	LDFLAGS="-L$MXE_PATH/usr/i686-w64-mingw32.static/lib" \
	make libminiupnpc.a

	mkdir $MXE_PATH/usr/i686-w64-mingw32.static/include/miniupnpc
	cp *.h $MXE_PATH/usr/i686-w64-mingw32.static/include/miniupnpc
	cp libminiupnpc.a $MXE_PATH/usr/i686-w64-mingw32.static/lib
	cd -

	echo -e "\n[ MADE miniupnpc ]\n"
}

function get_secp {
	echo -e "\n[ RETRIEVING miniupnpc ]\n"
	if [[ ! -f secp256k1.tar.gz ]]; then
		FILEID=0B5j8d4FSc7drYzlpMVJKbVdISVk
		FILENAME=secp256k1.tar.gz
		wget --load-cookies /tmp/cookies.txt "https://docs.google.com/uc?export=download&confirm=$(wget --quiet --save-cookies /tmp/cookies.txt --keep-session-cookies --no-check-certificate 'https://docs.google.com/uc?export=download&id=$FILEID' -O- | sed -rn 's/.*confirm=([0-9A-Za-z_]+).*/\1\n/p')&id=$FILEID" -O $FILENAME && rm -rf /tmp/cookies.txt

		if [[ -f secp256k1.tar.gz ]]; then
			tar xzvf secp256k1.tar.gz
		else
			echo -e "\nUnable to retrieve secp256k1 from Google drive."
			echo "Please download and unpack it manually"
		fi
	else
		echo "Found secp256k1.tar.gz. Unpacking..."
		tar xzvf secp256k1.tar.gz
	fi
	echo -e "\n[ RETRIEVED miniupnpc ]\n"
}

function qmake  {
	 echo -e "\n[ RUNNING qmake... ]\n"
	 sed -i "s*/root/secp256k1*${PWD}/secp256k1*g" $SRC_PATH/qt.pro

	export PATH=$PATH:$MXE_PATH/usr/bin

	 cd $SRC_PATH
	 MXE_INCLUDE_PATH=$MXE_PATH/usr/i686-w64-mingw32.static/include
	 MXE_LIB_PATH=$MXE_PATH/usr/i686-w64-mingw32.static/lib

	 i686-w64-mingw32.static-qmake-qt5 \
	 BOOST_LIB_SUFFIX=-mt \
	 BOOST_THREAD_LIB_SUFFIX=_win32-mt \
	 BOOST_INCLUDE_PATH=$MXE_INCLUDE_PATH/boost \
	 BOOST_LIB_PATH=$MXE_LIB_PATH \
	 OPENSSL_INCLUDE_PATH=$MXE_INCLUDE_PATH/openssl \
	 OPENSSL_LIB_PATH=$MXE_LIB_PATH \
	 BDB_INCLUDE_PATH=$MXE_INCLUDE_PATH \
	 BDB_LIB_PATH=$MXE_LIB_PATH \
	 MINIUPNPC_INCLUDE_PATH=$MXE_INCLUDE_PATH \
	 MINIUPNPC_LIB_PATH=$MXE_LIB_PATH \
	 QMAKE_LRELEASE=$MXE_PATH/usr/i686-w64-mingw32.static/qt5/bin/lrelease qt.pro
	 cd -

	 echo -e "\n[ RAN qmake ]\n"
}

function make_release  {
	cd $SRC_PATH
	if [[ -f Makefile.Release ]]; then
		sed -i 's*build/16 bitcoin-qt_res.o*16 -lpthread build/bitcoin-qt_res.o*' Makefile.Release
	else
		echo "No Makefile.Release file found. Aborting.."
		exit 1
	fi

	make -f Makefile.Release
	cd -

	if [[ $? == 0  && -f $SRC_PATH/release/Graphcoin-qt.exe ]]; then
		echo "*** Successfully made Graphcoin! It's stored in release/Graphcoin-qt.exe. Enjoy! ***"
		ls $SRC_PATH/release/Graphcoin-qt.exe
	else
		echo "*** Something may have went wrong while building Graphcoin-qt.exe "
	fi
}


main
