#ifndef KSocketTcp_HEADER_DEFINED
#define KSocketTcp_HEADER_DEFINED

#include "KSocket.hpp"

/// \brief A class that holds a TCP network socket.
class KSocketTcp : public KSocket {
	bool m_listen;
	Addr m_peer;
	uint32_t m_pending_data;
	uint32_t pending_data();
public:
	KSocketTcp(Propertie propertie = NoPropertie);
	bool listen(Addr address = "::", int pendingConnections = 1);
	bool connect(const Addr& address);
	bool accept(KSocketTcp& client) const;
	Addr peer() const;

	bool write(const std::string& str);
	bool write(const void* buffer, uint32_t size);
	template<typename T>
	bool write_var(const T& var) {return write(&var, sizeof var);}

	bool read(std::string& str);
	bool read(void* buffer, uint32_t size);
	template<typename T>
	bool read_var(T& var) {return read(&var, sizeof var);}

	void close();
	~KSocketTcp();
};

#endif
