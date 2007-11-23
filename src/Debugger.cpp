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

#include <getopt.h>
#include <cassert>
#include <errno.h>

#include <sstream>

#include "Debugger.hpp"
#include "ClientSocket.hpp"
#include "Exception.hpp"
#include "SrcFiles.hpp"

#include "DbgCmd/DbgCmd_Top.hpp"

#include "Kdwp/KdwpCmd_Event_Composite.hpp"
#include "Kdwp/KdwpCmd_EventRequest_Clear.hpp"
#include "Kdwp/KdwpCmd_KVM_HandShake.hpp"
#include "Kdwp/KdwpCmd_Event_Composite.hpp"
#include "Kdwp/KdwpCmd_ReferenceType_SourceFile.hpp"
#include "Kdwp/KdwpCmd_ReferenceType_Signature.hpp"
#include "Kdwp/KdwpCmd_ReferenceType_Methods.hpp"
#include "Kdwp/KdwpCmd_Method_LineTable.hpp"
#include "Kdwp/KdwpCmd_VM_Suspend.hpp"
#include "Kdwp/KdwpCmd_VM_Resume.hpp"
#include "Kdwp/KdwpCmd_VM_AllThreads.hpp"
#include "Kdwp/KdwpCmd_ThreadReference_Status.hpp"
#include "Kdwp/Constant.hpp"

struct option long_options[] = 
{
  {"host", required_argument, 0, 'o'},
  {"port", required_argument, 0, 'p'},
  {"timeout", required_argument, 0, 't'},
  {"emacs", no_argument, 0, 'e'},
  {"srcpath", required_argument, 0, 's'},
  {"help", no_argument, 0, 'h'},
  {0, 0, 0, 0}
};

namespace jmdb
{
  //////////////////////////////// Private ////////////////////////////////////
  //============================= Operation ===================================
  
  void
  Debugger::usage()
  {
    std::cout << gettext("Usage: jmdb [options]...")
              << std::endl
              << gettext("A Java source level debugger for J2ME")
              << std::endl << std::endl
              << gettext("  -e, --emacs     using emacs mode")
              << std::endl
              << gettext("  -o, --host      hostname (default = localhost)")
              << std::endl
              << gettext("  -h, --help      print this usage")
              << std::endl
              << gettext("  -p, --port      port (default = 8000)")
              << std::endl
              << gettext("  -s, --srcpath   the root search paths to find the .java source files.")
              << std::endl
              << gettext("                  if specify more than one search path, they have to be separated by colon.")
              << std::endl
              << gettext("  -t, --timeout   timeout (seconds, default = 5)")
              << std::endl << std::endl
              << gettext("Author: Wei Hu <wei.hu.tw@gmail.com>")
              << std::endl;
  }
  
  void
  Debugger::prompt_usr() const
  {
    std::cout << "(jmdb) ";
  }
  
  void
  Debugger::set_terminal_raw()
  {
    if (-1 == tcgetattr(STDIN_FILENO, &m_orig_termios))
    {
      assert(1 != 1);
    }
    
    m_new_termios = m_orig_termios;
    
    // ~ICANON: Put jmdb into non-canonical mode.
    // ~ECHO: Disable echo
    m_new_termios.c_lflag &= ~(ICANON | ECHO);
    
    m_new_termios.c_cc[VMIN] = 1; // minimum chars to wait fo
    
    if (-1 == tcsetattr(STDIN_FILENO, TCSANOW, &m_new_termios))
    {
      assert(1 != 1);
    }
  }
  
  void
  Debugger::restore_terminal()
  {
    if (-1 == tcsetattr(0, TCSANOW, &m_orig_termios))
    {
      assert(1 != 1);
    }
  }
  
