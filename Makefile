make:
	emcc --bind main.cc Volsung/src/*.cc -I Volsung/include/ -std=c++17 -O3 -s DISABLE_EXCEPTION_CATCHING=0
