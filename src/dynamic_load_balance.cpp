#include "dynamic_load_balance.hpp"


dynamic_load_balance::dynamic_load_balance(const std::string &filename, 
											mpi::communicator &comm):
	 				  						load_balance(filename, comm)
{

}

dynamic_load_balance::~dynamic_load_balance()
{

}

void dynamic_load_balance::execute()
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

void dynamic_load_balance::master()
{
	// Give them another when ready...
	bool ready;
	int recv_count = 0;
	for(int index=0; index < m_jobs.size(); ++index)
	{
		mpi::status msg = m_comm.probe();
		if (msg.tag() == msg_rank_ready)
		{
			recv_count++;
			m_comm.recv(msg.source(), msg.tag(), ready);
			m_comm.send(msg.source(), msg_data_packet, m_jobs[index]);
		}
	}
			
	//if the number of jobs is less than processor counts..
	for( recv_count; recv_count< m_comm.size(); ++recv_count)
	{
		mpi::status msg = m_comm.probe();
		if (msg.tag() == msg_rank_ready)
			m_comm.recv(msg.source(), msg.tag(), ready);	
	}
				
	// Tell the workers we are finished
	bool done = true;
	for(int r=1; r<m_comm.size(); ++r)
		m_comm.send(r, msg_finished, done);

}

void dynamic_load_balance::worker()
{
	// Ready for work...
	bool ready = true;
  	m_comm.send(0, msg_rank_ready, ready);
	
	bool done = false;
	while( !done)
	{
		mpi::status msg = m_comm.probe();
		if (msg.tag() == msg_data_packet) 
		{
		  // Receive the packet of data
		  std::string cmd;
		  m_comm.recv(msg.source(), msg.tag(), cmd);
		  //std::cout << cmd << std::endl;
		  system(cmd.c_str());
		  
		  // Ready
		  m_comm.send(0, msg_rank_ready, ready);
	  	}
		else if( msg.tag() == msg_finished )
		{
	  	  m_comm.recv(msg.source(), msg.tag(), done);
	  	}
	}
}


