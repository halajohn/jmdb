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

#ifndef KdwpCmd_Event_Composite_hpp
#define KdwpCmd_Event_Composite_hpp

#include <cassert>
#include <vector>

#include "Value.hpp"
#include "Location.hpp"
#include "Constant.hpp"
#include "PacketHeader.hpp"

namespace jmdb
{
  typedef class KdwpCmd_Event_Composite KdwpCmd_Event_Composite;
  class KdwpCmd_Event_Composite : public PacketHeader
  {
  public:
    
    struct EventBase
    {
      uint8_t m_event_kind;
      uint32_t m_req_id;
      
      EventBase(EventKindEnum const kind, uint32_t const req_id)
        : m_event_kind(static_cast<uint8_t>(kind)),
          m_req_id(req_id)
      { }
      
      virtual ~EventBase() = 0;
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket) = 0;
    };
    
    struct VMStart : public EventBase
    {
      uint32_t m_thread_id;
      
      VMStart(uint32_t const req_id)
        : EventBase(EVENT_VM_START, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct SingleStep : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      
      SingleStep(uint32_t const req_id)
        : EventBase(EVENT_SINGLE_STEP, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct BreakPoint : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      
      BreakPoint(uint32_t const req_id)
        : EventBase(EVENT_BREAKPOINT, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct MethodEntry : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      
      MethodEntry(uint32_t const req_id)
        : EventBase(EVENT_METHOD_ENTRY, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct MethodExit : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      
      MethodExit(uint32_t const req_id)
        : EventBase(EVENT_METHOD_EXIT, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct Exception : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      uint32_t m_exception_id;
      Location m_catch_location;
      
      Exception(uint32_t const req_id)
        : EventBase(EVENT_EXCEPTION, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct ThreadStart : public EventBase
    {
      uint32_t m_thread_id;
      
      ThreadStart(uint32_t const req_id)
        : EventBase(EVENT_THREAD_START, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct ThreadDeath : public EventBase
    {
      uint32_t m_thread_id;
      
      ThreadDeath(uint32_t const req_id)
        : EventBase(EVENT_THREAD_END, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct ClassPrepare : public EventBase
    {
      uint32_t m_thread_id;
      uint8_t m_ref_type;
      uint32_t m_ref_id;
      std::string m_signature;
      uint32_t m_status;
      
      ClassPrepare(uint32_t const req_id)
        : EventBase(EVENT_CLASS_PREPARE, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct ClassUnload : public EventBase
    {
      std::string m_signature;
      
      ClassUnload(uint32_t const req_id)
        : EventBase(EVENT_CLASS_UNLOAD, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct FieldAccess : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      uint8_t m_ref_type;
      uint32_t m_ref_id;
      uint32_t m_field_id;
      uint32_t m_object_id;
      
      FieldAccess(uint32_t const req_id)
        : EventBase(EVENT_FIELD_ACCESS, req_id)
      { }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
    struct FieldModification : public EventBase
    {
      uint32_t m_thread_id;
      Location m_location;
      uint8_t m_ref_type;
      uint32_t m_ref_id;
      uint32_t m_field_id;
      uint32_t m_object_id;
      ValueBase *m_value;
      
      FieldModification(uint32_t const req_id)
        : EventBase(EVENT_FIELD_MODIFICATION, req_id),
          m_value(0)
      { }
      
      ~FieldModification()
      {
        assert(m_value != 0);
        
        delete m_value;
      }
      
      virtual ClientSocket const &get_from_socket(ClientSocket const &socket);
    };
    
  private:
    
    // Attribute
    
    uint8_t m_suspend_policy;
    uint32_t m_event_count;
    std::vector<EventBase const *> m_events;
    
  public:
    
    // Life cycle
    
    KdwpCmd_Event_Composite()
      : PacketHeader(PacketHeader::EVENT_PACKET)
    { }
    
    ~KdwpCmd_Event_Composite()
    {
      for (std::vector<EventBase const *>::iterator iter = m_events.begin();
           iter != m_events.end();
           ++iter)
      {
        delete *iter;
      }
    }
    
    // Operation
    
    void dump_to_console() const;
    
    // Access
    
    std::vector<EventBase const *> &
    events()
    { return m_events; }
    
    // Friend
    
    friend ClientSocket const &operator>>(ClientSocket const &socket, KdwpCmd_Event_Composite &event);
    friend ClientSocket const &operator>>(ClientSocket const &socket, EventBase &base);
  };
}

#endif
