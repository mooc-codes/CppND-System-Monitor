#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay {



  void Display(System& system, int n = 10);
  void DisplaySystem(System& system, WINDOW* window);
  void DisplayProcesses(std::vector<Process>& processes, WINDOW* window, int n);
  std::string ProgressBar(float percent);

  void sig_handler(int sig);

};  // namespace NCursesDisplay

#endif