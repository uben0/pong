#ifndef KKey_HEADER_DEFINED
#define KKey_HEADER_DEFINED

#include <iostream>
#include "KLib_SDL.hpp"

/// \brief A namespace regrouping keys.
///
namespace KKey {
	/// \brief The type of a key.
	///
	enum Id {
		/// \brief Definition of the None key.
		///
		None = -1,
		/// \brief Definition of the Digit0 key.
		///
		Digit0 = SDLK_0,
		/// \brief Definition of the Digit1 key.
		///
		Digit1 = SDLK_1,
		/// \brief Definition of the Digit2 key.
		///
		Digit2 = SDLK_2,
		/// \brief Definition of the Digit3 key.
		///
		Digit3 = SDLK_3,
		/// \brief Definition of the Digit4 key.
		///
		Digit4 = SDLK_4,
		/// \brief Definition of the Digit5 key.
		///
		Digit5 = SDLK_5,
		/// \brief Definition of the Digit6 key.
		///
		Digit6 = SDLK_6,
		/// \brief Definition of the Digit7 key.
		///
		Digit7 = SDLK_7,
		/// \brief Definition of the Digit8 key.
		///
		Digit8 = SDLK_8,
		/// \brief Definition of the Digit9 key.
		///
		Digit9 = SDLK_9,
		/// \brief Definition of the A key.
		///
		A = SDLK_a,
		/// \brief Definition of the B key.
		///
		B = SDLK_b,
		/// \brief Definition of the C key.
		///
		C = SDLK_c,
		/// \brief Definition of the D key.
		///
		D = SDLK_d,
		/// \brief Definition of the E key.
		///
		E = SDLK_e,
		/// \brief Definition of the F key.
		///
		F = SDLK_f,
		/// \brief Definition of the G key.
		///
		G = SDLK_g,
		/// \brief Definition of the H key.
		///
		H = SDLK_h,
		/// \brief Definition of the I key.
		///
		I = SDLK_i,
		/// \brief Definition of the J key.
		///
		J = SDLK_j,
		/// \brief Definition of the K key.
		///
		K = SDLK_k,
		/// \brief Definition of the L key.
		///
		L = SDLK_l,
		/// \brief Definition of the M key.
		///
		M = SDLK_m,
		/// \brief Definition of the N key.
		///
		N = SDLK_n,
		/// \brief Definition of the O key.
		///
		O = SDLK_o,
		/// \brief Definition of the P key.
		///
		P = SDLK_p,
		/// \brief Definition of the Q key.
		///
		Q = SDLK_q,
		/// \brief Definition of the R key.
		///
		R = SDLK_r,
		/// \brief Definition of the S key.
		///
		S = SDLK_s,
		/// \brief Definition of the T key.
		///
		T = SDLK_t,
		/// \brief Definition of the U key.
		///
		U = SDLK_u,
		/// \brief Definition of the V key.
		///
		V = SDLK_v,
		/// \brief Definition of the W key.
		///
		W = SDLK_w,
		/// \brief Definition of the X key.
		///
		X = SDLK_x,
		/// \brief Definition of the Y key.
		///
		Y = SDLK_y,
		/// \brief Definition of the Z key.
		///
		Z = SDLK_z,
		/// \brief Definition of the Backspace key.
		///
		Backspace = SDLK_BACKSPACE,
		/// \brief Definition of the Return key.
		///
		Return    = SDLK_RETURN,
		/// \brief Definition of the Left key.
		///
		Left      = SDLK_LEFT,
		/// \brief Definition of the Right key.
		///
		Right     = SDLK_RIGHT,
		/// \brief Definition of the Up key.
		///
		Up        = SDLK_UP,
		/// \brief Definition of the Down key.
		///
		Down      = SDLK_DOWN,
		/// \brief Definition of the LShift key.
		///
		LShift    = SDLK_LSHIFT,
		/// \brief Definition of the RShift key.
		///
		RShift    = SDLK_RSHIFT,
		/// \brief Definition of the LCtrl key.
		///
		LCtrl     = SDLK_LCTRL,
		/// \brief Definition of the RCtrl key.
		///
		RCtrl     = SDLK_RCTRL,
		/// \brief Definition of the LAlt key.
		///
		LAlt      = SDLK_LALT,
		/// \brief Definition of the RAlt key.
		///
		RAlt      = SDLK_RALT,
		/// \brief Definition of the Space key.
		///
		Space     = SDLK_SPACE,
		/// \brief Definition of the Tab key.
		///
		Tab       = SDLK_TAB,
		/// \brief Definition of the Escape key.
		///
		Escape    = SDLK_ESCAPE,
		/// \brief Definition of the LBracket key.
		///
		LBracket  = SDLK_LEFTBRACKET,
		/// \brief Definition of the RBracket key.
		///
		RBracket  = SDLK_RIGHTBRACKET,
		/// \brief Definition of the LMeta key.
		///
		LMeta     = SDLK_LGUI,
		/// \brief Definition of the RMeta key.
		///
		RMeta     = SDLK_RGUI,
		/// \brief Definition of the F1 key.
		///
		F1  = SDLK_F1,
		/// \brief Definition of the F2 key.
		///
		F2  = SDLK_F2,
		/// \brief Definition of the F3 key.
		///
		F3  = SDLK_F3,
		/// \brief Definition of the F4 key.
		///
		F4  = SDLK_F4,
		/// \brief Definition of the F5 key.
		///
		F5  = SDLK_F5,
		/// \brief Definition of the F6 key.
		///
		F6  = SDLK_F6,
		/// \brief Definition of the F7 key.
		///
		F7  = SDLK_F7,
		/// \brief Definition of the F8 key.
		///
		F8  = SDLK_F8,
		/// \brief Definition of the F9 key.
		///
		F9  = SDLK_F9,
		/// \brief Definition of the F10 key.
		///
		F10 = SDLK_F10,
		/// \brief Definition of the F11 key.
		///
		F11 = SDLK_F11,
		/// \brief Definition of the F12 key.
		///
		F12 = SDLK_F12,
		/// \brief Definition of the F13 key.
		///
		F13 = SDLK_F13,
		/// \brief Definition of the F14 key.
		///
		F14 = SDLK_F14,
		/// \brief Definition of the F15 key.
		///
		F15 = SDLK_F15,
		/// \brief Definition of the F16 key.
		///
		F16 = SDLK_F16,
		/// \brief Definition of the F17 key.
		///
		F17 = SDLK_F17,
		/// \brief Definition of the F18 key.
		///
		F18 = SDLK_F18,
		/// \brief Definition of the F19 key.
		///
		F19 = SDLK_F19,
		/// \brief Definition of the F20 key.
		///
		F20 = SDLK_F20,
		/// \brief Definition of the F21 key.
		///
		F21 = SDLK_F21,
		/// \brief Definition of the F22 key.
		///
		F22 = SDLK_F22,
		/// \brief Definition of the F23 key.
		///
		F23 = SDLK_F23,
		/// \brief Definition of the F24 key.
		///
		F24 = SDLK_F24
	};

	/// \brief Convert a key to a const char array.
	///
	const char* to_c_str(Id keyId);

	/// \brief Cenvert a const char array to a key.
	///
	Id from_c_str(const char* str);
}

/// \biref Operator >> for output stream.
///
std::ostream& operator<<(std::ostream& ostream, const KKey::Id& key);

/// \biref Operator << for input stream.
///
std::istream& operator>>(std::istream& istream,       KKey::Id& key);

#endif
