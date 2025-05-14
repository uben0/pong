#ifndef KEvent_HEADER_DEFINED
#define KEvent_HEADER_DEFINED

#include <set>
#include "KLib_SDL.hpp"
#include "KKey.hpp"

class KWindow;
/// \brief Class managing events ocurring on window.
///
class KEvent {
	friend class KWindow;
	friend class KLib_SDL::Linker;

	std::multiset<KKey::Id> m_key;
	struct {
		int x, y;
		bool l, m, r;
	} m_mouse;
	bool m_close;
	bool m_quit;

	KEvent();
public:
	/// \brief Multiset storing pressed keys.
	///
	std::multiset<KKey::Id> key;
	/// \brief Struct storing mouse events.
	///
	struct {
		/// \brief X coordinate where a clic occured.
		///
		int x;
		/// \brief Y coordinate where a clic occured.
		///
		int y;
		/// \brief Whether left button have been pressed or not.
		///
		bool l;
		/// \brief Whether middle button have been pressed or not.
		///
		bool m;
		/// \brief Whether right button have been pressed or not.
		///
		bool r;
	} mouse;
	/// \brief Whether close event have been received or not.
	///
	bool close;
	/// \brief Whether quit event have been received or not.
	///
	bool quit;

	/// \brief Update the class with new events.
	///
	/// Previous events are cleared.
	void update();

	/// \brief Get a pressed key.
	///
	/// It does not remove the returned key from the class.
	KKey::Id get_key() const;

	/// \brief Get a pressed key.
	///
	/// It removes the returned key from the class.
	KKey::Id pop_key();

	/// \brief Add a key to the pressed key.
	///
	void add_key(KKey::Id keyId);

	/// \brief Operator to test whether a key have been pressed or not.
	///
	bool operator[](KKey::Id keyId) const;
};

#endif
