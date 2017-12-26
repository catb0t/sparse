#!/bin/sh
util/premake5 gmake
make -j
make config=dist -j
bin/dbg/test_sparse
bin/dist/test_sparse