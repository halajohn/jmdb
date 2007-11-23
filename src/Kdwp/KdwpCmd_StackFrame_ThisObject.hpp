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

#ifndef KdwpCmd_StackFrame_ThisObject_hpp
#define KdwpCmd_StackFrame_ThisObject_hpp

#include <string>

#include "KdwpCmd_StackFrame.hpp"

namespace jmdb
{
  typedef class KdwpCmd_StackFrame_ThisObject KdwpCmd_StackFrame_ThisObject;
  class KdwpCmd_StackFrame_ThisObject : public KdwpCmd_StackFrame
  {
  private:
    
    // Out data
    
    uint32_t m_thread_id;
    uint32_t m_frame_id;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_StackFrame_ThisObject::m_thread_id)) +
                                               sizeof(get_size(&KdwpCmd_StackFrame_ThisObject::m_frame_id)));
    
    // Reply data
    
    uint8_t m_tag;
    uint32_t m_this_object_id;
    
  public:
    
    // Life cycle
    
    KdwpCmd_StackFrame_ThisObject(uint32_t const thread_id, uint32_t const frame_id)
      : KdwpCmd_StackFrame(FLAG_NONE, THIS_OBJECT, EXTRA_HEADER_SIZE),
        m_thread_id(thread_id),
        m_frame_id(frame_id)
    { }
    
    // Access
    
    uint32_t
    get_this_object_id() const
    { return m_this_object_id; }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_StackFrame_ThisObject &cmd);
  };
}

#endif
