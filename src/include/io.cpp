#ifndef __GUARD_IO__
#define __GUARD_IO__

namespace cnsd {

  namespace debug {
    bool __LOG = false;

    void log(std::string msg, bool same_line = false)
    {
      if(__LOG)
      {
        std::cout << msg;
        if(!same_line)
          std::cout << std::endl;
      }
      return ;
    }

    void enable_log(bool value)
    {
      __LOG = value;
      return ;
    }

    bool log_enabled() { return __LOG; }

    }
}

#endif
