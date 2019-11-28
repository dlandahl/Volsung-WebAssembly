make:
	emcc --bind main.cc Volsung/src/*.cc -I Volsung/include/ -std=c++17 -O0
