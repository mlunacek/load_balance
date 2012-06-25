#ifndef load_balance_hpp
#define load_balance_hpp

#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
namespace mpi = boost::mpi;

class load_balance
{
  public:
	  
	load_balance(const std::string &, mpi::communicator &);
	~load_balance();

	virtual void execute() = 0;
	
	static load_balance * factory(const std::string &, 
								  mpi::communicator &,
								  const std::string &);
		
  private:
	  
	std::string m_filename;
	void read_jobs();
	
  protected:
	  
	std::vector<std::string> m_jobs;    	
	mpi::communicator m_comm;
	void serial();

};

#endif
