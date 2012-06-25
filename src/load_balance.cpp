#include <iostream>
#include <fstream>


#include "load_balance.hpp"
#include "dynamic_load_balance.hpp"
#include "static_load_balance.hpp"


load_balance * load_balance::factory(const std::string &filename, 
									 mpi::communicator &comm,
									 const std::string &schedule)
{
	if( schedule == "dynamic")
		return new dynamic_load_balance(filename,comm);
	
	if( schedule == "static")
		return new static_load_balance(filename,comm);
	
	return NULL;
}

load_balance::load_balance(const std::string &filename, mpi::communicator &comm)
{
	m_comm = comm;
	m_filename = filename;
	
	// Populates the m_jobs vector of commands.
	read_jobs();
	
}

load_balance::~load_balance()
{

}

void load_balance::read_jobs()
{
	std::string line;
	std::ifstream cmd_lines(m_filename.c_str());
	if (cmd_lines.is_open())
	{
	    while ( cmd_lines.good() )
	    {
	         getline (cmd_lines,line);
			 if( line.length() > 1)
				 m_jobs.push_back(line);
	    }
		cmd_lines.close();
	}
}

void load_balance::serial()
{
	for(int i=0; i<m_jobs.size(); ++i)
	{
		std::string cmd = m_jobs[i];
  		system(cmd.c_str());
	}
}



