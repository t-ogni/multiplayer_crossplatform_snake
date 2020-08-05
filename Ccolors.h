#include <iostream>
#include <string>

namespace colors {
	std::string Clear = "\033[0m";
	namespace FG {
		std::string Black = "\033[30m";
		std::string Red = "\033[31m";
		std::string Green = "\033[32m";
		std::string Yellow = "\033[33m";
		std::string Blue = "\033[34m";
		std::string Pink = "\033[35m";
		std::string Cyan = "\033[36m";
		std::string White = "\033[37m";
	}
	namespace BG {
		std::string Black = "\033[40m";
		std::string Red = "\033[41m";
		std::string Green = "\033[42m";
		std::string Yellow = "\033[43m";
		std::string Blue = "\033[44m";
		std::string Pink = "\033[45m";
		std::string Cyan = "\033[46m";
		std::string White = "\033[47m";
	}
	namespace Styles {
		std::string Bold = "\033[01m";
		std::string Underline = "\033[04m";
		std::string Blinking = "\033[05m";
		std::string Reversed = "\033[07m";
	}
} 

