/*
* (C) 2009,2014,2015 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#include "cli.h"

#include <botan/version.h>
#include <botan/internal/stl_util.h>
#include <iterator>
#include <sstream>

namespace {
/**
* This method is responsible for ensuring the proper usage of the botan commands,
* This method is called when there are either less than enough arguments, "help",
* --help, or -h.
*/
std::string main_help()
   {
   const std::set<std::string> avail_commands =
      Botan::map_keys_as_set(Botan_CLI::Command::global_registry());

   std::ostringstream oss;

   oss << "Usage: botan <cmd> <cmd-options>\n";
   oss << "Available commands:\n";

   for(auto& cmd_name : avail_commands)
      {
      auto cmd = Botan_CLI::Command::get_cmd(cmd_name);
      oss << cmd->cmd_spec() << "\n";
      }

   return oss.str();
   }

}

int main(int argc, char* argv[])
   {
	   /**
	   * This method is the first thing run every time,
	   * the method takes the version numbers and 
	   * compares them to the current version numbers.
	   * Should there be any conflict then the program will
	   * cease.
	   */
   std::cerr << Botan::runtime_version_check(BOTAN_VERSION_MAJOR,
                                             BOTAN_VERSION_MINOR,
                                             BOTAN_VERSION_PATCH);

   const std::string cmd_name = (argc <= 1) ? "help" : argv[1];

   //Here the program tests  
   if(cmd_name == "help" || cmd_name == "--help" || cmd_name == "-h")
      {
      std::cout << main_help();
      return 1;
      }

	  //The program tests to see whether the specified commands are accurate and supported.
   std::unique_ptr<Botan_CLI::Command> cmd(Botan_CLI::Command::get_cmd(cmd_name));

   //if an incorrect command is given then the program ends here
   if(!cmd)
      {
      std::cout << "Unknown command " << cmd_name << " (try --help)\n";
      return 1;
      }
//Thr program then executes the specified commands as specified.
   std::vector<std::string> args(argv + 2, argv + argc);
   return cmd->run(args);
   }
