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

#ifndef DbgCmd_Top_hpp
#define DbgCmd_Top_hpp

#include <map>
#include <string>

#include "DbgCmd.hpp"

namespace jmdb
{
  typedef class DbgCmd_Top DbgCmd_Top;
  class DbgCmd_Top : public DbgCmd<HAS_SUB_MENU>
  {
  private:
    
    // Sub command func
    
    std::auto_ptr<DbgCmd<HAS_SUB_MENU> > mp_thread;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_quit;
    std::auto_ptr<DbgCmd<HAS_SUB_MENU> > mp_info;
    std::auto_ptr<DbgCmd<HAS_SUB_MENU> > mp_help;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_break;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_continue;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_next;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_print;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_up;
    std::auto_ptr<DbgCmd<NO_SUB_MENU> > mp_down;
    
    // Operation
    
    void print_help_0arg();
    
    void search_cmd_0arg();
    
  public:
    
    // Life cycle
    
    DbgCmd_Top(Debugger * const debugger);
    
    // Operation
    
    void do_command(std::string &command_list);
  };
}

#endif
