#include <iostream>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <bitset>

#include "KSocketUdp.hpp"
#include "KTool.hpp"
#include "KTerm.hpp"

KSocketUdp::KSocketUdp(Propertie propertie):
KSocket(propertie)
{}

KSocketUdp::~KSocketUdp() {
	close();
}

void KSocketUdp::close() {
	if (is_opened()) ::close(m_socket);
	m_socket = Closed;
	m_addr.clear();
}

bool KSocketUdp::open(IpVersion ipVersion) {
	close();
	m_socket = ::socket(ipVersion == Ipv4 ? Ipv4 : Ipv6, Udp, 0);
	if (is_closed()) {
		error = "unable to open socket : " + std::string(strerror(errno));
		return false;
	}
	init_propertie();
	return true;
}

bool KSocketUdp::bind(const Addr& address) {
	if (address.is_empty()) {
		error = "provided address is empty";
		return false;
	}
	open(address.get_ip_version());
	if (::bind(m_socket, (sockaddr*)&address, sizeof address) == Failure) {
		error = "unable to bind socket : " + std::string(strerror(errno));
		close();
		return false;
	}
	socklen_t tmpAddrLen = sizeof m_addr;
	if (getsockname(m_socket, (sockaddr*)&m_addr, &tmpAddrLen) == Failure) {
		error = "unable to get bound address : " + std::string(strerror(errno));
		close();
		return false;
	}
	return true;
}

bool KSocketUdp::bind(uint16_t port, IpVersion ipVersion) {
	if (ipVersion == Ipv4) {
		return bind({"0.0.0.0", port});
	}
	return bind({"::", port});
}

bool KSocketUdp::read(void* buffer, unsigned size) {
	if (is_opened() && buffer != nullptr && size > 0) {
		ssize_t state = ::read(m_socket, buffer, size);
		if (state == Failure) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				error = "unable to read data : " + std::string(strerror(errno));
			}
			return false;
		}
		return state > 0;
	}
	return false;
}

bool KSocketUdp::read(Addr& address, void* buffer, unsigned size) {
	if (is_opened() && buffer != nullptr && size > 0) {
		socklen_t socklen = sizeof address;
		ssize_t state = recvfrom(m_socket, buffer, size, 0, (sockaddr*)&address, &socklen);
		if (state == Failure) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				error = "unable to read data : " + std::string(strerror(errno));
			}
			return false;
		}
		return state > 0;
	}
	return false;
}

bool KSocketUdp::read(std::string& str) {
	str.clear();
	char buffer[64];
	while (read(buffer, sizeof buffer)) {
		for (unsigned i = 0; i < sizeof buffer; i++) {
			if (buffer[i] == '\0') return true;
			str.push_back(buffer[i]);
		}
	}
	str.clear();
	return false;
}

bool KSocketUdp::read(Addr& address, std::string& str) {
	str.clear();
	char buffer[64];
	while (read(address, buffer, sizeof buffer)) {
		for (unsigned i = 0; i < sizeof buffer; i++) {
			if (buffer[i] == '\0') return true;
			str.push_back(buffer[i]);
		}
	}
	str.clear();
	return false;
}

std::string KSocketUdp::read(Addr& address) {
	std::string buffer;
	read(address, buffer);
	return buffer;
}
std::string KSocketUdp::read() {
	std::string buffer;
	read(buffer);
	return buffer;
}

bool KSocketUdp::write(const Addr& address, const void* buffer, unsigned size) {
	if (buffer == nullptr) {
		error = "buffer pointer is null";
		return false;
	}
	if (address.is_empty()) {
		error = "provided address is mepty";
		return false;
	}
	if (is_closed()) open(address.get_ip_version());
	if (sendto(
		m_socket, buffer, size, 0, address.get_addr_ptr(), sizeof address
	) == Failure) {
		error = "unable to send data : " + std::string(strerror(errno));
		return false;
	}
	return true;
}

bool KSocketUdp::write(Addr const& address, const std::string& str) {
	return write(address, str.c_str(), str.size() + 1);
}

bool KSocketUdp::join_membership(const Addr& address) {
	if (is_closed()) {
		error = "socket is not opened";
		return false;
	}
	if (address.get_ip_version() != Ipv6) {
		error = "provided address is not ipv6";
		return false;
	}
	struct ipv6_mreq group = {((sockaddr_in6*)&address)->sin6_addr, 0};
	if(setsockopt(m_socket, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, &group, sizeof group) == -1) {
		error = "unable to join membership : " + std::string(strerror(errno));
		return false;
	}
	return true;
}

bool KSocketUdp::drop_membership(const Addr& address) {
	if (is_closed()) {
		error = "socket is not opened";
		return false;
	}
	if (address.get_ip_version() != Ipv6) {
		error = "provided address is not ipv6";
		return false;
	}
	struct ipv6_mreq group = {((sockaddr_in6*)&address)->sin6_addr, 0};
	if(setsockopt(m_socket, IPPROTO_IPV6, IPV6_DROP_MEMBERSHIP, &group, sizeof group) == -1) {
		error = "unable to drop membership : " + std::string(strerror(errno));
		return false;
	}
	return true;
}
