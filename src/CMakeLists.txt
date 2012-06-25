include_directories ( ${MPI_INCLUDE_PATH} ${Boost_INCLUDE_DIR})

set(load_balance_SOURCES
	load_balance.cpp
	load_balance.hpp
	dynamic_load_balance.cpp
	dynamic_load_balance.hpp
	static_load_balance.cpp
	static_load_balance.hpp
)

add_executable( load_balance driver.cpp ${load_balance_SOURCES})

target_link_libraries(  
    load_balance
	${MPI_LIBRARIES}
	${Boost_LIBRARIES}
)


