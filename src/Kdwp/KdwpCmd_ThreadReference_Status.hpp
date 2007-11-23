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

#ifndef KdwpCmd_ThreadReference_Status_hpp
#define KdwpCmd_ThreadReference_Status_hpp

#include <cassert>

#include "KdwpCmd_ThreadReference.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ThreadReference_Status KdwpCmd_ThreadReference_Status;
  class KdwpCmd_ThreadReference_Status : public KdwpCmd_ThreadReference
  {
  private:
    
    uint32_t m_thread_id;
    
    // :NOTE: Wei 2004-Jun-12:
    //
    // Because of the 'UNKNOWN' value, I have to define this variable as signed type.
    int32_t m_thread_status;
    
    uint32_t m_suspend_status;
    
    static uint32_t const EXTRA_HEADER_SIZE = sizeof(get_size(&KdwpCmd_ThreadReference_Status::m_thread_id));
    
  public:
    
    // Life cycle
    
    KdwpCmd_ThreadReference_Status(uint32_t const thread_id)
      : KdwpCmd_ThreadReference(FLAG_NONE, STATUS, EXTRA_HEADER_SIZE),
        m_thread_id(thread_id)
    { }
    
    // Inquiry
    
    std::auto_ptr<std::string> get_thread_status_string();
    std::auto_ptr<std::string> get_suspend_status_string();
    
    uint32_t
    suspend_status()
    { return m_suspend_status; }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ThreadReference_Status &cmd);
  };
}

#endif
