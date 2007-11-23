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

#ifndef Location_hpp
#define Location_hpp

#include "../Type.hpp"

namespace jmdb
{
  // Forward declaration
  //
  
  class ClientSocket;
  
  typedef struct Location Location;
  struct Location
  {
    uint8_t m_class_type;
    uint32_t m_class_id;
    uint32_t m_method_id;
    uint64_t m_offset;
    
    // Life cycle
    
    Location()
    { }
    
    Location(uint8_t const class_type,
             uint32_t const class_id,
             uint32_t const method_id,
             uint64_t const offset)
      : m_class_type(class_type),
        m_class_id(class_id),
        m_method_id(method_id),
        m_offset(offset)
    { }
    
    // Operation
    
    void dump_to_console() const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, Location const &cmd);
    friend ClientSocket const &operator>>(ClientSocket const &socket, Location &cmd);
  };
}

#endif
