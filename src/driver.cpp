#include <iostream>
#include <mpi.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
namespace mpi = boost::mpi;

#include "load_balance.hpp"


enum message_tags {msg_data_packet, msg_finished, msg_rank_ready};


int main(int argc, char ** argv)
{
	//Initialize BOOST MPI
	mpi::environment env(argc, argv);
	mpi::communicator world;
	
	// Command line parameters
	std::string filename;
	std::string schedule;
	try 
	{
		po::options_description config("Configuration");
		config.add_options()
			("help,h", "")
			("filename,f", po::value(&filename), 
				"Filename containing command lines for load balancing. Each command line should be on a single line in the file.")
			("schedule,s", po::value(&schedule)->default_value("static"), 
				"Type of schedule: dynamic or static. The default schedule is static, which is best for jobs that are similar in runtime.  Consider using 'dynamic' if there exist a variance in job runtime.")
			;
	    
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, config), vm);
		po::notify(vm);

		// Print help information
		if (vm.count("help")) 
		{  
			if( world.rank() == 0 )
				std::cout << std::endl<< config << std::endl;
			return 0;
		}
		// Make sure there's a file
		if (vm.count("filename") == 0)
		{
			if( world.rank() == 0 )
			{
				std::cout << "\nPlease specify a filename\n" << std::endl;
				std::cout << config << std::endl;		
			}
			return 0;
		}
	}
    catch( std::exception& e )
    {
	   if( world.rank() == 0 )
       		std::cout << e.what() << "\n";
       return 1;
    }    
	
	// Call the load balance object
	load_balance * lb = load_balance::factory(filename, world, schedule);
	lb->execute();
	delete lb;	
			
	
	return 0;
}













