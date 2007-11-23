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

#ifndef KdwpCmd_Method_VariableTable_hpp
#define KdwpCmd_Method_VariableTable_hpp

#include <vector>

#include "KdwpCmd_Method.hpp"

namespace jmdb
{
  typedef class KdwpCmd_Method_VariableTable KdwpCmd_Method_VariableTable;
  class KdwpCmd_Method_VariableTable : public KdwpCmd_Method
  {
  public:
    
    struct VariableInfo
    {
      uint64_t m_code_index;
      std::string m_name;
      std::string m_signature;
      uint32_t m_length;
      uint32_t m_slot_index;
    };
    typedef struct VariableInfo VariableInfo;
    
  private:
    
    // Out data
    
    uint32_t m_class_id;
    uint32_t m_method_id;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_Method_VariableTable::m_class_id)) +
                                               sizeof(get_size(&KdwpCmd_Method_VariableTable::m_method_id)));
    
    // Reply data
    
    uint32_t m_arg_count;
    uint32_t m_slot_count;
    
    std::vector<VariableInfo> m_slots;
    
  public:
    
    // Life cycle
    
    KdwpCmd_Method_VariableTable(uint32_t const class_id, uint32_t const method_id)
      : KdwpCmd_Method(FLAG_NONE, VARIABLE_TABLE, EXTRA_HEADER_SIZE),
        m_class_id(class_id),
        m_method_id(method_id)
    { }
    
    // Operation
    
    void dump_all_variables() const;
    VariableInfo const *get_variable_info(std::string const &name) const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_Method_VariableTable &cmd);
  };
}

#endif
