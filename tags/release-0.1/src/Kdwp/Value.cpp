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

#include "Value.hpp"

namespace jmdb
{
  ValueBase::~ValueBase()
  {
  }
  
  std::string const ValueTraits<V_ARRAY>::VALUE_NAME = "Array";
  std::string const ValueTraits<V_BYTE>::VALUE_NAME = "Byte";
  std::string const ValueTraits<V_CHAR>::VALUE_NAME = "Char";
  std::string const ValueTraits<V_OBJECT>::VALUE_NAME = "Object";
  std::string const ValueTraits<V_FLOAT>::VALUE_NAME = "Float";
  std::string const ValueTraits<V_DOUBLE>::VALUE_NAME = "Double";
  std::string const ValueTraits<V_INT>::VALUE_NAME = "Int";
  std::string const ValueTraits<V_LONG>::VALUE_NAME = "Long";
  std::string const ValueTraits<V_SHORT>::VALUE_NAME = "Short";
  std::string const ValueTraits<V_VOID>::VALUE_NAME = "Void";
  std::string const ValueTraits<V_BOOLEAN>::VALUE_NAME = "Boolean";
  std::string const ValueTraits<V_STRING>::VALUE_NAME = "String";
  std::string const ValueTraits<V_THREAD>::VALUE_NAME = "Thread";
  std::string const ValueTraits<V_THREAD_GROUP>::VALUE_NAME = "Thread Group";
  std::string const ValueTraits<V_CLASS_LOADER>::VALUE_NAME = "Class Loader";
  std::string const ValueTraits<V_CLASS_OBJECT>::VALUE_NAME = "Class Object";
  
  ClientSocket const &
  operator>>(ClientSocket const &socket, ValueBase &value)
  {
    value.get_from_socket(socket);
    
    return socket;
  }
}
