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

#ifndef KdwpCmd_ThreadReference_Name_hpp
#define KdwpCmd_ThreadReference_Name_hpp

#include "KdwpCmd_ThreadReference.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ThreadReference_Name KdwpCmd_ThreadReference_Name;
  class KdwpCmd_ThreadReference_Name : public KdwpCmd_ThreadReference
  {
  private:
    
    uint32_t m_thread_id;
    std::auto_ptr<std::string> mp_thread_name;
    
    static uint32_t const EXTRA_HEADER_SIZE = sizeof(get_size(&KdwpCmd_ThreadReference_Name::m_thread_id));
    
  public:
    
    // Life cycle
    
    KdwpCmd_ThreadReference_Name(uint32_t const thread_id)
      : KdwpCmd_ThreadReference(FLAG_NONE, NAME, EXTRA_HEADER_SIZE),
        m_thread_id(thread_id)
    { }
    
    // Inquiry
    
    std::auto_ptr<std::string>
    get_thread_name()
    {
      return mp_thread_name;
    }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Name &cmd);
  };
}

#endif
