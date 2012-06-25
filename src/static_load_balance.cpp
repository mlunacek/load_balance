#include "static_load_balance.hpp"

#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/mpi/packed_oarchive.hpp>

#include <vector>
#include <string>

static_load_balance::static_load_balance(const std::string &filename, 
											mpi::communicator &comm):
	 				  						load_balance(filename, comm)
{

}

static_load_balance::~static_load_balance()
{

}

void static_load_balance::execute()
{
	if( m_comm.rank() == 0 )
	{
		if( m_comm.size() == 1)
			serial();
		else	
			master();
	}
	else
		worker();
}

void static_load_balance::master()
{
	double total_jobs = m_jobs.size();
	double total_workers = m_comm.size();
	
	int low_load = floor(total_jobs/total_workers);
	int high_load = ceil(total_jobs/total_workers);
	int high_workers = total_workers;
	int low_workers = 0.0;
		
	if( m_jobs.size()%m_comm.size() != 0)
	{
		high_workers = (total_jobs - low_load*total_workers)/(high_load - low_load);
		low_workers = total_workers - high_workers;
	}
	
	// Create an array containing the number of jobs
	std::vector<int> number_of_jobs;
	for(int r=0; r<m_comm.size(); ++r)
	{
		if( r < high_workers)
			number_of_jobs.push_back(high_load);
		else
			number_of_jobs.push_back(low_load);
	}
	
	std::vector<std::string>::iterator it = m_jobs.begin() + number_of_jobs[0];
	for(int r=1; r<m_comm.size(); ++r)
	{
		std::vector<std::string> tmp;  	
		std::copy(it, it + number_of_jobs[r], std::back_inserter(tmp));
		m_comm.send(r,10,tmp);
		it+=number_of_jobs[r];
	}
		
	//TODO Master does less work if possible	
	for(int i=0;i<number_of_jobs[0]; ++i)
		system(m_jobs[i].c_str());

}

void static_load_balance::worker()
{
	m_comm.recv(0,10,m_jobs);
		
	for(int i=0;i<m_jobs.size(); ++i)
		system(m_jobs[i].c_str());
}

