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

#ifndef KdwpCmd_ReferenceType_Signature_hpp
#define KdwpCmd_ReferenceType_Signature_hpp

#include <string>

#include "KdwpCmd_ReferenceType.hpp"

namespace jmdb
{
  typedef class KdwpCmd_ReferenceType_Signature KdwpCmd_ReferenceType_Signature;
  class KdwpCmd_ReferenceType_Signature : public KdwpCmd_ReferenceType
  {
  private:
    
    uint32_t m_reference_id;
    
    std::string m_signature;
    
    static uint32_t const EXTRA_HEADER_SIZE = sizeof(get_size(&KdwpCmd_ReferenceType_Signature::m_reference_id));
    
  public:
    
    // Life cycle
    
    KdwpCmd_ReferenceType_Signature(uint32_t const reference_id)
      : KdwpCmd_ReferenceType(FLAG_NONE, SIGNATURE, EXTRA_HEADER_SIZE),
        m_reference_id(reference_id)
    { }
    
    // Access
    
    std::string &
    signature()
    {
      return m_signature;
    }
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_ReferenceType_Signature &cmd);
  };
}

#endif
