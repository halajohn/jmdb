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

#ifndef KdwpCmd_ReferenceType_Methods_hpp
#define KdwpCmd_ReferenceType_Methods_hpp

#include <string>
#include <vector>

#include "KdwpCmd_ReferenceType.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ReferenceType_Methods KdwpCmd_ReferenceType_Methods;
  class KdwpCmd_ReferenceType_Methods : public KdwpCmd_ReferenceType
  {
  private:
    
    struct MethodInfo
    {
      uint32_t m_id;
      std::string m_name;
      std::string m_signature;
      uint32_t m_access_flags;
    };
    
    uint32_t m_reference_id;
    
    uint32_t m_method_count;
    
    std::vector<MethodInfo> m_methods;
    
    static uint32_t const EXTRA_HEADER_SIZE = sizeof(get_size(&KdwpCmd_ReferenceType_Methods::m_reference_id));
    
  public:
    
    // Life cycle
    
    KdwpCmd_ReferenceType_Methods(uint32_t const reference_id)
      : KdwpCmd_ReferenceType(FLAG_NONE, METHODS, EXTRA_HEADER_SIZE),
        m_reference_id(reference_id)
    { }
    
    // Operation
    
    void dump_all_methods() const;
    std::string const &get_method_name(uint32_t const method_id) const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ReferenceType_Methods &cmd);
  };
}

#endif
