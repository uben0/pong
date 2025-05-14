#ifndef KSocket_HEADER_DEFINED
#define KSocket_HEADER_DEFINED

#include <string>
#include <sys/socket.h>

/// \brief A class that holds a network socket.
class KSocket {
	KSocket(const KSocket& src) = delete;
public:
	static constexpr int PortLenght = 6;
	static constexpr int AddrLenght = 54;
	static constexpr int Closed     = -1;
	static constexpr int Failure    = -1;
	static std::string error;

	enum Propertie {
		NoPropertie = 0b0000,
		Verbose     = 0b0001,
		Blocking    = 0b0010,
		ReuseAddr   = 0b0100,
		Broadcast   = 0b1000
	};

	enum IpVersion {
		IpvX = AF_UNSPEC,
		Ipv4 = AF_INET,
		Ipv6 = AF_INET6
	};

	enum Type {
		Udp = SOCK_DGRAM,
		Tcp = SOCK_STREAM
	};

	class Addr : public sockaddr_storage {
	public:
		Addr();
		Addr(const sockaddr* address);
		Addr(std::string address, uint16_t port = 0);
		Addr(const char* address, uint16_t port = 0);
		bool from(std::string address, uint16_t port = 0);
		void clear();
		const sockaddr* get_addr_ptr() const;
		unsigned int get_len() const;
		uint16_t get_port() const;
		IpVersion get_ip_version() const;
		bool is_set() const;
		bool is_empty() const;
		bool is_ipv4() const;
		bool is_ipv6() const;
		std::string to_string(bool port = true)  const;
	private:
		const void* ipv4_addr() const;
		const void* ipv6_addr() const;
	};
	bool is_opened() const;
	bool is_closed() const;
	bool is_bound() const;
	Addr addr() const;
	bool set_propertie(Propertie propertie);
	bool set_propertie(bool set, Propertie propertie);
protected:
	KSocket(Propertie propertie = NoPropertie);
	int  m_socket;
	Addr m_addr;
	Propertie m_propertie;
	bool init_propertie();
private:
	bool apply_propertie_blocking(bool set);
	bool apply_propertie_broadcast(bool set);
	bool apply_propertie_reuse_addr(bool set);
};

std::ostream& operator<<(std::ostream& ostream, const KSocket::Addr& addr);
std::istream& operator>>(std::istream& istream,       KSocket::Addr& addr);

KSocket::Propertie operator|(KSocket::Propertie left, KSocket::Propertie right);
KSocket::Propertie operator&(KSocket::Propertie left, KSocket::Propertie right);
KSocket::Propertie operator~(KSocket::Propertie right);

bool operator==(const KSocket::Addr& left, const KSocket::Addr& right);
bool operator!=(const KSocket::Addr& left, const KSocket::Addr& right);

namespace std {
	string to_string(KSocket::Addr);
}

typedef KSocket::Addr KSocketAddr;

#endif
