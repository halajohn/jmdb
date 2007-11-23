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

#include "Constant.hpp"

namespace jmdb
{
  std::auto_ptr<std::string>
  get_access_flag_string(uint32_t const access_flags)
  {
    bool printed = false;
    
    std::auto_ptr<std::string> str(new std::string());
    
    if (access_flags & ACC_PUBLIC)
    {
      (*str).append("Public");
      
      printed = true;
    }
    
    if (access_flags & ACC_PRIVATE)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Private");
      
      printed = true;
    }
    
    if (access_flags & ACC_PROTECTED)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Protected");
      
      printed = true;
    }
    
    if (access_flags & ACC_STATIC)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Static");
      
      printed = true;
    }
    
    if (access_flags & ACC_FINAL)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Final");
      
      printed = true;
    }
    
    if (access_flags & ACC_SYNCHRONIZED)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Synchronized");
      
      printed = true;
    }
    
    if (access_flags & ACC_VOLATILE)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Volatile");
      
      printed = true;
    }
    
    if (access_flags & ACC_TRANSIENT)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Transient");
      
      printed = true;
    }
    
    if (access_flags & ACC_NATIVE)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Native");
      
      printed = true;
    }
    
    if (access_flags & ACC_INTERFACE)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Interface");
      
      printed = true;
    }
    
    if (access_flags & ACC_ABSTRACT)
    {
      if (true == printed)
      {
        (*str).append(", ");
      }
      
      (*str).append("Abstract");
      
      printed = true;
    }
    
    return str;
  }
  
  std::auto_ptr<std::string>
  class_type_to_string(ClassTypeEnum const type)
  {
    std::auto_ptr<std::string> str(new std::string());
    
    switch (type)
    {
    case CT_CLASS:
      *str = "Class";
      break;
      
    case CT_INTERFACE:
      *str = "Interface";
      break;
      
    case CT_ARRAY:
      *str = "Array";
      break;
      
    default:
      assert(1 != 1);
      break;
    }
    
    return str;
  }
}
