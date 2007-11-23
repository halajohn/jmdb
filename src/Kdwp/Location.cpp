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

#include <iostream>

#include "../ClientSocket.hpp"
#include "Location.hpp"
#include "Constant.hpp"

namespace jmdb
{
  /////////////////////////////////// Public //////////////////////////////////
  //================================= Operation ===============================
  
  void
  Location::dump_to_console() const
  {
    std::cout << *(class_type_to_string(static_cast<ClassTypeEnum>(m_class_type)))
              << "(0x" << std::hex
              << m_class_id << "), method "
              << std::dec << m_method_id << ", offset "
              << m_offset;
  }
  
  //================================= Friend ==================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, Location const &location)
  {
    socket << location.m_class_type;
    socket << location.m_class_id;
    socket << location.m_method_id;
    socket << location.m_offset;
    
    return socket;
  }
  
  ClientSocket const &
  operator>>(ClientSocket const &socket, Location &location)
  {
    socket >> location.m_class_type;
    socket >> location.m_class_id;
    socket >> location.m_method_id;
    socket >> location.m_offset;
    
    return socket;
  }
}
