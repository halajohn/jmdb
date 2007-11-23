// Jmdb - A J2ME debugger with Emacs interface
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef ClientSocket_hpp
#define ClientSocket_hpp

// System include
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

// Project include
//

#include "Type.hpp"
#include "Exception.hpp"

namespace jmdb
{
  typedef class ClientSocket ClientSocket;
  class ClientSocket
  {
  private:
    
    // Attribute
    
    int32_t m_socket_fd;
    std::string m_host;
    uint32_t m_port;
    uint32_t m_timeout;
    
    static uint32_t const DEFAULT_CONNECT_TIMEOUT = 5;
    static uint32_t const DEFAULT_CONNECT_PORT = 8000;
    static std::string const DEFAULT_CONNECT_HOST;
    
    // Operation
    
    template<typename SendType>
    void
    send(SendType const &value) const
    {
      int const result = ::send(m_socket_fd, static_cast<void const *>(&value), sizeof(SendType), 0);
      
      if (-1 == result)
      {
        throw SocketException("ERROR: send data failed.");
      }
    }
    
    template<typename RecvType>
    void
    recv(RecvType &value) const
    {
      if (-1 == ::recv(m_socket_fd, &value, sizeof(RecvType), 0))
      {
        throw SocketException("ERROR: recv data failed.");
      }
    }
    
  public:
    
    // Life cycle
    
    ClientSocket();
    
    // Operation
    
    void connect();
    
    int32_t
    socket_fd() const
    { return m_socket_fd; }
    
    // Operator
    
    // Send
    
    ClientSocket const &operator<<(std::string const &str) const;
    ClientSocket const &operator<<(char const * const &str) const;
    
    ClientSocket const &operator<<(uint64_t const value) const;
    ClientSocket const &operator<<(uint32_t const value) const;
    ClientSocket const &operator<<(uint16_t const value) const;
    
    ClientSocket const &
    operator<<(int64_t const value) const
    {
      operator<<(static_cast<uint64_t>(value));
      
      return *this;
    }
    
    ClientSocket const &
    operator<<(int32_t const value) const
    {
      operator<<(static_cast<uint32_t>(value));
      
      return *this;
    }
    
    ClientSocket const &
    operator<<(int16_t const value) const
    {
      operator<<(static_cast<uint16_t>(value));
      
      return *this;
    }
    
    template<typename SendType_>
    ClientSocket const &
    operator<<(SendType_ const value) const
    {
      send(value);
      
      return *this;
    }
    
    // Recv
    
    ClientSocket const &operator>>(uint64_t &value) const;
    ClientSocket const &operator>>(uint32_t &value) const;
    ClientSocket const &operator>>(uint16_t &value) const;
    ClientSocket const &operator>>(std::string &str) const;
    
    ClientSocket const &
    operator>>(int64_t &value) const
    {
      operator>>(reinterpret_cast<uint64_t &>(value));
      
      return *this;
    }
    
    ClientSocket const &
    operator>>(int32_t &value) const
    {
      operator>>(reinterpret_cast<uint32_t &>(value));
      
      return *this;
    }
    
    ClientSocket const &
    operator>>(int16_t &value) const
    {
      operator>>(reinterpret_cast<uint16_t &>(value));
      
      return *this;
    }
    
    template<typename RecvType>
    ClientSocket const &
    operator>>(RecvType &value) const
    {
      recv(value);
      
      return *this;
    }
    
    // Access
    
    void
    set_host(std::string const &host)
    { m_host = host; }
    
    void
    set_timeout(uint32_t const timeout)
    { m_timeout = timeout; }
    
    void
    set_port(uint32_t const port)
    { m_port = port; }
  };
}

#endif
