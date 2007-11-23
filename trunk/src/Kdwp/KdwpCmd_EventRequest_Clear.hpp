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

#ifndef KdwpCmd_EventRequest_Clear_hpp
#define KdwpCmd_EventRequest_Clear_hpp

#include "KdwpCmd_EventRequest.hpp"
#include "Constant.hpp"
#include "../Type.hpp"

namespace jmdb
{
  typedef class KdwpCmd_EventRequest_Clear KdwpCmd_EventRequest_Clear;
  class KdwpCmd_EventRequest_Clear : public KdwpCmd_EventRequest
  {
  private:
    
    uint8_t m_event_kind;
    uint32_t m_req_id;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_EventRequest_Clear::m_event_kind)) +
                                               sizeof(get_size(&KdwpCmd_EventRequest_Clear::m_req_id)));
    
  public:
    
    // Life cycle
    
    KdwpCmd_EventRequest_Clear(EventKindEnum const kind, uint32_t const req_id)
      : KdwpCmd_EventRequest(FLAG_NONE, CLEAR, EXTRA_HEADER_SIZE),
        m_event_kind(kind),
        m_req_id(req_id)
    { }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_EventRequest_Clear &cmd);
  };
}

#endif
