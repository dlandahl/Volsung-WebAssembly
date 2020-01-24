
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
	debug_callback = [] (std::string message) { std::cout << message; };
	prog.configure_io(1, 2);
	Program::add_directive("length", [] (std::vector<TypedValue> arguments, Program*) {
		EM_ASM_({
			set_length($0);
		}, (float) arguments[0].get_value<Number>());
	});

	Program::add_directive("log", [] (std::vector<TypedValue> arguments, Program*) {
		if (arguments[0].is_type<Number>()) Volsung::log( (Text) arguments[0].get_value<Number>());
		if (arguments[0].is_type<Text>()) Volsung::log(arguments[0].get_value<Text>());
		if (arguments[0].is_type<Sequence>()) {
			Volsung::log((Text) arguments[0].get_value<Sequence>());
		}
		std::cout << std::flush;
	});

	Program::add_directive("mono", [] (std::vector<TypedValue> arguments, Program*) {
		EM_ASM_({
			set_mono();
		});
	});

	Program::add_directive("stereo", [] (std::vector<TypedValue> arguments, Program*) {
		EM_ASM_({
			unset_mono();
		});
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
	return parser.parse_program(prog);
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
