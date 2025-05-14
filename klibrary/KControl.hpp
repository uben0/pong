#ifndef KControl_HEADER_DEFINED
#define KControl_HEADER_DEFINED

#include "KKey.hpp"

/// \brief Class that store keyboard and mouse state.
///
class KControl {
	const unsigned char * const key;
public:
	/// \brief Struct storing mouse state.
	///
	struct {
		/// \brief X coordinate of the mouse.
		///
		int x;
		/// \brief Y coordinate of the mouse.
		///
		int y;
		/// \brief Whether left button are pressed or not.
		///
		bool l;
		/// \brief Whether middle button are pressed or not.
		///
		bool m;
		/// \brief Whether right button aren pressed or not.
		///
		bool r;
	} mouse;

	/// \brief Initialize the class.
	///
	KControl();

	/// \brief Take a snapshot of the keyboard and mouse state.
	///
	void update();

	/// \brief Return whether a key is pushed or not.
	///
	bool operator[](KKey::Id keyId) const;
};

#endif
