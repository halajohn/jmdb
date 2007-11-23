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

#ifndef Debugger_hpp
#define Debugger_hpp

#include <sys/termios.h>

#include <memory>
#include <iostream>
#include <deque>

#include "Type.hpp"
#include "ClientSocket.hpp"
#include "History.hpp"

namespace jmdb
{
  // Forward declaration
  //
  
  class DbgCmd_Top;
  class KdwpCmd_Event_Composite;
  class SrcFiles;
  class Location;
  
  typedef class Debugger Debugger;
  class Debugger
  {
  private:
    
    enum KeyEnum
    {
      // ASCII code table
      KEY_ETX = 0x3, // end of text
      
      KEY_BS = 0x8,
      KEY_TAB = 0x9,
      KEY_LF = 0xA,
      KEY_CR = 0xD,
      KEY_ESC = 0x1B,
      KEY_SPACE = 0x20,
      KEY_EXCLAMATION = 0x21, // !
      KEY_QUOTATION = 0x22, // "
      KEY_NUMBER_SIGN = 0x23, // #
      KEY_DOLLAR = 0x24, // $
      KEY_PERCENT = 0x25, // %
      KEY_AMPERSAND = 0x26, // &
      KEY_APOSTROPHE = 0x27, // '
      KEY_LEFT_PAREN = 0x28, // (
      KEY_RIGHT_PAREN = 0x29, // )
      KEY_ASTERISK = 0x2A, // *
      KEY_PLUS = 0x2B, // +
      KEY_COMMA = 0x2C, // ,
      KEY_HYPHEN = 0x2D, // -
      KEY_PERIOD = 0x2E, // .
      KEY_SLASH = 0x2F, // /
      
      KEY_0 = 0x30,
      KEY_1 = 0x31,
      KEY_2 = 0x32,
      KEY_3 = 0x33,
      KEY_4 = 0x34,
      KEY_5 = 0x35,
      KEY_6 = 0x36,
      KEY_7 = 0x37,
      KEY_8 = 0x38,
      KEY_9 = 0x39,
      
      KEY_COLON = 0x3A, // :
      KEY_SEMICOLON = 0x3B, // ;
      KEY_LESS_THAN = 0x3C, // <
      KEY_EQUAL = 0x3D, // =
      KEY_GREATER_THAN = 0x3E, // >
      KEY_QUESTION_MARK = 0x3F, // ?
      KEY_AT = 0x40, // @
      
      KEY_A = 0x41,
      KEY_B = 0x42,
      KEY_C = 0x43,
      KEY_D = 0x44,
      KEY_E = 0x45,
      KEY_F = 0x46,
      KEY_G = 0x47,
      KEY_H = 0x48,
      KEY_I = 0x49,
      KEY_J = 0x4A,
      KEY_K = 0x4B,
      KEY_L = 0x4C,
      KEY_M = 0x4D,
      KEY_N = 0x4E,
      KEY_O = 0x4F,
      KEY_P = 0x50,
      KEY_Q = 0x51,
      KEY_R = 0x52,
      KEY_S = 0x53,
      KEY_T = 0x54,
      KEY_U = 0x55,
      KEY_V = 0x56,
      KEY_W = 0x57,
      KEY_X = 0x58,
      KEY_Y = 0x59,
      KEY_Z = 0x5A,
      
      KEY_LEFT_BRACKET = 0x5B, // [
      KEY_BACK_SLASH = 0x5C,
      KEY_RIGHT_BRACKET = 0x5D, // ]
      KEY_CARET = 0x5E, // ^
      KEY_UNDERSCORE = 0x5F, // _
      KEY_ACUTE_ACCENT = 0x60, // `
      
      KEY_a = 0x61,
      KEY_b = 0x62,
      KEY_c = 0x63,
      KEY_d = 0x64,
      KEY_e = 0x65,
      KEY_f = 0x66,
      KEY_g = 0x67,
      KEY_h = 0x68,
      KEY_i = 0x69,
      KEY_j = 0x6A,
      KEY_k = 0x6B,
      KEY_l = 0x6C,
      KEY_m = 0x6D,
      KEY_n = 0x6E,
      KEY_o = 0x6F,
      KEY_p = 0x70,
      KEY_q = 0x71,
      KEY_r = 0x72,
      KEY_s = 0x73,
      KEY_t = 0x74,
      KEY_u = 0x75,
      KEY_v = 0x76,
      KEY_w = 0x77,
      KEY_x = 0x78,
      KEY_y = 0x79,
      KEY_z = 0x7A,
      
