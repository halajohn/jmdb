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

#ifndef KdwpCmd_ReferenceType_Fields_hpp
#define KdwpCmd_ReferenceType_Fields_hpp

#include <string>
#include <vector>

#include "KdwpCmd_ReferenceType.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ReferenceType_Fields KdwpCmd_ReferenceType_Fields;
  class KdwpCmd_ReferenceType_Fields : public KdwpCmd_ReferenceType
  {
  public:
    
    struct FieldInfo
    {
      // :NOTE: Wei 2004-Jun-19:
      //
      // According to the kdp, field ID is 64-bit.
      //
      // The upper 32-bit is the class id,
      // and the lower 23-bit is the field index.
      uint64_t m_id;
      
      std::string m_name;
      std::string m_signature;
      uint32_t m_access_flags;
    };
    
  private:
    
    // Out data
    
    uint32_t m_reference_id;
    
    static uint32_t const EXTRA_HEADER_SIZE = sizeof(get_size(&KdwpCmd_ReferenceType_Fields::m_reference_id));
    
    // Reply data
    
    uint32_t m_field_count;
    
    std::vector<FieldInfo> m_fields;
    
  public:
    
    // Life cycle
    
    KdwpCmd_ReferenceType_Fields(uint32_t const reference_id)
      : KdwpCmd_ReferenceType(FLAG_NONE, FIELDS, EXTRA_HEADER_SIZE),
        m_reference_id(reference_id)
    { }
    
    // Operation
    
    void dump_all_fields() const;
    FieldInfo const *get_field_info(std::string const &name) const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ReferenceType_Fields &cmd);
  };
}

#endif
