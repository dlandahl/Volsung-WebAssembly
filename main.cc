
#include <iostream>
#include <fstream>
#include <array>

#define NDEBUG
#include <emscripten/bind.h>
#include <emscripten.h>

#include "Volsung.hh"

using namespace emscripten;
using namespace Volsung;

Program prog;

int main(int argc, char ** argv)
{
    set_debug_callback([] (std::string message) { std::cout << message << std::endl; });
    prog.configure_io(1, 2);
    
    Program::add_directive("length", [] (std::vector<TypedValue> arguments, Program*) {
        EM_ASM_({
            set_length($0);
        }, (float) arguments[0].get_value<Number>());
    });
}

bool parse(std::string code)
{
    Parser parser;
    parser.source_code = code;
    prog.reset();
    prog.create_user_object("start", 0, 1, nullptr, [] (const MultichannelBuffer&, MultichannelBuffer& output, std::any) {
        static bool do_thing = true;
        if (do_thing) output[0][0] = 1;
        do_thing = false;
    });
    const bool result = parser.parse_program(prog);

    if (result) {
        const bool mono = !prog.get_audio_object_raw_pointer<AudioOutputObject>("output")->is_connected(1);

        EM_ASM_({
            mono = $0;
        }, mono);
    }
    
    return result;
}

void run()
{
    MultichannelBuffer buffer = prog.run( { AudioBuffer::zero } );

    for (size_t n = 0; n < AudioBuffer::blocksize; n++) {
        EM_ASM_({
            add_to_data($0, $1);
        }, buffer[0][n], buffer[1][n]);
    }
}

EMSCRIPTEN_BINDINGS(my_module) {
    function("parse", &parse);
    function("run", &run);
}
