# sparse

---

sparse number arrays (bignum will be supported later) implemented by run-length-encoding

designed to trade speed of random access for reduced memory consumption for huge arrays which contain mostly zeroes

for the implementation of hashtables

on linux, use `util/premake5 gmake`to build for GNU make

on windows, use `util/premake5.exe vs2013` to build for VS2013

on OSX, use `util/premake-osx gmake` to build for GNU make

see https://github.com/premake/premake-core/wiki for more information