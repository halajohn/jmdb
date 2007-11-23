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

#include <cassert>
#include <iostream>

#include "KdwpCmd_VM_AllClasses.hpp"
#include "Constant.hpp"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //=============================== Operation =================================
    
  std::auto_ptr<std::string>
  KdwpCmd_VM_AllClasses::get_status_string(ClassStatus const status) const
  {
    bool printed = false;
    
    std::auto_ptr<std::string> str(new std::string());
    
    if (status & VERIFIED)
    {
      (*str).append("Verified");
      
      printed = true;
    }
    
    if (status & PREPARED)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Prepared");
      
      printed = true;
    }
    
    if (status & INITIALIZED)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Initialized");
      
      printed = true;
    }
    
    if (status & ERROR)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Error");
      
      printed = true;
    }
    
    return str;
  }
  
  ///////////////////////////////// Public ////////////////////////////////////
  //=============================== Operation =================================
  
  void
  KdwpCmd_VM_AllClasses::dump_all_classes() const
  {
    std::cout << "There are " << std::dec << m_class_count << " class"
              << ((m_class_count <= 1) ? "" : "es") << " in the KVM." << std::endl;
    
    std::cout << "Class" << ((m_class_count <= 1) ? "" : "es")
              << " information " << ((m_class_count <= 1) ? "is: " : "are: ") << std::endl;
    
    std::cout << std::endl;
    
    for (std::vector<ClassInfo>::const_iterator iter = m_classes.begin();
         iter != m_classes.end();
         ++iter)
    {
      std::cout << std::hex << iter->m_id << " ";
      std::cout << *(class_type_to_string(static_cast<ClassTypeEnum>(iter->m_type))) << " ";
      std::cout << iter->m_signature << " ";
      std::cout << *(get_status_string(static_cast<ClassStatus>(iter->m_status))) << std::endl;
    }
    
    std::cout << std::endl;
  }
  
  //=============================== Friend ====================================
  
  ClientSocket const &
  operator<<(ClientSocket const &socket, KdwpCmd_VM_AllClasses &cmd)
  {
    socket << static_cast<PacketHeader &>(cmd);
    
    // Reply
    
    socket >> static_cast<PacketHeader &>(cmd);
    
    switch (cmd.get_error_code())
    {
    case ERROR_NONE:
      socket >> cmd.m_class_count;
      
      for (uint32_t i = 0; i < cmd.m_class_count; ++i)
      {
        KdwpCmd_VM_AllClasses::ClassInfo ci;
        
        socket >> ci.m_type;
        socket >> ci.m_id;
        socket >> ci.m_signature;
        socket >> ci.m_status;
        
        cmd.m_classes.push_back(ci);
      }
      break;
      
    case ERROR_VM_DEAD:
      assert(1 != 1);
      break;
      
    default:
      assert(1 != 1);
      break;
    }
    
    return socket;
  }
}
