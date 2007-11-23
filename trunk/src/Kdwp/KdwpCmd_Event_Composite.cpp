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
#include "KdwpCmd_Event_Composite.hpp"
#include "Constant.hpp"
#include "Value.hpp"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================== Life cycle =================================
  
  KdwpCmd_Event_Composite::EventBase::~EventBase()
  {
  }
  
  //============================== Operation ==================================
    
  ClientSocket const &
  KdwpCmd_Event_Composite::VMStart::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::SingleStep::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::BreakPoint::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::MethodEntry::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::MethodExit::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::Exception::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    socket >> m_exception_id;
    socket >> m_catch_location;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::ThreadStart::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::ThreadDeath::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::ClassPrepare::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_ref_type;
    socket >> m_ref_id;
    socket >> m_signature;
    socket >> m_status;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::ClassUnload::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_signature;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::FieldAccess::get_from_socket(ClientSocket const &socket)
  {
    socket >> m_thread_id;
    socket >> m_location;
    socket >> m_ref_type;
    socket >> m_ref_id;
    socket >> m_field_id;
    socket >> m_object_id;
    
    return socket;
  }
  
  ClientSocket const &
  KdwpCmd_Event_Composite::FieldModification::get_from_socket(ClientSocket const &socket)
  {
    uint8_t type;
    ValueBase *value;
    
    socket >> m_thread_id;
    socket >> m_location;
    socket >> m_ref_type;
    socket >> m_ref_id;
    socket >> m_field_id;
    socket >> m_object_id;
    socket >> type;
    
    switch (type)
    {
    case V_ARRAY:
      value = new Value<V_ARRAY>;
      break;
      
    case V_BYTE:
      value = new Value<V_BYTE>;
      break;
      
    case V_CHAR:
      value = new Value<V_CHAR>;
      break;
      
    case V_OBJECT:
      value = new Value<V_OBJECT>;
      break;
      
    case V_FLOAT:
      value = new Value<V_FLOAT>;
      break;
      
    case V_DOUBLE:
      value = new Value<V_DOUBLE>;
      break;
      
    case V_INT:
      value = new Value<V_INT>;
      break;
      
    case V_LONG:
      value = new Value<V_LONG>;
      break;
      
    case V_SHORT:
      value = new Value<V_SHORT>;
      break;
      
    case V_VOID:
      value = new Value<V_VOID>;
      break;
      
    case V_BOOLEAN:
      value = new Value<V_BOOLEAN>;
      break;
      
    case V_STRING:
      value = new Value<V_STRING>;
      break;
      
    case V_THREAD:
      value = new Value<V_THREAD>;
      break;
      
    case V_THREAD_GROUP:
      value = new Value<V_THREAD_GROUP>;
      break;
      
    case V_CLASS_LOADER:
      value = new Value<V_CLASS_LOADER>;
      break;
      
    case V_CLASS_OBJECT:
      value = new Value<V_CLASS_OBJECT>;
      break;
      
    default:
      assert(1 != 1);
      break;
    }
    
    socket >> (*value);
    
    m_value = value;
    
    return socket;
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //============================== Operation ==================================
  
  void
  KdwpCmd_Event_Composite::dump_to_console() const
  {
    std::cout << "Receive " << std::dec << m_event_count << " event"
              << ((m_event_count <= 1) ? "" : "s") << "." << std::endl;
    
    std::cout << "Event" << ((m_event_count <= 1) ? "" : "s")
              << " information " << ((m_event_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<EventBase const *>::const_iterator iter = m_events.begin();
         iter != m_events.end();
         ++iter)
    {
      switch ((*iter)->m_event_kind)
      {
      case EVENT_VM_START:
        std::cout << gettext("<VM Start> initial thread ID is 0x") << std::hex
                  << static_cast<VMStart const *>(*iter)->m_thread_id
                  << std::endl;
        break;
        
      case EVENT_SINGLE_STEP:
        std::cout << "<Single Step> thread "
                  << static_cast<SingleStep const *>(*iter)->m_thread_id
                  << " steps to ";
        (static_cast<SingleStep const *>(*iter)->m_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_BREAKPOINT:
        std::cout << "<Break Point> thread "
                  << std::dec << static_cast<BreakPoint const *>(*iter)->m_thread_id
                  << " hit a breakpoint at ";
        (static_cast<BreakPoint const *>(*iter)->m_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_METHOD_ENTRY:
        std::cout << "<Method Entry> thread "
                  << static_cast<MethodEntry const *>(*iter)->m_thread_id
                  << " enter a method at ";
        (static_cast<MethodEntry const *>(*iter)->m_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_METHOD_EXIT:
        std::cout << "<Method Exit> thread "
                  << static_cast<MethodExit const *>(*iter)->m_thread_id
                  << " exit a method at ";
        (static_cast<MethodExit const *>(*iter)->m_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_EXCEPTION:
        std::cout << "<Exception> thread "
                  << static_cast<Exception const *>(*iter)->m_thread_id
                  << " throws an exception at ";
        (static_cast<Exception const *>(*iter)->m_location).dump_to_console();
        std::cout << ", catch at ";
        (static_cast<Exception const *>(*iter)->m_catch_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_THREAD_START:
        std::cout << "<Thread Start> thread "
                  << static_cast<ThreadStart const *>(*iter)->m_thread_id
                  << " started." << std::endl;
        break;
        
      case EVENT_THREAD_END:
        std::cout << "<Thread Start> thread "
                  << static_cast<ThreadDeath const *>(*iter)->m_thread_id
                  << " completed." << std::endl;
        break;
        
      case EVENT_CLASS_PREPARE:
        std::cout << "<Class Prepare> thread "
                  << static_cast<ClassPrepare const *>(*iter)->m_thread_id
                  << " prepares a "
                  << *(class_type_to_string(static_cast<ClassTypeEnum>(static_cast<ClassPrepare const *>(*iter)->m_ref_type)))
                  << "(" << static_cast<ClassPrepare const *>(*iter)->m_ref_id << ", "
                  << static_cast<ClassPrepare const *>(*iter)->m_signature
                  << ")"
                  << std::endl;
        break;
        
      case EVENT_CLASS_UNLOAD:
        std::cout << "<Class Unload> unload "
                  << static_cast<ClassUnload const *>(*iter)->m_signature
                  << std::endl;
        break;
        
      case EVENT_FIELD_ACCESS:
        std::cout << "<Field Access> thread "
                  << static_cast<FieldAccess const *>(*iter)->m_thread_id
                  << " access "
                  << *(class_type_to_string(static_cast<ClassTypeEnum>(static_cast<FieldAccess const *>(*iter)->m_ref_type)))
                  << "(" << static_cast<FieldAccess const *>(*iter)->m_ref_id << ")::"
                  << static_cast<FieldAccess const *>(*iter)->m_field_id
                  << " in " << static_cast<FieldAccess const *>(*iter)->m_object_id
                  << " at ";
        (static_cast<FieldAccess const *>(*iter)->m_location).dump_to_console();
        std::cout << std::endl;
        break;
        
      case EVENT_FIELD_MODIFICATION:
        std::cout << "<Field Modification> thread "
                  << static_cast<FieldModification const *>(*iter)->m_thread_id
                  << " modify "
                  << *(class_type_to_string(static_cast<ClassTypeEnum>(static_cast<FieldModification const *>(*iter)->m_ref_type)))
                  << "(" << static_cast<FieldModification const *>(*iter)->m_ref_id << ")::"
                  << static_cast<FieldModification const *>(*iter)->m_field_id
                  << " in " << static_cast<FieldModification const *>(*iter)->m_object_id
                  << " at ";
        (static_cast<FieldModification const *>(*iter)->m_location).dump_to_console();
        std::cout << " using ";
        (static_cast<FieldModification const *>(*iter)->m_value)->dump_to_console();
        std::cout << std::endl;
        break;
      }
    }
    
    std::cout << std::endl;
  }
  
  //============================== Friend =====================================
  
  ClientSocket const &
  operator>>(ClientSocket const &socket, KdwpCmd_Event_Composite::EventBase &base)
  {
    return base.get_from_socket(socket);
  }
  
  ClientSocket const &
  operator>>(ClientSocket const &socket, KdwpCmd_Event_Composite &event)
  {
    socket >> static_cast<PacketHeader &>(event);
    
    socket >> event.m_suspend_policy;
    socket >> event.m_event_count;
    
    for (uint32_t i = 0; i < event.m_event_count; ++i)
    {
      uint8_t type;
      uint32_t req_id;
      KdwpCmd_Event_Composite::EventBase *base;
      
      socket >> type;
      socket >> req_id;
      
      switch (type)
      {
      case EVENT_VM_START:
        base = new KdwpCmd_Event_Composite::VMStart(req_id);
        break;
        
      case EVENT_SINGLE_STEP:
        base = new KdwpCmd_Event_Composite::SingleStep(req_id);
        break;
        
      case EVENT_BREAKPOINT:
        base = new KdwpCmd_Event_Composite::BreakPoint(req_id);
        break;
        
      case EVENT_METHOD_ENTRY:
        base = new KdwpCmd_Event_Composite::MethodEntry(req_id);
        break;
        
      case EVENT_METHOD_EXIT:
        base = new KdwpCmd_Event_Composite::MethodExit(req_id);
        break;
        
      case EVENT_EXCEPTION:
        base = new KdwpCmd_Event_Composite::Exception(req_id);
        break;
        
      case EVENT_THREAD_START:
        base = new KdwpCmd_Event_Composite::ThreadStart(req_id);
        break;
        
      case EVENT_THREAD_END:
        base = new KdwpCmd_Event_Composite::ThreadDeath(req_id);
        break;
        
      case EVENT_CLASS_PREPARE:
        base = new KdwpCmd_Event_Composite::ClassPrepare(req_id);
        break;
        
      case EVENT_CLASS_UNLOAD:
        base = new KdwpCmd_Event_Composite::ClassUnload(req_id);
        break;
        
      case EVENT_FIELD_ACCESS:
        base = new KdwpCmd_Event_Composite::FieldAccess(req_id);
        break;
        
      case EVENT_FIELD_MODIFICATION:
        base = new KdwpCmd_Event_Composite::FieldModification(req_id);
        break;
      }
      
      socket >> (*base);
      
      event.m_events.push_back(base);
    }
    
    return socket;
  }
}
