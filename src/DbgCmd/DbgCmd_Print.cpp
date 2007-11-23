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

#include "DbgCmd_Print.hpp"
#include "MemFunctor.hpp"

#include "../Kdwp/KdwpCmd_ThreadReference_Frames.hpp"
#include "../Kdwp/KdwpCmd_Method_VariableTable.hpp"
#include "../Kdwp/KdwpCmd_StackFrame_GetValues.hpp"
#include "../Kdwp/KdwpCmd_StackFrame_ThisObject.hpp"
#include "../Kdwp/KdwpCmd_ReferenceType_Fields.hpp"
#include "../Kdwp/KdwpCmd_ReferenceType_GetValues.hpp"
#include "../Kdwp/KdwpCmd_ObjectReference_GetValues.hpp"
#include "../Kdwp/Constant.hpp"

#include "../Debugger.hpp"
#include "../Exception.hpp"

#include "DbgCmd.tcc"

namespace jmdb
{
  ///////////////////////////////// Private ///////////////////////////////////
  //============================ Operation ====================================
  
  void
  DbgCmd_Print::print_help_0arg()
  {
    std::cout << std::endl;
    std::cout << gettext(" Description  : Print value of the specified variable.") << std::endl;
    std::cout << gettext(" Syntax       : print <variable name>") << std::endl;
    std::cout << std::endl;
  }
  
  void
  DbgCmd_Print::print_variable_0arg()
  {
    print_variable_1arg(mp_debugger->last_printed_variable_name());
  }
  
  void
  DbgCmd_Print::print_variable_1arg(std::string const &name)
  {
    mp_debugger->last_printed_variable_name() = name;
    
    // See if it is a static or instance variable
    
    KdwpCmd_ReferenceType_Fields field_cmd(mp_debugger->get_last_event_class_id());
    
    mp_debugger->socket() << field_cmd;
    
    KdwpCmd_ReferenceType_Fields::FieldInfo const *fi;
    
    if ((fi = field_cmd.get_field_info(name)) != 0)
    {
      // This is a static or instance variable.
      
      if (fi->m_access_flags & ACC_STATIC)
      {
        // This is a static variable.
        
        KdwpCmd_ReferenceType_GetValues static_field_cmd(mp_debugger->get_last_event_class_id());
        
        static_field_cmd.add_wanted_field(fi->m_id);
        
        mp_debugger->socket() << static_field_cmd;
        
        std::cout << name << " = " << std::dec << static_field_cmd.get_field_value(0) << std::endl;
      }
      else
      {
        // This is a instance variable.
        
        // get this object
        KdwpCmd_ThreadReference_Frames frame_cmd(mp_debugger->get_last_event_thread_id(),
                                                 0, 1);
        
        mp_debugger->socket() << frame_cmd;
        
        KdwpCmd_StackFrame_ThisObject this_object_cmd(mp_debugger->get_last_event_thread_id(),
                                                      frame_cmd.get_last_frame_id());
        
        mp_debugger->socket() << this_object_cmd;
        
        KdwpCmd_ObjectReference_GetValues instance_field_cmd(mp_debugger->get_last_event_class_id(),
                                                             this_object_cmd.get_this_object_id());
        
        instance_field_cmd.add_wanted_field(fi->m_id);
        
        mp_debugger->socket() << instance_field_cmd;
        
        std::cout << name << " = " << std::dec << instance_field_cmd.get_field_value(0) << std::endl;
      }
    }
    else
    {
      // See if it is a local variable
      
      KdwpCmd_ThreadReference_Frames frame_cmd(mp_debugger->get_last_event_thread_id(),
                                               0, 1);
      
      mp_debugger->socket() << frame_cmd;
      
      int32_t const frame_id = frame_cmd.get_last_frame_id();
      
      if (-1 == frame_id)
      {
        // :NOTE: Wei 2004-Jun-19:
        //
        // There is no current frame.
        std::cout << gettext("Can't find the variable named ") << name << std::endl;
      }
      else
      {
        // This is a local variable
        
        KdwpCmd_Method_VariableTable vari_cmd(mp_debugger->get_last_event_class_id(),
                                              mp_debugger->get_last_event_method_id());
        
        mp_debugger->socket() << vari_cmd;
        
        KdwpCmd_Method_VariableTable::VariableInfo const *vi;
        
        if ((vi = vari_cmd.get_variable_info(name)) != 0)
        {
          KdwpCmd_StackFrame_GetValues get_value_cmd(mp_debugger->get_last_event_thread_id(),
                                                     frame_id);
          
          get_value_cmd.add_wanted_variable(vi->m_slot_index, vi->m_signature[0]);
          
          mp_debugger->socket() << get_value_cmd;
          
          std::cout << name << " = " << std::dec << get_value_cmd.get_variable_value(0) << std::endl;
        }
        else
        {
          // :NOTE: Wei 2004-Jun-19:
          //
          // There is no variable named @name in the lastest frame.
          std::cout << gettext("Can't find the variable named ") << name << std::endl;
        }
      }
    }
  }
  
  ////////////////////////////// Public ///////////////////////////////////////
  //============================ Operation ====================================
  
  void
  DbgCmd_Print::do_command(std::string &command_list)
  {
    try
    {
      do_command_by_category<DO_COMMAND>(command_list,
                                         mem_fun(this, &DbgCmd_Print::print_variable_0arg),
                                         mem_fun(this, &DbgCmd_Print::print_variable_1arg));
    }
    catch (BadCommand &/* e */)
    {
      std::cerr << "Invalid arguments." << std::endl;
      
      print_help_0arg();
    }
  }
  
  void
  DbgCmd_Print::do_help(std::string &command_list)
  {
    do_command_by_category<DO_HELP>(command_list,
                                    mem_fun(this, &DbgCmd_Print::print_help_0arg));
  }
}
