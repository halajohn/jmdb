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

#ifndef MemFunctor_hpp
#define MemFunctor_hpp

namespace jmdb
{
  // 0 arg
  
  template<typename T_return, typename T_obj>
  class MemFunctor0
  {
  private:
    
    typedef T_return (T_obj::*FunctionPtr)();
    
    T_obj * const mp_obj;
    FunctionPtr mp_func;
    
  public:
    
    explicit MemFunctor0(T_obj * const p_obj, FunctionPtr p_func)
      : mp_obj(p_obj),
        mp_func(p_func)
    { }
    
    T_return
    operator()() const
    {
      return (mp_obj->*(this->mp_func))();
    }
  };
  
  template<typename T_return, typename T_obj>
  MemFunctor0<T_return, T_obj>
  mem_fun(T_obj * const p_obj, T_return (T_obj::*p_func)())
  {
    return MemFunctor0<T_return, T_obj>(p_obj, p_func);
  }
  
  // 1 arg
  
  template<typename T_return, typename T_obj, typename T_arg1>
  class MemFunctor1
  {
  private:
    
    typedef T_return (T_obj::*FunctionPtr)(T_arg1);
    
    T_obj * const mp_obj;
    FunctionPtr mp_func;
    
  public:
    
    explicit MemFunctor1(T_obj * const p_obj, FunctionPtr p_func)
      : mp_obj(p_obj),
        mp_func(p_func)
    { }
    
    T_return
    operator()(T_arg1 arg) const
    {
      return (mp_obj->*(this->mp_func))(arg);
    }
  };
  
  template<typename T_return, typename T_obj, typename T_arg1>
  MemFunctor1<T_return, T_obj, T_arg1>
  mem_fun(T_obj * const p_obj, T_return (T_obj::*p_func)(T_arg1))
  {
    return MemFunctor1<T_return, T_obj, T_arg1>(p_obj, p_func);
  }
  
  // 2 arg
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
  class MemFunctor2
  {
  private:
    
    typedef T_return (T_obj::*FunctionPtr)(T_arg1, T_arg2);
    
    T_obj * const mp_obj;
    FunctionPtr mp_func;
    
  public:
    
    explicit MemFunctor2(T_obj * const p_obj, FunctionPtr p_func)
      : mp_obj(p_obj),
        mp_func(p_func)
    { }
    
    T_return
    operator()(T_arg1 arg1, T_arg2 arg2) const
    {
      return (mp_obj->*(this->mp_func))(arg1, arg2);
    }
  };
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2>
  MemFunctor2<T_return, T_obj, T_arg1, T_arg2>
  mem_fun(T_obj * const p_obj, T_return (T_obj::*p_func)(T_arg1, T_arg2))
  {
    return MemFunctor2<T_return, T_obj, T_arg1, T_arg2>(p_obj, p_func);
  }
  
  // 3 arg
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
  class MemFunctor3
  {
  private:
    
    typedef T_return (T_obj::*FunctionPtr)(T_arg1, T_arg2, T_arg3);
    
    T_obj * const mp_obj;
    FunctionPtr mp_func;
    
  public:
    
    explicit MemFunctor3(T_obj * const p_obj, FunctionPtr p_func)
      : mp_obj(p_obj),
        mp_func(p_func)
    { }
    
    T_return
    operator()(T_arg1 arg1, T_arg2 arg2, T_arg3 arg3) const
    {
      return (mp_obj->*(this->mp_func))(arg1, arg2, arg3);
    }
  };
  
  template<typename T_return, typename T_obj, typename T_arg1, typename T_arg2, typename T_arg3>
  MemFunctor3<T_return, T_obj, T_arg1, T_arg2, T_arg3>
  mem_fun(T_obj * const p_obj, T_return (T_obj::*p_func)(T_arg1, T_arg2, T_arg3))
  {
    return MemFunctor3<T_return, T_obj, T_arg1, T_arg2, T_arg3>(p_obj, p_func);
  }
}

#endif
