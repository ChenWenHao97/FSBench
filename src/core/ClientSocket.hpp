#pragma once
#include "Socket.hpp"
#include <iostream>
#include "SocketException.hpp"
#include <string>
class ClientSocket : private Socket
{
public:
  ClientSocket(std::string host, int port);
  virtual ~ClientSocket() {}
  void Set_Non_Blocking(bool is);
  const ClientSocket &operator<<(const std::string &) const;
  const ClientSocket &operator>>(std::string &) const;
};

ClientSocket::ClientSocket(std::string host, int port)
{
  if (!Socket::create())
  {
    throw SocketException("clinetsocket Could not create client socket.");
  }

  if (!Socket::connect(host, port))
  {
    // cout << "clientsocket错误的port:" << port << endl;
    throw SocketException("clinetsocket Could not connect to port.");
  }
}
void ClientSocket::Set_Non_Blocking(bool is)
{
  Socket::set_non_blocking(is);
}

const ClientSocket &ClientSocket::operator<<(const std::string &s) const
{
  if (!Socket::send(s))
  {
    throw SocketException("clinetsocket Could not write to socket.");
  }

  return *this;
}

const ClientSocket &ClientSocket::operator>>(std::string &s) const
{
  if (!Socket::recv(s))
  {
    throw SocketException("clinetsocket Could not read from socket.");
  }

  return *this;
}