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

#include "DbgCmd.hpp"

namespace jmdb
{
  //////////////////////////////// Protected //////////////////////////////////
  //============================== Life cycle =================================
  
  DbgCmdBase::~DbgCmdBase()
  {
  }
  
  //============================== Operation ==================================
  
  std::auto_ptr<std::string>
  DbgCmdBase::pick_command(std::string &command_list)
  {
    std::auto_ptr<std::string> command(new std::string);
    std::string::size_type loc = command_list.find(' ', 0);
    
    if (loc != std::string::npos)
    {
      *command = command_list.substr(0, loc);
      
      // skip white space.
      std::string::size_type const max_index = command_list.size() - 1;
      
      while (1)
      {
        ++loc;
        
        if (loc > max_index)
        {
          loc = std::string::npos;
          break;
        }
        else if (' ' != command_list[loc])
        {
          break;
        }
      }
      
      command_list.erase(0, loc);
    }
    else
    {
      *command = command_list;
      command_list.erase();
    }
    
    return command;
  }
}
