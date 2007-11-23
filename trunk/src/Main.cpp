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

#include <cstdlib> // for EXIT_SUCCESS & EXIT_FAILURE
#include <csignal>

#include "Debugger.hpp"

void
sigint_handler(int /* signum */)
{
}

int
main(int argc, char **argv)
{
  setlocale(LC_ALL, "");
  //bind_textdomain_codeset("jmdb", "UTF-8");
  bind_textdomain_codeset("jmdb", "Big5");
  textdomain("jmdb");
  
  jmdb::Debugger debugger;
  struct sigaction new_action, old_action;
  
  debugger.init(argc, argv);
  
  new_action.sa_handler = sigint_handler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = 0;
  
  sigaction(SIGINT, &new_action, &old_action);
  
  debugger.run();
  
  sigaction(SIGINT, &old_action, 0);
  
  return EXIT_SUCCESS;
}