  Debugger::KeyEnum
  Debugger::getch()
  {
    KeyEnum c;
    Debugger::KeyMode mode = NORMAL_MODE;
    
    while (1)
    {
      set_terminal_raw();
      
      // Extract unformatted data from stream
      c = static_cast<KeyEnum>(std::cin.get());
      
      restore_terminal();
      
      switch (mode)
      {
      case NORMAL_MODE:
        switch (c)
        {
        case KEY_ESC:
          mode = ESC_MODE;
          break;
          
        case KEY_CR:
        case KEY_LF:
        case KEY_BS:
        case KEY_DEL:
        case KEY_SPACE ... KEY_TILDE: // Printable characters
          return c;
          
        default:
          std::cout << "c = " << std::hex << c << std::endl;
          assert(1 != 1);
        }
        break;
        
      case ESC_MODE:
        switch (c)
        {
        case KEY_LEFT_BRACKET:
          mode = BRACKET_MODE;
          break;
          
        default:
          std::cout << "c = " << std::hex << c << std::endl;
          assert(1 != 1);
        }
        break;
        
      case BRACKET_MODE:
        switch (c)
        {
        case KEY_A: return KEY_UP;
        case KEY_B: return KEY_DOWN;
        case KEY_C: return KEY_RIGHT;
        case KEY_D: return KEY_LEFT;
          
        default:
          std::cout << "c = " << std::hex << c << std::endl;
          assert(1 != 1);
        }
        break;
      }
    }
  }
  
  int32_t
  Debugger::get_line_number(uint32_t const class_id,
                            uint32_t const method_id,
                            uint32_t const offset) const
  {
    KdwpCmd_Method_LineTable kdwp_cmd(class_id, method_id);
    
    m_socket << kdwp_cmd;
    
    return kdwp_cmd.get_line_number(offset);
  }
  
  bool
  Debugger::wait_event()
  {
    fd_set rset;
    
    FD_ZERO(&rset);
    FD_SET(m_socket.socket_fd(), &rset);
    
    int const result = select(m_socket.socket_fd() + 1, &rset, 0, 0, 0);
    
    assert(result != 0);
    
    if (-1 == result)
    {
      switch (errno)
      {
      case EINTR:
        {
          KdwpCmd_VM_Suspend suspend_cmd;
          
          m_socket << suspend_cmd;
          
          m_vm_has_suspended = true;
        }
        return false;
        
      case 514: // This is the value of ERESTARTNOHAND
        // :NOTE: Wei 2004-Jun-22:
        //
        // If I use gdb to debug jmdb, and I use 'signal SIGINT' to
        // interrupt jmdb, then jmdb the upper select() will return -1
        // and set errno to 'ERESTARTNOHAND'.
        //
        // I don't know whether this is a bug or not.
        // However, in order to debug jmdb with gdb,
        // I have to handle this situation.
        {
          KdwpCmd_VM_Suspend suspend_cmd;
          
          m_socket << suspend_cmd;
          
          m_vm_has_suspended = true;
        }
        return false;
        
      default:
        std::cerr << "unknow errno: " << errno << std::endl;
        return false;
      }
    }
    else
    {
      if (FD_ISSET(m_socket.socket_fd(), &rset))
      {
        KdwpCmd_Event_Composite * const p_event = new KdwpCmd_Event_Composite();
        
        m_socket >> (*p_event);
        
        p_event->dump_to_console();
        
        mp_last_event.reset(p_event);
        
        return true;
      }
      else
      {
        assert(1 != 1);
        return false;
      }
    }
  }
  
  void
  Debugger::handle_event()
  {
    std::vector<KdwpCmd_Event_Composite::EventBase const *> &events = mp_last_event->events();
    
    // reset the current frame id to 0.
    m_current_frame_id = 0;
    
    for (std::vector<KdwpCmd_Event_Composite::EventBase const *>::iterator iter = events.begin();
         iter != events.end();
         ++iter)
    {
      switch ((*iter)->m_event_kind)
      {
      case EVENT_SINGLE_STEP:
        {
          KdwpCmd_Event_Composite::SingleStep const *p_single_step_event =
            static_cast<KdwpCmd_Event_Composite::SingleStep const *>(*iter);
          
          m_last_event_thread_id = p_single_step_event->m_thread_id;
          m_last_event_class_id = p_single_step_event->m_location.m_class_id;
          m_last_event_method_id = p_single_step_event->m_location.m_method_id;
          
          handle_filename_and_line_number(p_single_step_event->m_location);
          
          // clear this single step event.
          KdwpCmd_EventRequest_Clear clear_cmd(EVENT_SINGLE_STEP, p_single_step_event->m_req_id);
          
          m_socket << clear_cmd;
          
          // all threads in the vm is suspeneded when jmdb receives single step event.
          m_vm_has_suspended = true;
        }
        break;
        
      case EVENT_BREAKPOINT:
        {
          KdwpCmd_Event_Composite::BreakPoint const *p_breakpoint_event =
            static_cast<KdwpCmd_Event_Composite::BreakPoint const *>(*iter);
          
          m_last_event_thread_id = p_breakpoint_event->m_thread_id;
          m_last_event_class_id = p_breakpoint_event->m_location.m_class_id;
          m_last_event_method_id = p_breakpoint_event->m_location.m_method_id;
          
          handle_filename_and_line_number(p_breakpoint_event->m_location);
          
          // all threads in the vm is suspeneded when jmdb receives breakpoint event.
          m_vm_has_suspended = true;
        }
        break;
        
      case EVENT_VM_START:
      case EVENT_METHOD_ENTRY:
      case EVENT_METHOD_EXIT:
      case EVENT_EXCEPTION:
      case EVENT_THREAD_START:
      case EVENT_THREAD_END:
      case EVENT_CLASS_PREPARE:
      case EVENT_FIELD_ACCESS:
      case EVENT_FIELD_MODIFICATION:
      case EVENT_CLASS_UNLOAD:
        break;
        
      default:
        assert(1 != 1);
        break;
      }
    }
  }
  
