#include <iostream>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
// #include <bitset>

#include "KSocket.hpp"
#include "KTool.hpp"
#include "KTerm.hpp"

std::string KSocket::error;

KSocket::Addr::Addr() {
	clear();
}

KSocket::Addr::Addr(std::string str, uint16_t port) {
	from(str, port);
}

KSocket::Addr::Addr(const char* str, uint16_t port) {
	from(str, port);
}

KSocket::Addr::Addr(const struct sockaddr * address) {
	clear();
	if (address == nullptr) return;
	switch (address->sa_family) {
		case Ipv4:
		memcpy(this, address, sizeof(sockaddr_in));
		break;
		case Ipv6:
		memcpy(this, address, sizeof(sockaddr_in6));
	}
}

bool KSocket::Addr::from(std::string str, uint16_t port) {
	clear();
	if (str.find_first_of('.') != str.npos) {
		// IPV4
		size_t i = str.find_first_of(':');
		if (i != str.npos) {
			port = std::stoi(str.substr(i + 1));
			str.erase(i);
		}
		if (inet_pton(Ipv4, str.c_str(), &((sockaddr_in*)this)->sin_addr) > 0) {
			((sockaddr_in*)this)->sin_port = htons(port);
			ss_family = Ipv4;
			return true;
		}
	}
	else {
		// IPV6
		size_t i = str.find_first_of(']');
		if (i != str.npos) {
			port = std::stoi(str.substr(i + 2));
			str = str.substr(1, i - 1);
		}
		if (inet_pton(Ipv6, str.c_str(), &((sockaddr_in6*)this)->sin6_addr) > 0) {
			((sockaddr_in6*)this)->sin6_port = htons(port);
			ss_family = Ipv6;
			return true;
		}
	}
	return false;
}

void KSocket::Addr::clear() {
	ss_family = IpvX;
}

const sockaddr * KSocket::Addr::get_addr_ptr() const {
	return (const sockaddr*) this;
}

unsigned int KSocket::Addr::get_len() const {
	switch (ss_family) {
		case Ipv4: return sizeof (sockaddr_in);
		case Ipv6: return sizeof (sockaddr_in6);
	}
	return 0;
}

KSocket::IpVersion KSocket::Addr::get_ip_version() const {
	return (IpVersion)ss_family;
}

uint16_t KSocket::Addr::get_port() const {
	switch (ss_family) {
		case Ipv4: return ntohs(((sockaddr_in *)this)->sin_port);
		case Ipv6: return ntohs(((sockaddr_in6*)this)->sin6_port);
	}
	return 0;
}

bool KSocket::Addr::is_set() const {
	return ss_family == Ipv4 || ss_family == Ipv6;
}

namespace std {
	string to_string(KSocket::Addr address) {
		return address.to_string();
	}
}

const void* KSocket::Addr::ipv4_addr() const {
	return &((const sockaddr_in*)this)->sin_addr;
}
const void* KSocket::Addr::ipv6_addr() const {
	return &((const sockaddr_in6*)this)->sin6_addr;
}

bool KSocket::Addr::is_ipv4() const {
	return ss_family == Ipv4;
}

bool KSocket::Addr::is_ipv6() const {
	return ss_family == Ipv6;
}

bool KSocket::Addr::is_empty() const {
	return ss_family == IpvX;
}

std::string KSocket::Addr::to_string(bool port) const {
	if (is_empty()) return std::string();
	char buffer[AddrLenght];
	if (is_ipv4() && inet_ntop(Ipv4, ipv4_addr(), buffer, AddrLenght)) {
		if (port) {
			int writeCursor = strlen(buffer);
			buffer[writeCursor++] = ':';
			KTool::itoa(
				get_port(),
				buffer + writeCursor,
				PortLenght
			);
		}
		return buffer;
	}
	else if (is_ipv6() && inet_ntop(Ipv6, ipv6_addr(), buffer + (port ? 1:0), AddrLenght)) {
		if (port) {
			buffer[0] = '[';
			int writeCursor = strlen(buffer);
			buffer[writeCursor++] = ']';
			buffer[writeCursor++] = ':';
			KTool::itoa(
				get_port(),
				buffer + writeCursor,
				PortLenght
			);
		}
		return buffer;
	}
	return std::string();
}

KSocket::Addr KSocket::addr() const {
	return m_addr;
}

