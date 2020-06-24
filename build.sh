
echo "Sourcing Emscripten"
source ../emsdk/emsdk_env.sh

echo "Building"
emcc --bind main.cc Volsung/src/*.cc -I Volsung/include/ -std=c++17 -O3 -s DISABLE_EXCEPTION_CATCHING=0 --preload-file Volsung/test/test_programs@/ --preload-file Volsung/extra/standard_library@/ -s ALLOW_MEMORY_GROWTH=1

echo "Moving"
mv a.out.* website
