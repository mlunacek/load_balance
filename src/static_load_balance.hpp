#ifndef static_load_balance_hpp
#define static_load_balance_hpp

#include "load_balance.hpp"


class static_load_balance : public load_balance
{
  public:
	  
	static_load_balance(const std::string &, mpi::communicator &);
	~static_load_balance();

	virtual void execute();
		
  private:
	  
	void master();
	void worker();  
	
	
	
	  
};

#endif