bool operator==(const KSocket::Addr& left, const KSocket::Addr& right) {
	if (left.ss_family != right.ss_family) {
		return false;
	}
	return std::memcmp(&left, &right, left.get_len()) == 0;
}

bool operator!=(const KSocket::Addr& left, const KSocket::Addr& right) {
	if (left.ss_family != right.ss_family) {
		return true;
	}
	return std::memcmp(&left, &right, left.get_len()) != 0;
}

std::ostream& operator<<(std::ostream &ostream, KSocket::Addr const& addr) {
	return ostream << addr.to_string(true);
}

std::istream& operator>>(std::istream &istream, KSocket::Addr & addr) {
	char buff[INET6_ADDRSTRLEN + KSocket::PortLenght + 2];
	for (unsigned int i = 0; i < sizeof buff; i++) {
		istream.get(buff[i]);
		if (
			(buff[i] < '0' || buff[i] > '9') &&
			(buff[i] < 'a' || buff[i] > 'z') &&
			(buff[i] < 'A' || buff[i] > 'Z') &&
			buff[i] != ':' && buff[i] != '.' &&
			buff[i] != '[' && buff[i] != ']'
		) {
			istream.putback(buff[i]);
			buff[i] = '\0';
			addr.from(buff);
			break;
		}
	}
	return istream;
}

KSocket::KSocket(Propertie propertie) :
	m_socket(Closed),
	m_addr(),
	m_propertie(propertie)
{}

bool KSocket::is_opened() const {
	return m_socket != Closed;
}

bool KSocket::is_closed() const {
	return m_socket == Closed;
}

bool KSocket::is_bound() const {
	return m_addr.is_set();
}

KSocket::Propertie operator|(KSocket::Propertie left, KSocket::Propertie right) {
	return (KSocket::Propertie)((unsigned)left | (unsigned)right);
}
KSocket::Propertie operator&(KSocket::Propertie left, KSocket::Propertie right) {
	return (KSocket::Propertie)((unsigned)left & (unsigned)right);
}
KSocket::Propertie operator~(KSocket::Propertie right) {
	return (KSocket::Propertie)(~(unsigned)right);
}

bool KSocket::set_propertie(Propertie propertie) {
	bool success = true;
	if (is_opened()) {
		if ((propertie & Broadcast) != (m_propertie & Broadcast)) {
			success &= apply_propertie_broadcast(propertie & Broadcast);
		}
		if ((propertie & ReuseAddr) != (m_propertie & ReuseAddr)) {
			success &= apply_propertie_reuse_addr(propertie & ReuseAddr);
		}
		if ((propertie & Blocking) != (m_propertie & Blocking)) {
			success &= apply_propertie_blocking(propertie & Blocking);
		}
	}
	m_propertie = propertie;
	return success;
}

bool KSocket::set_propertie(bool set, Propertie propertie) {
	if (set) {
		return set_propertie(m_propertie | propertie);
	}
	else {
		return set_propertie(m_propertie & ~propertie);
	}
}

bool KSocket::init_propertie() {
	bool success = true;
	if (is_opened()) {
		if (m_propertie & Broadcast) {
			success &= apply_propertie_broadcast(true);
		}
		if (m_propertie & ReuseAddr) {
			success &= apply_propertie_reuse_addr(true);
		}
		if ((m_propertie & Blocking) == false) {
			success &= apply_propertie_blocking(false);
		}
	}
	return success;
}

bool KSocket::apply_propertie_blocking(bool set) {
	int flags = fcntl(m_socket, F_GETFL, 0);
	if (flags == -1) {
		perror("set blocking failed");
		return false;
	}
	flags = set ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	if (fcntl(m_socket, F_SETFL, flags)) {
		error = "unable to change socket propertie : " + std::string(strerror(errno));
		return false;
	}
	return true;
}

bool KSocket::apply_propertie_broadcast(bool set) {
	int boolean = set;
	if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &boolean, sizeof boolean) == -1) {
		error = "unable to change socket propertie : " + std::string(strerror(errno));
		return false;
	}
	return true;
}

bool KSocket::apply_propertie_reuse_addr(bool set) {
	int boolean = set;
	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &boolean, sizeof boolean) == -1) {
		error = "unable to change socket propertie : " + std::string(strerror(errno));
		return false;
	}
	return true;
}
