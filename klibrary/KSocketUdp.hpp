#ifndef KSocketUdp_HEADER_DEFINED
#define KSocketUdp_HEADER_DEFINED

#include "KSocket.hpp"

/// \brief A class that holds an UDP network socket.
class KSocketUdp : public KSocket {
public:
	bool join_membership(const Addr& address);
	bool drop_membership(const Addr& address);

	KSocketUdp(Propertie propertie = NoPropertie);
	bool open(IpVersion = Ipv6);
	bool bind(const Addr& address);
	bool bind(uint16_t port, IpVersion ipVersion = Ipv6);

	bool write(const Addr& address, const void* buffer, unsigned size);
	bool write(const Addr& address, const std::string& str);
	template<typename T>
	bool write_var(const Addr& address, T& var) {
		return write(address, &var, sizeof var);
	}

	bool read(void* buffer, unsigned size);
	bool read(Addr& address, void* buffer, unsigned size);
	bool read(std::string& str);
	bool read(Addr& address, std::string& str);
	std::string read(Addr& address);
	std::string read();
	template<typename T>
	bool read_var(T& var) {
		return read(&var, sizeof var);
	}
	template<typename T>
	bool read_var(Addr& address, T& var) {
		return read(address, &var, sizeof var);
	}
	void close();
	~KSocketUdp();
};

#endif
