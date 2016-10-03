
how to build gtest-1.7.0:

cd src-code
unzip gtest-1.7.0.zip
mkdir build
cd build
cmake ../gtest-1.7.0
make


how to install gtest-1.7.0:

cp libgtest.a libgtest_main.a ../../platorm-specific/lib
cd ..
cp -rf gtest-1.7.0/include ../platform-specific/include

