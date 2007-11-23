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

#ifndef Value_hpp
#define Value_hpp

#include <cassert>
#include <string>
#include <iostream>

#include "../Type.hpp"

namespace jmdb
{
  // Value enum
  
  enum ValueTypeEnum
  {
    V_ARRAY = 91,
    V_BYTE = 66,
    V_CHAR = 67,
    V_OBJECT = 76,
    V_FLOAT = 70,
    V_DOUBLE = 68,
    V_INT = 73,
    V_LONG = 74,
    V_SHORT = 83,
    V_VOID = 86,
    V_BOOLEAN = 90,
    V_STRING = 115,
    V_THREAD = 116,
    V_THREAD_GROUP = 103,
    V_CLASS_LOADER = 108,
    V_CLASS_OBJECT = 99
  };
  typedef enum ValueTypeEnum ValueTypeEnum;
  
  // Value traits class
  
  template<ValueTypeEnum T_enum>
  struct ValueTraits;
  
  template<>
  struct ValueTraits<V_ARRAY>
  {
    static uint8_t const ID = V_ARRAY;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_BYTE>
  {
    static uint8_t const ID = V_BYTE;
    static std::string const VALUE_NAME;
    typedef uint8_t DataType;
  };
  
  template<>
  struct ValueTraits<V_CHAR>
  {
    static uint8_t const ID = V_CHAR;
    static std::string const VALUE_NAME;
    typedef uint16_t DataType;
  };
  
  template<>
  struct ValueTraits<V_OBJECT>
  {
    static uint8_t const ID = V_OBJECT;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_FLOAT>
  {
    static uint8_t const ID = V_FLOAT;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_DOUBLE>
  {
    static uint8_t const ID = V_DOUBLE;
    static std::string const VALUE_NAME;
    typedef uint64_t DataType;
  };
  
  template<>
  struct ValueTraits<V_INT>
  {
    static uint8_t const ID = V_INT;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_LONG>
  {
    static uint8_t const ID = V_LONG;
    static std::string const VALUE_NAME;
    typedef uint64_t DataType;
  };
  
  template<>
  struct ValueTraits<V_SHORT>
  {
    static uint8_t const ID = V_SHORT;
    static std::string const VALUE_NAME;
    typedef uint16_t DataType;
  };
  
  template<>
  struct ValueTraits<V_VOID>
  {
    static uint8_t const ID = V_VOID;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType; // shouldn't be used
  };
  
  template<>
  struct ValueTraits<V_BOOLEAN>
  {
    static uint8_t const ID = V_BOOLEAN;
    static std::string const VALUE_NAME;
    typedef uint8_t DataType;
  };
  
  template<>
  struct ValueTraits<V_STRING>
  {
    static uint8_t const ID = V_STRING;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_THREAD>
  {
    static uint8_t const ID = V_THREAD;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_THREAD_GROUP>
  {
    static uint8_t const ID = V_THREAD_GROUP;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_CLASS_LOADER>
  {
    static uint8_t const ID = V_CLASS_LOADER;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  template<>
  struct ValueTraits<V_CLASS_OBJECT>
  {
    static uint8_t const ID = V_CLASS_OBJECT;
    static std::string const VALUE_NAME;
    typedef uint32_t DataType;
  };
  
  // Forward declaration
  //
  
  class ClientSocket;
  
  template<ValueTypeEnum T_enum, typename T_traits>
  struct Value;
  
  template<ValueTypeEnum T_enum, typename T_traits>
  ClientSocket const &operator>>(ClientSocket const &socket, Value<T_enum, T_traits> const &value);
  
  struct ValueBase
  {
    virtual ~ValueBase() = 0;
    virtual void dump_to_console() const = 0;
    virtual void get_from_socket(ClientSocket const &socket) = 0;
    
    // :NOTE: Wei 2004-Jun-19:
    //
    // Becuase uint64_t is the largest possible data type that a Value can hold.
    // Thus I use uint64_t as the return type.
    virtual uint64_t get_value() const = 0;
  };
  
  template<ValueTypeEnum T_enum, typename T_traits = ValueTraits<T_enum> >
  struct Value : public ValueBase
  {
    uint8_t m_type;
    typename T_traits::DataType m_data;
    
    Value()
    {
      m_type = T_traits::ID;
    }
    
    virtual void
    dump_to_console() const
    {
      std::cout << "(" << T_traits::VALUE_NAME << ")"
                << std::hex << m_data;
    }
    
    virtual void
    get_from_socket(ClientSocket const &socket)
    {
      if (T_enum != V_VOID)
      {
        socket >> m_data;
      }
    }
    
    virtual uint64_t
    get_value() const
    {
      if (V_VOID == T_enum)
      {
        assert(1 != 1);
      }
      
      return m_data;
    }
    
    friend ClientSocket const &operator>><T_enum, T_traits>(ClientSocket const &socket, Value<T_enum, T_traits> const &value);
  };
  
  ClientSocket const &operator>>(ClientSocket const &socket, ValueBase &value);
}

#endif