  //////////////////////////////// Public /////////////////////////////////////
  //============================ Life cycle ===================================
  
  Debugger::Debugger()
    : mp_dbgcmd_top(new DbgCmd_Top(this)),
      m_quit_flag(false),
      m_current_frame_id(0),
      mp_src_files(new SrcFiles(this)),
      m_vm_has_suspended(false),
      m_emacs_mode(false),
      m_wait_event(false)
  {
    std::cout << gettext("Java 2 Micro Edition Debugger (jmdb)") << std::endl;
    std::cout << gettext("Copyright 2007 Wei Hu <wei.hu.tw@gmail.com>") << std::endl;
    std::cout << gettext("You can type \"help\" to learn how to use jmdb.") << std::endl;
    std::cout << gettext("There is absolutely no warranty for jmdb.") << std::endl;
    std::cout << std::endl;
  }
  
  Debugger::~Debugger()
  { }
  
  //============================ Operation ====================================
  
  void
  Debugger::init(int argc, char **argv)
  {
    std::stringstream ss;
    int c;
    int option_index;
    
    while (1)
    {
      c = getopt_long(argc, argv, "h:p:t:", long_options, &option_index);
      
      if (-1 == c)
      {
        break;
      }
      
      switch (c)
      {
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
        break;
        
      case 'o':
        {
          std::string host;
          
          ss << optarg;
          ss >> host;
          
          m_socket.set_host(host);
          
          ss.clear();          ss.str("");
        }
        break;
        
      case 'p':
        {
          uint32_t port;
          
          ss << optarg;
          ss >> port;
          
          m_socket.set_port(port);
          
          ss.clear();
          ss.str("");
        }
        break;
        
      case 's':
        mp_src_files->set_src_search_path(optarg);
        break;
        
      case 't':
        {
          uint32_t timeout;
          
          ss << optarg;
          ss >> timeout;
          
          m_socket.set_timeout(timeout);
          
          ss.clear();
          ss.str("");
        }
        break;
        
      case 'e':
        m_emacs_mode = true;
        break;
        
      case '?':
        // getopt_long already printed an error message.
        std::cout << std::endl;
        usage();
        exit(EXIT_FAILURE);
        break;
      }
    }
    
    if (argc != optind)
    {
      std::cout << argv[0] << ": invalid command line argument -- " << argv[optind] << std::endl << std::endl;
      usage();
      exit(EXIT_FAILURE);
    }
    
    m_socket.connect();
    
    // Preform handshake
    {
      KdwpCmd_KVM_HandShake handshake;
      
      m_socket << handshake;
    }
    
    // Wait VMStart event
    {
      KdwpCmd_Event_Composite event;
      
      m_socket >> event;
      
      event.dump_to_console();
    }
  }
  
