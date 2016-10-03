
how to build glog-0.3.4:

cd src-code
tar -xf glog-0.3.4.tar.gz
mkdir build
cd build
../glog-0.3.4/configure --prefix=`pwd`/install
make
make install


how to install glog-0.3.4:

cp install ../../platform-specific/

