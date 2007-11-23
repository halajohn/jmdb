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

#include <cerrno>
#include <cassert>
#include <fcntl.h>

#include "ClientSocket.hpp"
#include "Exception.hpp"

namespace jmdb
{
  ////////////////////////////////// Private //////////////////////////////////
  //=========================== Attribute =====================================
  
  std::string const ClientSocket::DEFAULT_CONNECT_HOST = "localhost";
  
  ////////////////////////////////// Public ///////////////////////////////////
  //=========================== Life cycle ====================================
  
  ClientSocket::ClientSocket()
    : m_socket_fd(-1),
      m_host(DEFAULT_CONNECT_HOST),
      m_port(DEFAULT_CONNECT_PORT),
      m_timeout(DEFAULT_CONNECT_TIMEOUT)
  {
    m_socket_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    
    if (-1 == m_socket_fd)
    {
      throw SocketException("ERROR: Could not create socket.");
    }
    
    int on = 1;
    if (::setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) == -1)
    {
      throw SocketException("ERROR: SO_REUSEADDR failed.");
    }
  }
  
  //=============================== Operation =================================
  
  void
  ClientSocket::connect()
  {
    try
    {
      sockaddr_in sock_addr;
      
      sock_addr.sin_family = AF_INET;
      sock_addr.sin_port = htons(m_port);
      int result = inet_pton(AF_INET, "127.0.0.1", &(sock_addr.sin_addr));
      
      switch (result)
      {
      case 1:
        // success
        break;
        
      case 0:
        throw SocketException("ERROR: Input is not a valid IPv4 dotted-decimal string.");
        
      case -1:
        if (EAFNOSUPPORT == errno)
        {
          throw SocketException("ERROR: AF_INET is not supported.");
        }
        else
        {
          assert(1 != 1);
        }
      }
      
      int const flags = fcntl(m_socket_fd, F_GETFL, 0);
      fcntl(m_socket_fd, F_SETFL, flags | O_NONBLOCK);
      
      result = ::connect(m_socket_fd, reinterpret_cast<sockaddr *>(&sock_addr), sizeof(sock_addr));
      
      // restore flags, the original mode may be blocking or non-blocking mode.
      fcntl(m_socket_fd, F_SETFL, flags);
      
      if (result < 0)
      {
        if (EINPROGRESS == errno)
        {
          fd_set rset, wset;
          struct timeval tval;
          
          FD_ZERO(&rset);
          FD_SET(m_socket_fd, &rset);
          wset = rset;
          
          tval.tv_sec = m_timeout;
          tval.tv_usec = 0;
          
          int const select_result = select(m_socket_fd + 1, &rset, &wset, 0, m_timeout ? &tval : 0);
          
          if (0 == select_result)
          {
            // timeout
            throw SocketException("ERROR: Connect timeout.");
          }
          else
          {
            if ((0 == FD_ISSET(m_socket_fd, &rset)) && (0 == FD_ISSET(m_socket_fd, &wset)))
            {
              throw SocketException("ERROR: socket file descriptor not set while select return.");
            }
          }
        }
        else
        {
          throw SocketException("ERROR: Could not connect to host:port");
        }
      }
      else
      {
        // connect completed immediately
        assert (0 == result);
      }
    }
    catch (SocketException &e)
    {
      close(m_socket_fd);
      throw;
    }
  }
  
  //============================== Operator ===================================
  
  /////////////////////////////////////////////////////////////////////////////
  // Recv
  
  ClientSocket const &
  ClientSocket::operator>>(uint64_t &value) const
  {
    uint32_t upper_word;
    uint32_t lower_word;
    
    recv(upper_word);
    recv(lower_word);
    
    value = static_cast<uint64_t>(ntohl(lower_word));
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator>>(uint32_t &value) const
  {
    recv(value);
    
    value = ntohl(value);
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator>>(uint16_t &value) const
  {
    recv(value);
    
    value = ntohs(value);
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator>>(std::string &str) const
  {
    uint32_t length;
    
    operator>>(length);
    
    str.clear();
    str.reserve(length + 1); // plus null terminating character.
    
    for (uint32_t i = 0; i < length; ++i)
    {
      uint8_t ch;
      
      operator>>(ch);
      
      str.insert(str.end(), ch);
    }
    
    return *this;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  // Send
  
  ClientSocket const &
  ClientSocket::operator<<(uint64_t const value) const
  {
    uint32_t upper_word = static_cast<uint32_t>(value >> 32);
    uint32_t lower_word = static_cast<uint32_t>(value);
    
    upper_word = htonl(upper_word);
    lower_word = htonl(lower_word);
    
    send(upper_word);
    send(lower_word);
    
    return *this;
  }
  
  // :NOTE: Wei 2004-May-27:
  //
  // I have to use 'const' argument type,
  // otherwise, if I pass a const variable into ClientSocket::operator<<(...),
  // then the template will be specialized to do the job,
  // becuase ('T' can _not_ convert to 'const T')
  ClientSocket const &
  ClientSocket::operator<<(uint32_t const value) const
  {
    uint32_t temp = htonl(value);
    
    send(temp);
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator<<(uint16_t const value) const
  {
    uint16_t temp = htons(value);
    
    send(temp);
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator<<(std::string const &str) const
  {
    operator<<(str.size()); // send string length without the null terminating character.
    
    for (uint32_t i = 0; i < str.size(); ++i)
    {
      operator<<(str[i]); // send bytes
    }
    
    return *this;
  }
  
  ClientSocket const &
  ClientSocket::operator<<(char const * const &str) const
  {
    operator<<(strlen(str)); // send string length without the null terminating character.
    
    for (uint32_t i = 0; i < strlen(str); ++i)
    {
      operator<<(str[i]); // send bytes
    }
    
    return *this;
  }
}
