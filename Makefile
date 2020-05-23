make:
	emcc --bind main.cc Volsung/src/*.cc -I Volsung/include/ -std=c++17 -O1 -s DISABLE_EXCEPTION_CATCHING=0 --preload-file files -s ALLOW_MEMORY_GROWTH=1
