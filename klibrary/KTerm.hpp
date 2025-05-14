#ifndef KTerm_HEADER_DEFINED
#define KTerm_HEADER_DEFINED
#include <iostream>

/// \brief A namespace regrouping macro for terminal formating.
///
namespace KTerm {
	/// \brief Code to reset formating in terminal.
	///
	constexpr const char Def[]     = "\e[0m";
	/// \brief Code to enable bold text in terminal.
	///
	constexpr const char Bold[]    = "\e[1m";
	/// \brief Code to enable dim text in terminal.
	///
	constexpr const char Dim[]     = "\e[2m";
	/// \brief Code to enable inverted color in terminal.
	///
	constexpr const char Invert[]  = "\e[7m";
	/// \brief Code to disable bold text in terminal.
	///
	constexpr const char UBold[]   = "\e[21m";
	/// \brief Code to disable dim text in terminal.
	///
	constexpr const char UDim[]    = "\e[22m";
	/// \brief Code to disable inverted color in terminal.
	///
	constexpr const char UInvert[] = "\e[27m";
	/// \brief Code to disable colored text in terminal.
	///
	constexpr const char UColor[]  = "\e[39m";
	/// \brief Code to print black text in terminal.
	///
	constexpr const char Black[]   = "\e[30m";
	/// \brief Code to print red text in terminal.
	///
	constexpr const char Red[]     = "\e[31m";
	/// \brief Code to print green text in terminal.
	///
	constexpr const char Green[]   = "\e[32m";
	/// \brief Code to print yellow text in terminal.
	///
	constexpr const char Yellow[]  = "\e[33m";
	/// \brief Code to print blue text in terminal.
	///
	constexpr const char Blue[]    = "\e[34m";
	/// \brief Code to print magenta text in terminal.
	///
	constexpr const char Magenta[] = "\e[35m";
	/// \brief Code to print cyan text in terminal.
	///
	constexpr const char Cyan[]    = "\e[36m";
	/// \brief Code to print gray text in terminal.
	///
	constexpr const char Gray[]    = "\e[37m";

	/// \brief Code to set colored text in terminal.
	///
	/// \tparam colorId the id of the color (0-255)
	std::string Color(unsigned char colorId);
}

#endif
