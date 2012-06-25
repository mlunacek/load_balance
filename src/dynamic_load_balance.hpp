#ifndef dynamic_load_balance_hpp
#define dynamic_load_balance_hpp

#include "load_balance.hpp"

enum message_tags {msg_data_packet, msg_finished, msg_rank_ready};

class dynamic_load_balance : public load_balance
{
  public:
	  
	dynamic_load_balance(const std::string &, mpi::communicator &);
	~dynamic_load_balance();

	virtual void execute();
		
  private:
	  
	void master();
	void worker();  
	  
};

#endif
