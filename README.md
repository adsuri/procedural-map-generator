cmake -S . -B build -DCMAKE_CXX_COMPILER=g++

cmake --build build --target release -j$(nproc)
cmake --build build --target debug -j$(nproc)

cmake --build build --target clean