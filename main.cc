
#include <iostream>
#include <fstream>
#include <array>

#include "Volsung.hh"
#include <emscripten/bind.h>
#include <emscripten.h>

using namespace emscripten;
using namespace Volsung;

Program prog;

int main(int argc, char ** argv)
{
	debug_callback = [] (std::string message) { std::cout << message; };
	prog.configure_io(1, 1);
	Program::add_directive("length", [] (std::vector<TypedValue> arguments, Program*) {
		EM_ASM_({
			set_length($0);
		}, arguments[0].get_value<float>());
	});

	Program::add_directive("mono", [] (std::vector<TypedValue> arguments, Program*) {
		if (arguments[0].get_value<float>()) {
			EM_ASM_({
				set_mono();
			});
			prog.configure_io(1, 1);
		}
		else {
			EM_ASM_({
				unset_mono();
			});
			prog.configure_io(1, 2);
		}
		prog.reset();
	});
}

void parse(std::string code)
{
	Parser parser;
	parser.source_code = code;
	parser.parse_program(prog);
}

void run()
{
	auto y = prog.run( {0, 0} );
	EM_ASM_({
		add_to_data($0, $1);
	}, y[0], y[1]);
}

EMSCRIPTEN_BINDINGS(my_module) {
	function("parse", &parse);
    function("run", &run);
}
