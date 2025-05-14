#ifndef KTool_HEADER_DEFINED
#define KTool_HEADER_DEFINED

#include <cassert>
#include <algorithm>

/// \brief A namespace regrouping tools.
///
namespace KTool {
	/// \brief Return the lenght of an array.
	///
	template<typename T>
	unsigned int lenof(const T& tab) {
		return sizeof tab / sizeof *tab;
	}

	/// \brief Convert an integer to a character number.
	///
	/// \tparam base the base of the wrote number
	/// \tparam T the type of the integer
	/// \param num the integer
	/// \param str the buffer to write the number on
	/// \param size the size of the buffer
	template<int base = 10, typename T>
	char* itoa(T num, char* str, unsigned int size) {
		assert(size >= 2 && base >= 2);

		unsigned int i = 0;
		bool isNegative = false;

		/* Handle 0 explicitely, otherwise empty string is printed for 0 */
		if (num == 0) {
			str[0] = '0';
			str[1] = '\0';
			return str;
		}

		// In standard itoa(), negative numbers are handled only with
		// base 10. Otherwise numbers are considered unsigned.
		if (num < 0 && base == 10) {
			isNegative = true;
			num = -num;
		}

		// Process individual digits
		while (num != 0 && i < size) {
			int rem = num % base;
			str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
			num = num/base;
		}

		// If number is negative, append '-'
		if (isNegative && i < size) {
			str[i++] = '-';
		}

		if (i >= size) {
			str[0] = '0';
			str[1] = '\0';
			return str;
		}
		str[i] = '\0'; // Append string terminator

		// Reverse the string
		int start = 0;
		int end = i - 1;
		while (start < end) {
			std::swap(*(str+start), *(str+end));
			start++;
			end--;
		}

		return str;
	}

	/// \brief Convert an integer to a character number.
	///
	/// \tparam base the base of the wrote number
	/// \tparam T the type of the integer
	/// \param num the integer
	/// \return It returns a static buffer.
	template<int base = 10, typename T>
	char* itoa(T num) {
		static char buff[32];
		itoa<base>(num, buff, 32);
		return buff;
	}

}

#endif
