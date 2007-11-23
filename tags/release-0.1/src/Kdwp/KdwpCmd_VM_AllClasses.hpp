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

#ifndef KdwpCmd_VM_AllClasses_hpp
#define KdwpCmd_VM_AllClasses_hpp

// System include
//

#include <vector>

// Local include
//

#include "KdwpCmd_VM.hpp"
#include "Constant.hpp"

namespace jmdb
{
  typedef class KdwpCmd_VM_AllClasses KdwpCmd_VM_AllClasses;
  class KdwpCmd_VM_AllClasses : public KdwpCmd_VM
  {
  private:
    
    enum ClassStatus
    {
      VERIFIED = 1,
      PREPARED = 2,
      INITIALIZED = 4,
      ERROR = 8
    };
    typedef enum ClassStatus ClassStatus;
    
    struct ClassInfo
    {
      uint8_t m_type;
      uint32_t m_id;
      std::string m_signature;
      uint32_t m_status;
    };
    typedef struct ClassInfo ClassInfo;
    
    uint32_t m_class_count;
    std::vector<ClassInfo> m_classes;
    
    // Operation
    
    std::auto_ptr<std::string> get_type_string(ClassTypeEnum const tag) const;
    std::auto_ptr<std::string> get_status_string(ClassStatus const status) const;
    
  public:
    
    // Life cycle
    
    KdwpCmd_VM_AllClasses()
      : KdwpCmd_VM(FLAG_NONE, ALL_CLASSES, 0)
    { }
    
    // Operation
    
    void dump_all_classes() const;
    
    // Friend
    
    friend ClientSocket const &operator<<(ClientSocket const &socket, KdwpCmd_VM_AllClasses &cmd);
  };
}

#endif
