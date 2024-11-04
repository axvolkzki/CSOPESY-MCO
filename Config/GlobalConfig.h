#pragma once
#include "../TypedefRepo.h"
#include <cstdint>

class GlobalConfig
{
public:
	GlobalConfig();
	~GlobalConfig();

	bool loadConfigFile(String& filename);				// Loads the config file
	void printConfig() const;							// Prints the config
	int getRandomInstructionCount() const;				// Returns a random instruction count

	// Getters
	uint16_t getNumCPU() const;							// Returns the number of CPUs
	String getScheduler() const;						// Returns the scheduler
	uint32_t getQuantumCycles() const;					// Returns the quantum cycles
	uint32_t getBatchProcessFreq() const;				// Returns the batch process frequency
	uint32_t getMinIns() const;							// Returns the minimum instructions
	uint32_t getMaxIns() const;							// Returns the maximum instructions
	uint32_t getDelaysPerExec() const;					// Returns the delays per execution

private:
	struct Config {							// Struct variable to hold the values of the config.txt
		uint16_t num_cpu;					// Number of CPUs: [1, 128]
		String scheduler;					// or enum SchedulerType { FCFS, RR };
		uint32_t quantum_cycles;			// For Round Robin: [1, 2^32]
		uint32_t batch_process_freq;		// Frequency of batch process: [1, 2^32]
		uint32_t min_ins;					// Minimum instructions: [1, 2^32]
		uint32_t max_ins;					// Maximum instructions: [1, 2^32]
		uint32_t delays_per_exec;			// Delays per execution: [0, 2^32]
	};

	Config config;										// Config variable
	bool parseConfigFile(String& line);					// Parses the config file
};

