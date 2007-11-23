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

#ifndef KdwpCmd_Method_LineTable_hpp
#define KdwpCmd_Method_LineTable_hpp

#include <vector>

#include "KdwpCmd_Method.hpp"

namespace jmdb
{
  typedef class KdwpCmd_Method_LineTable KdwpCmd_Method_LineTable;
  class KdwpCmd_Method_LineTable : public KdwpCmd_Method
  {
  private:
    
    struct LineInfo
    {
      uint64_t m_code_index;
      uint32_t m_line_number;
    };
    typedef struct LineInfo LineInfo;
    
    uint32_t m_class_id;
    uint32_t m_method_id;
    
    uint64_t m_start;
    uint64_t m_end;
    
    uint32_t m_line_count;
    std::vector<LineInfo> m_lines;
    
    static uint32_t const EXTRA_HEADER_SIZE = (sizeof(get_size(&KdwpCmd_Method_LineTable::m_class_id)) +
                                               sizeof(get_size(&KdwpCmd_Method_LineTable::m_method_id)));
    
  public:
    
    // Life cycle
    
    KdwpCmd_Method_LineTable(uint32_t const class_id, uint32_t const method_id)
      : KdwpCmd_Method(FLAG_NONE, LINE_TABLE, EXTRA_HEADER_SIZE),
        m_class_id(class_id),
        m_method_id(method_id)
    { }
    
    // Operation
    
    void dump_all_lines() const;
    int32_t get_line_number(uint32_t const offset) const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_Method_LineTable &cmd);
  };
}

#endif
