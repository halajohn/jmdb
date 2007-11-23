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

#ifndef DbgCmd_Thread_Suspend_hpp
#define DbgCmd_Thread_Suspend_hpp

#include <memory>
#include <string>

#include "DbgCmd.hpp"
#include "../Type.hpp"

namespace jmdb
{
  typedef class DbgCmd_Thread_Suspend DbgCmd_Thread_Suspend;
  class DbgCmd_Thread_Suspend : public DbgCmd<HAS_SUB_MENU>
  {
  private:
    
    // Sub command func
    
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_all;
    
    // Operation
    
    void print_help_0arg();
    
    void suspend_thread_0arg();
    void suspend_thread_1arg(std::string const &thread_id_string);
    
  public:
    
    // Life cycle
    
    DbgCmd_Thread_Suspend(Debugger * const debugger,
                          std::string const &name);
    
    // Operation
    
    void do_command(std::string &command_list);
    void do_help(std::string &command_list);
  };
}

#endif