  void
  Debugger::run()
  {
    std::string usr_input;
    KeyEnum c;
    bool end_command;
    
    // :NOTE: Wei 2004-Jun-22:
    //
    // If not all of the VM threads are suspended,
    // then we need to suspend VM before we can issue some command.
    // This facility can be done through the use of wait_event().
    if (false == is_all_thread_suspended())
    {
      wait_event();
    }
    
    try
    {
      while (1)
      {
        prompt_usr();
        
        end_command = false;
        
        while (false == end_command)
        {
          c = getch();
          
          switch (c)
          {
          case KEY_CR:
          case KEY_LF:
            // create a new line.
            std::cout << std::endl;
            
            end_command = true;
            
            if (true == usr_input.empty())
            {
              usr_input = m_history.get_recent_cmd();
            }
            
            m_history.push_back(usr_input);
            break;
            
          case KEY_UP:
            for (uint32_t i = 0; i < usr_input.size(); ++i)
            {
              delete_one_console_char();
            }
            
            usr_input = m_history.get_recent_cmd();
            std::cout << usr_input;
            break;
            
          case KEY_DOWN:
            for (uint32_t i = 0; i < usr_input.size(); ++i)
            {
              delete_one_console_char();
            }
            
            usr_input = m_history.put_recent_cmd();
            std::cout << usr_input;
            break;
            
          case KEY_RIGHT:
            std::cout << static_cast<char>(KEY_ESC);
            std::cout << static_cast<char>(KEY_LEFT_BRACKET);
            std::cout << static_cast<char>(KEY_C);
            break;
            
          case KEY_LEFT:
            std::cout << static_cast<char>(KEY_ESC);
            std::cout << static_cast<char>(KEY_LEFT_BRACKET);
            std::cout << static_cast<char>(KEY_D);
            break;
            
          case KEY_BS:
          case KEY_DEL:
            if (usr_input.empty() != true)
            {
              delete_one_console_char();
              
              usr_input.erase(--usr_input.end());
            }
            break;
            
          default:
            // Echo normal characters.
            std::cout << static_cast<char>(c);
            
            usr_input.insert(usr_input.end(), c);
            break;
          }
        }
        
        if (usr_input.empty() != true)
        {
          mp_dbgcmd_top->do_command(usr_input);
          
          assert(true == usr_input.empty());
          
          if (true == m_quit_flag)
          {
            // check to see if we suspend VM now.
            // If we suspend VM now, then we have to resume VM before
            // we quit jmdb.
            
            if (true == m_vm_has_suspended)
            {
              KdwpCmd_VM_Resume resume_cmd;
              
              m_socket << resume_cmd;
              
              // :NOTE: Wei 2004-Jun-22:
              //
              // The following setting is unnecessary.
              // I set it becuase of the consistency.
              m_vm_has_suspended = false;
            }
            
            break;
          }
          
          if (true == m_wait_event)
          {
            if (true == wait_event())
            {
              handle_event();
            }
            
            m_wait_event = false;
          }
        }
      }
    }
    catch (SocketException &e)
    {
      std::cerr << e.description() << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  
  void
  Debugger::handle_filename_and_line_number(Location const &location)
  {
    // get file name
    KdwpCmd_ReferenceType_SourceFile source_cmd(location.m_class_id);
    
    m_socket << source_cmd;
    
    // get base name
    KdwpCmd_ReferenceType_Signature sig_cmd(location.m_class_id);
    
    m_socket << sig_cmd;
    
    std::string const full_name = mp_src_files->search_src_file(sig_cmd.signature(), source_cmd.source_file());
    
    if (false == full_name.empty())
    {
      // we find the corresponding source file.
      
      int32_t const line_number = get_line_number(location.m_class_id,
                                                  location.m_method_id,
                                                  location.m_offset);
      
      if (-1 == line_number)
      {
        KdwpCmd_ReferenceType_Methods method_cmd(location.m_class_id);
        
        m_socket << method_cmd;
        
        std::cout << gettext("Can't find the line number of ")
                  << method_cmd.get_method_name(location.m_method_id) << std::endl;
      }
      else
      {
        if (true == m_emacs_mode)
        {
          std::cout << "" << full_name << ":" << line_number << std::endl;
        }
      }
    }
  }
  
  bool
  Debugger::is_all_thread_suspended() const
  {
    // check to see whether we have to prompt the user at beginning or not.
    KdwpCmd_VM_AllThreads all_threads_cmd;
    
    m_socket << all_threads_cmd;
    
    for (std::vector<uint32_t>::const_iterator iter = all_threads_cmd.thread_id().begin();
         iter != all_threads_cmd.thread_id().end();
         ++iter)
    {
      KdwpCmd_ThreadReference_Status status_cmd(*iter);
      
      m_socket << status_cmd;
      
      if (status_cmd.suspend_status() != SUSPENDED)
      {
        return false;
      }
    }
    
    return true;
  }
}
