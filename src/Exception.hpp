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

#ifndef Exception_hpp
#define Exception_hpp

#include "Type.hpp"

namespace jmdb
{
  typedef class SocketException SocketException;
  class SocketException : public std::exception
  {
  private:
    
    // Attribute
    
    static uint32_t const BUFFER_SIZE = 50;
    
    char m_message[BUFFER_SIZE];
    
    // Life cycle
    
    SocketException(); // disable default constructor
    
  public:
    
    // Life cycle
    
    SocketException(char const * const message)
    {
      uint32_t i;
      
      for (i = 0; (i < BUFFER_SIZE) && (message[i] != 0); ++i)
      {
        m_message[i] = message[i];
      }
      
      m_message[i] = 0; // null terminator
    };
    
    inline char *
    description()
    {
      return m_message;
    }
  };
  
  typedef class BadNumber BadNumber;
  class BadNumber : public std::exception
  {
  };
  
  typedef class BadCommand BadCommand;
  class BadCommand : public std::exception
  {
  };
  
  typedef class UnknownMethod UnknownMethod;
  class UnknownMethod : public std::exception
  {
  };
  
  typedef class UnknownFrame UnknownFrame;
  class UnknownFrame : public std::exception
  {
  };
  
  typedef class HandShakeError HandShakeError;
  class HandShakeError : public std::exception
  {
  };
  
  typedef class KdwpErrorInvalidThread KdwpErrorInvalidThread;
  class KdwpErrorInvalidThread : public std::exception
  {
  };
  
  typedef class KdwpErrorInvalidMethodId KdwpErrorInvalidMethodId;
  class KdwpErrorInvalidMethodId : public std::exception
  {
  };
  
  typedef class KdwpErrorNotFound KdwpErrorNotFound;
  class KdwpErrorNotFound : public std::exception
  {
  };
}

#endif
