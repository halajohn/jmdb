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

#ifndef DbgCmd_Info_Field_hpp
#define DbgCmd_Info_Field_hpp

#include "DbgCmd.hpp"
#include "../Type.hpp"

namespace jmdb
{
  typedef class DbgCmd_Info_Field DbgCmd_Info_Field;
  class DbgCmd_Info_Field : public DbgCmd<NO_SUB_MENU>
  {
  private:
    
    void print_help_0arg();
    
    void get_field_info_0arg();
    void get_field_info_1arg(std::string const &class_id_string);
    
  public:
    
    // Life cycle
    
    DbgCmd_Info_Field(Debugger * const debugger,
                      std::string const &name)
      : DbgCmd<NO_SUB_MENU>(debugger, name)
    { }
    
    // Operation
    
    void do_command(std::string &command_list);
    void do_help(std::string &command_list);
  };
}

#endif
