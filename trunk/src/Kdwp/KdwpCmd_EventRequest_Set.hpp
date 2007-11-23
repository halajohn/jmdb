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

#ifndef KdwpCmd_EventRequest_Set_hpp
#define KdwpCmd_EventRequest_Set_hpp

#include <cassert>
#include <vector>

#include "Location.hpp"
#include "KdwpCmd_EventRequest.hpp"
#include "Constant.hpp"

namespace jmdb
{
  typedef class KdwpCmd_EventRequest_Set KdwpCmd_EventRequest_Set;
  class KdwpCmd_EventRequest_Set : public KdwpCmd_EventRequest
  {
  private:
    
    enum ModifierTypeEnum
    {
      MOD_COUNT = 1,
      MOD_CONDITIONAL = 2,
      MOD_THREAD_ONLY = 3,
      MOD_CLASS_ONLY = 4,
      MOD_CLASS_MATCH = 5,
      MOD_CLASS_EXCLUDE = 6,
      MOD_LOCATION_ONLY = 7,
      MOD_EXCEPTION_ONLY = 8,
      MOD_FIELD_ONLY = 9,
      MOD_STEP = 10,
      MOD_INSTANCE_ONLY = 11
    };
    typedef enum ModifierTypeEnum ModifierTypeEnum;
    
    struct ModBase
    {
      uint8_t m_kind;
      
      ModBase(ModifierTypeEnum const type)
        : m_kind(static_cast<uint8_t>(type))
      { }
      
      virtual ~ModBase() = 0;
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const = 0;
    };
    
    struct ModCount : public ModBase
    {
      uint32_t m_count;
      
      ModCount()
        : ModBase(MOD_COUNT)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModCond : public ModBase
    {
      uint32_t m_expr_id;
      
      ModCond()
        : ModBase(MOD_CONDITIONAL)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModThreadOnly : public ModBase
    {
      uint32_t m_thread_id;
      
      ModThreadOnly()
        : ModBase(MOD_THREAD_ONLY)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModClassOnly : public ModBase
    {
      uint32_t m_class_id;
      
      ModClassOnly()
        : ModBase(MOD_CLASS_ONLY)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModClassMatch : public ModBase
    {
      std::string m_class_pattern;
      
      ModClassMatch()
        : ModBase(MOD_CLASS_MATCH)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModClassExclude : public ModBase
    {
      std::string m_class_pattern;
      
      ModClassExclude()
        : ModBase(MOD_CLASS_EXCLUDE)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModLocationOnly : public ModBase
    {
      Location m_location;
      
      ModLocationOnly(uint8_t const tag,
                      uint32_t const class_id,
                      uint32_t const method_id,
                      uint64_t const offset)
        : ModBase(MOD_LOCATION_ONLY),
          m_location(tag, class_id, method_id, offset)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModExceptionOnly : public ModBase
    {
      uint32_t m_exception;
      uint8_t m_caught;
      uint8_t m_uncaught;
      
      ModExceptionOnly()
        : ModBase(MOD_EXCEPTION_ONLY)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModFieldOnly : public ModBase
    {
      uint32_t m_declaring;
      uint32_t m_field_id;
      
      ModFieldOnly()
        : ModBase(MOD_FIELD_ONLY)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModStep : public ModBase
    {
      uint32_t m_thread_id;
      uint32_t m_size;
      uint32_t m_depth;
      
      ModStep(uint32_t const thread_id, StepSizeEnum const ss, StepDepthEnum const sd)
        : ModBase(MOD_STEP),
          m_thread_id(thread_id),
          m_size(ss),
          m_depth(sd)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    struct ModInstanceOnly : public ModBase
    {
      uint32_t m_instance;
      
      ModInstanceOnly()
        : ModBase(MOD_INSTANCE_ONLY)
      { }
      
      virtual ClientSocket const &DumpToSocket(ClientSocket const &socket) const;
    };
    
    uint8_t m_event_kind;
    uint8_t m_suspend_policy;
    uint32_t m_modifier_count;
    
    std::vector<ModBase const *> m_modifier;
    
    uint32_t m_request_id;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_EventRequest_Set::m_event_kind)) +
                                               sizeof(get_size(&KdwpCmd_EventRequest_Set::m_suspend_policy)) +
                                               sizeof(get_size(&KdwpCmd_EventRequest_Set::m_modifier_count)));
    
    // Operation
    
    uint32_t count_extra_modifier_size() const;
    
  public:
    
    // Life cycle
    
    KdwpCmd_EventRequest_Set()
      : KdwpCmd_EventRequest(FLAG_NONE, SET, EXTRA_HEADER_SIZE),
        m_suspend_policy(SP_ALL),
        m_modifier_count(0)
    { }
    
    ~KdwpCmd_EventRequest_Set();
    
    // Operation
    
    void set_breakpoint(uint32_t const class_id, uint32_t const method_id, uint64_t const offset);
    void set_running_next(uint32_t const thread_id);
    void set_running_step(uint32_t const thread_id);
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_EventRequest_Set &cmd);
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_EventRequest_Set::ModBase const &mod);
  };
}

#endif
