set -eE
OS="$1"
BUILD_TYPE="$2"
C_COMPILER="$3"
mkdir build
cd $_
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_C_COMPILER="$C_COMPILER"
cmake --build . -j $(nproc)