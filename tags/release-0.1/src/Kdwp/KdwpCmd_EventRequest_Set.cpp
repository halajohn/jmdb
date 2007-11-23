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

#include "KdwpCmd_EventRequest_Set.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Operation ==================================
  
  uint32_t
  KdwpCmd_EventRequest_Set::count_extra_modifier_size() const
  {
    uint32_t extra_size = m_modifier_count * sizeof(get_size(&KdwpCmd_EventRequest_Set::ModBase::m_kind));
    
    for (uint32_t i = 0; i < m_modifier_count; ++i)
    {
      switch (m_modifier[i]->m_kind)
      {
      case MOD_COUNT:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModCount::m_count));
        break;
        
      case MOD_CONDITIONAL:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModCond::m_expr_id));
        break;
        
      case MOD_THREAD_ONLY:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModThreadOnly::m_thread_id));
        break;
        
      case MOD_CLASS_ONLY:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModClassOnly::m_class_id));
        break;
        
      case MOD_CLASS_MATCH:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModClassMatch::m_class_pattern));
        break;
        
      case MOD_CLASS_EXCLUDE:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModClassExclude::m_class_pattern));
        break;
        
      case MOD_LOCATION_ONLY:
        extra_size += sizeof(get_size(&Location::m_class_type));
        extra_size += sizeof(get_size(&Location::m_class_id));
        extra_size += sizeof(get_size(&Location::m_method_id));
        extra_size += sizeof(get_size(&Location::m_offset));
        break;
        
      case MOD_EXCEPTION_ONLY:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModExceptionOnly::m_exception));
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModExceptionOnly::m_caught));
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModExceptionOnly::m_uncaught));
        break;
        
      case MOD_FIELD_ONLY:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModFieldOnly::m_declaring));
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModFieldOnly::m_field_id));
        break;
        
      case MOD_STEP:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModStep::m_thread_id));
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModStep::m_size));
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModStep::m_depth));
        break;
        
      case MOD_INSTANCE_ONLY:
        extra_size += sizeof(get_size(&KdwpCmd_EventRequest_Set::ModInstanceOnly::m_instance));
        break;
      }
    }
    
    return extra_size;
  }
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================= Life cycle ==================================
  
  KdwpCmd_EventRequest_Set::ModBase::~ModBase()
  {
  }
  
  //============================= Operation ===================================
  
  void
  KdwpCmd_EventRequest_Set::set_breakpoint(uint32_t const class_id,
                                           uint32_t const method_id,
                                           uint64_t const offset)
  {
    m_event_kind = EVENT_BREAKPOINT;
    
    ++m_modifier_count;
    
    ModLocationOnly * const p_modifier = new ModLocationOnly(static_cast<uint8_t>(CT_CLASS),
                                                             class_id,
                                                             method_id,
                                                             offset);
    
    m_modifier.push_back(p_modifier);
    
    add_extra_packet_size(count_extra_modifier_size());
  }
  
  void
  KdwpCmd_EventRequest_Set::set_running_next(uint32_t const thread_id)
  {
    m_event_kind = EVENT_SINGLE_STEP;
    
    ++m_modifier_count;
    
    ModStep * const p_modifier = new ModStep(thread_id, SS_LINE, SD_OVER);
    
    m_modifier.push_back(p_modifier);
    
    add_extra_packet_size(count_extra_modifier_size());
  }
  
  void
  KdwpCmd_EventRequest_Set::set_running_step(uint32_t const thread_id)
  {
    m_event_kind = EVENT_SINGLE_STEP;
    
    ++m_modifier_count;
    
    ModStep * const p_modifier = new ModStep(thread_id, SS_LINE, SD_INTO);
    
    m_modifier.push_back(p_modifier);
    
    add_extra_packet_size(count_extra_modifier_size());
  }
  
  KdwpCmd_EventRequest_Set::~KdwpCmd_EventRequest_Set()
  {
    for (std::vector<ModBase const *>::iterator iter = m_modifier.begin();
         iter != m_modifier.end();
         ++iter)
    {
      delete *iter;
    }
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModCount::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_count;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModCond::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_expr_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModThreadOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_thread_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModClassOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_class_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModClassMatch::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_class_pattern;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModClassExclude::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_class_pattern;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModLocationOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind << m_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModExceptionOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind
           << m_exception
           << m_caught
           << m_uncaught;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModFieldOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind
           << m_declaring
           << m_field_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModStep::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind
           << m_thread_id
           << m_size
           << m_depth;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_EventRequest_Set::ModInstanceOnly::DumpToSocket(ClientSocket const &socket) const
  {
    socket << m_kind
           << m_instance;
    
    return socket;
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_EventRequest_Set::ModBase const &mod)
  {
    return mod.DumpToSocket(socket);
  }
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_EventRequest_Set &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    socket << cmd.m_event_kind;
    socket << cmd.m_suspend_policy;
    socket << cmd.m_modifier_count;
    
    for (std::vector<KdwpCmd_EventRequest_Set::ModBase const *>::iterator iter = cmd.m_modifier.begin();
         iter != cmd.m_modifier.end();
         ++iter)
    {
      socket << *(*iter);
    }
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      socket >> cmd.m_request_id;
      break;
      
    case ERROR_INVALID_THREAD:
    case ERROR_INVALID_CLASS:
    case ERROR_INVALID_STRING:
    case ERROR_INVALID_OBJECT:
    case ERROR_INVALID_COUNT:
    case ERROR_INVALID_FIELD_ID:
    case ERROR_INVALID_METHOD_ID:
    case ERROR_INVALID_LOCATION:
    case ERROR_INVALID_EVENT_TYPE:
    case ERROR_NOT_IMPLEMENTED:
    case ERROR_VM_DEAD:
    default:
      std::cerr << cmd.get_error_code() << std::endl;
      assert(1 != 1);
      break;
    }
    
    return socket;
  }
}
