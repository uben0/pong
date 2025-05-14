#include <iostream>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <bitset>

#include "KSocketTcp.hpp"
#include "KTool.hpp"
#include "KTerm.hpp"

KSocketTcp::KSocketTcp(Propertie propertie) :
	KSocket(propertie),
	m_listen(false),
	m_pending_data(0)
{}

KSocketTcp::~KSocketTcp() {
	close();
}

void KSocketTcp::close() {
	if (is_opened()) {
		::shutdown(m_socket, SHUT_RDWR);
		::close(m_socket);
	}
	m_socket = Closed;
	m_listen = false;
	m_pending_data = 0;
	m_addr.clear();
	m_peer.clear();
}

bool KSocketTcp::listen(Addr address, int pendingConnections) {
	close();
	if (address.is_empty()) {
		error = "provided address is empty";
		return false;
	}
	if ((m_socket = ::socket(address.get_ip_version(), Tcp, 0)) == Closed) {
		error = "unable to open socket : " + std::string(strerror(errno));
		return false;
	}
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
	if (::listen(m_socket, pendingConnections) == Failure) {
		error = "unable to listen on socket : " + std::string(strerror(errno));
		close();
		return false;
	}
	m_listen = true;
	return true;
}

bool KSocketTcp::accept(KSocketTcp& client) const {
	if (not m_listen) {
		error = "socket is not listenning";
		return false;
	}
	client.close();
	client.set_propertie(m_propertie);
	socklen_t tmpAddrLen = sizeof client.m_peer;
	client.m_socket = ::accept(m_socket, (sockaddr*)&client.m_peer, &tmpAddrLen);
	if (client.is_closed()) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			error = "unable to accept client : " + std::string(strerror(errno));
		}
		return false;
	}
	client.init_propertie();
	tmpAddrLen = sizeof client.m_addr;
	if (getsockname(client.m_socket, (sockaddr*)&client.m_addr, &tmpAddrLen) == -1) {
		error = "unable to get bound address";
		client.close();
	}
	return true;
}

bool KSocketTcp::connect(const Addr& address) {
	if (address.is_empty()) {
		error = "provided address is empty";
		return false;
	}
	if ((m_socket = ::socket(address.get_ip_version(), Tcp, 0)) == Closed) {
		error = "unable to open socket : " + std::string(strerror(errno));
		return false;
	}
	if (::connect(m_socket, address.get_addr_ptr(), sizeof address) == Failure) {
		error = "unable to connect to server : " + std::string(strerror(errno));
		return false;
	}
	m_peer = address;
	socklen_t tmpAddrLen = sizeof m_addr;
	if (getsockname(m_socket, (sockaddr*)&m_addr, &tmpAddrLen) == Failure) {
		error = "unable to get bind address : " + std::string(strerror(errno));
		close();
		return false;
	}
	return true;
}

uint32_t KSocketTcp::pending_data() {
	if (is_opened() && m_pending_data == 0) {
		ssize_t state = ::read(m_socket, &m_pending_data, sizeof m_pending_data);
		if (state == 0) {
			close();
		}
		if (state < 0) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				error = "unable to get pending data : " + std::string(strerror(errno));
			}
		}
	}
	return m_pending_data;
}

bool KSocketTcp::read(std::string& str) {
	if (is_opened() && pending_data() > 0) {
		char buffer[m_pending_data];
		uint32_t lenght = m_pending_data;
		if (read(buffer, m_pending_data)) {
			str = std::string(buffer, lenght);
			return true;
		}
	}
	return false;
}

bool KSocketTcp::write(const std::string& str) {
	return write(str.c_str(), str.size());
}

bool KSocketTcp::read(void* buffer, uint32_t size) {
	if (is_opened() && pending_data() > 0) {
		ssize_t state;
		if (m_pending_data > size) {
			state = ::read(m_socket, buffer, size);
			m_pending_data -= size;
		} else {
			state = ::read(m_socket, buffer, m_pending_data);
			m_pending_data = 0;
		}
		if (state == 0) {
			close();
		} else
		if (state < 0) {
			error = "unable to read data : " + std::string(strerror(errno));
		}
		return state > 0;
	}
	return false;
}

bool KSocketTcp::write(const void* buffer, uint32_t size) {
	if (is_opened() && buffer != nullptr && size > 0) {
		ssize_t state = ::write(m_socket, &size, sizeof size);
		if (state >= 0) {
			state = ::write(m_socket, (void*)buffer, size);
		}
		if (state == Closed) {
			close();
			state = 0;
		}
		return state >= 0;
	}
	return false;
}
