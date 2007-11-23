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

#include <iostream>
#include <sys/stat.h>
#include <errno.h>

#include "SrcFiles.hpp"
#include "Exception.hpp"
#include "Debugger.hpp"

namespace jmdb
{
  /////////////////////////////////// Private /////////////////////////////////
  //================================ Operation ================================
  
  SrcFiles::FileState
  SrcFiles::file_state(std::string const &root_name) const
  {
    struct stat root_stat;
    
    int const result = stat(root_name.c_str(), &root_stat);
    
    if (0 == result)
    {
      if (S_ISDIR(root_stat.st_mode) != 0)
      {
        return FILE_IS_DIR;
      }
      
      return FILE_NORMAL;
    }
    else
    {
      // result == -1
      switch (errno)
      {
      case ENOENT:
      case ENOTDIR:
        return FILE_NON_EXIST;
        
      default:
        // :NOTE: Wei 2004-Mar-18:
        //
        // If we go here, means some other errors occurs.
        assert(1 != 1);
        return FILE_NON_EXIST;
      }
    }
  }
  
  std::string
  SrcFiles::get_basename_from_signature(std::string const &signature)
  {
    assert(signature.empty() != true);
    assert('L' == signature[0]);
    assert(';' == *(--signature.end()));
    
    if (signature.find_last_of('/') != std::string::npos)
    {
      // this class has a package name.
      return signature.substr(1, signature.find_last_of('/'));
    }
    else
    {
      // this class doesn't have a package name.
      return std::string();
    }
  }
  
  /////////////////////////////////// Public //////////////////////////////////
  //================================ Life cycle ===============================
  
  SrcFiles::~SrcFiles()
  {
    for (std::deque<FileInfo>::iterator iter = m_files.begin();
         iter != m_files.end();
         ++iter)
    {
      delete iter->mp_filestream;
    }
  }
  
  //================================= Operation ===============================
  
  void
  SrcFiles::set_src_search_path(std::string const &search_path)
  {
    std::string temp = search_path;
    
    while (temp.empty() != true)
    {
      std::string::size_type colon_pos = temp.find_first_of(':');
      std::string path = temp.substr(0, colon_pos);
      
      switch (file_state(path))
      {
      case FILE_IS_DIR:
        m_src_search_path.push_back(path);
        break;
        
      case FILE_NORMAL:
        std::cout << "WARNING: " << path << " is not a directory." << std::endl;
        break;
        
      case FILE_NON_EXIST:
        std::cout << "WARNING: " << path << " doesn't exist." << std::endl;
        break;
        
      default:
        assert(1 != 1);
        break;
      }
      
      if (colon_pos != std::string::npos)
      {
        temp.erase(0, colon_pos + 1);
      }
      else
      {
        // :NOTE: Wei 2004-Jun-19:
        //
        // This should be the last one.
        break;
      }
    }
  }
  
  std::string
  SrcFiles::search_src_file(std::string const &signature,
                            std::string const &filename)
  {
    std::string const base_name = get_basename_from_signature(signature);
    std::string const temp = base_name + filename;
    
    if (0 == m_src_search_path.size())
    {
      std::cout << "WARNING: You don't specify any source file search path. " << std::endl;
      
      return std::string();
    }
    else
    {
      for (std::vector<std::string>::iterator iter = m_src_search_path.begin();
           iter != m_src_search_path.end();
           ++iter)
      {
        std::string full_name = (*iter) + temp;
        
        switch (file_state(full_name))
        {
        case FILE_NORMAL:
          {
            std::deque<FileInfo>::iterator iter;
            
            for (iter = m_files.begin(); iter != m_files.end(); ++iter)
            {
              if (full_name == iter->m_fullname)
              {
                break;
              }
            }
            
            // record the information of this new source file.
            if (iter == m_files.end())
            {
              FileInfo fi;
              
              fi.m_fullname = full_name;
              
              if (false == mp_debugger->emacs_mode())
              {
                // If we run jmdb in the console, then we need to open the source file
                // by ourself.
                // Because jmdb have to show the source file manually.
                
                fi.mp_filestream = new std::ifstream(fi.m_fullname.c_str());
              }
              
              m_files.push_back(fi);
            }
          }
          return full_name;
          
        case FILE_IS_DIR:
        case FILE_NON_EXIST:
          break;
          
        default:
          assert(1 != 1);
          break;
        }
      }
      
      std::cout << "WARNING: I can't find the source file for " << filename << std::endl;
      return std::string();
    }
  }
}