      KEY_LEFT_BRACE = 0x7B, // {
      KEY_VERTICAL_BAR = 0x7C, // |
      KEY_RIGHT_BRACE = 0x7D, // }
      KEY_TILDE = 0x7E, // ~
      
      KEY_DEL = 0x7F,
      
      // :NOTE: Wei 2004-Jun-14:
      //
      // The following is my extension.
      KEY_UP = 0x0100,
      KEY_DOWN = 0x0101,
      KEY_LEFT = 0x0102,
      KEY_RIGHT = 0x0103
    };
    typedef enum KeyEnum KeyEnum;
    
    enum KeyMode
    {
      NORMAL_MODE,
      ESC_MODE,
      BRACKET_MODE
    };
    typedef enum KeyMode KeyMode;
    
    ClientSocket m_socket;
    std::auto_ptr<DbgCmd_Top> mp_dbgcmd_top;
    bool m_quit_flag;
    
    struct termios m_orig_termios;
    struct termios m_new_termios;
    
    History m_history;
    
    struct BreakPoint
    {
      uint32_t m_thread_id;
      uint32_t m_req_id;
    };
    typedef struct BreakPoint BreakPoint;
    
    std::deque<BreakPoint> m_breakpoints;
    
    uint32_t m_last_event_thread_id;
    uint32_t m_last_event_class_id;
    uint32_t m_last_event_method_id;
    
    std::string m_last_printed_variable_name;
    
    uint32_t m_current_frame_id;
    
    std::auto_ptr<SrcFiles> mp_src_files;
    
    bool m_vm_has_suspended;
    bool m_emacs_mode;
    bool m_wait_event;
    std::auto_ptr<KdwpCmd_Event_Composite> mp_last_event;
    
    // Operation
    
    void usage();
    void prompt_usr() const;
    
    KeyEnum getch();
    
    void set_terminal_raw();
    void restore_terminal();
    int32_t get_line_number(uint32_t const class_id,
                            uint32_t const method_id,
                            uint32_t const offset) const;
    
    void
    delete_one_console_char() const
    {
      std::cout << static_cast<char>(0x8);
      std::cout << static_cast<char>(0x20);
      std::cout << static_cast<char>(0x8);
    }
    
    bool wait_event();
    void handle_event();
    
  public:
    
    // Life cycle
    
    Debugger();
    ~Debugger();
    
    // Operation
    
    void init(int argc, char **argv);
    void run();
    void handle_filename_and_line_number(Location const &location);
    bool is_all_thread_suspended() const;
    
    void
    listen_event()
    {
      m_wait_event = true;
    }
    
    void
    set_quit_flag()
    {
      m_quit_flag = true;
    }
    
    uint32_t
    get_last_event_thread_id() const
    { return m_last_event_thread_id; }
    
    uint32_t
    get_last_event_class_id() const
    { return m_last_event_class_id; }
    
    uint32_t
    get_last_event_method_id() const
    { return m_last_event_method_id; }
    
    void
    remember_breakpoint(uint32_t const thread_id, uint32_t const req_id)
    {
      BreakPoint bp;
      
      bp.m_thread_id = thread_id;
      bp.m_req_id = req_id;
      
      m_breakpoints.push_back(bp);
    }
    
    void
    forget_breakpoint(uint32_t const req_id)
    {
      for (std::deque<BreakPoint>::iterator iter = m_breakpoints.begin();
           iter != m_breakpoints.end();
           ++iter)
      {
        if (iter->m_req_id == req_id)
        {
          m_breakpoints.erase(iter);
        }
      }
      
      // :NOTE: Wei 2004-Jun-17:
      //
      // Shouldn't be here.
      // Because we shouldn't forget a non-remembered break point.
      assert(1 != 1);
    }
    
    // Access
    
    ClientSocket &
    socket()
    { return m_socket; }
    
    bool
    emacs_mode() const
    { return m_emacs_mode; }
    
    std::string &
    last_printed_variable_name()
    { return m_last_printed_variable_name; }
    
    uint32_t &
    current_frame_id()
    { return m_current_frame_id; }
    
    bool &
    vm_has_suspended()
    { return m_vm_has_suspended; }
  };
}

#endif
