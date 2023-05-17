#ifndef RUN_SIMULATION_H
#define RUN_SIMULATION_H

#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <random>
#include <algorithm> // for for_each
#include <fstream>
#include "Process.h"
#include "utility.h"

#define RAND_UNICAST 1

using namespace std;

extern int number_of_processes; // number of processes/nodes

extern int delta;   // minimum message delay
extern int epsilon; // uncertainty windows
extern int delta_max;
extern double alpha; // message rate

extern int run_up_to;     // total number of physical clock cycles in simulation
extern int absolute_time; // index of physical clock cycles

extern probability_matrix prob_matrix;

/*
distance_matrix D;
string topology_file_name;
string prob_matrix_file_name;
*/

class RunSimulation
{
public:
  /*
   * initialize simulation paramters, i.e., global variables, from input file
   */
  static void InitAllConfiguration(const string &parameter_file_name);

  /*
   *initialize all processes with ID starting from 0
   */
  static vector<Process> InitProcess();
  /*
   * Main simulation: run random unicast model for a long time
   * , e.g., 'run_up_to' = 100000, then measure the average HVC size
   * for 'absolute_time' between 'epsilon' and 'run_up_to'.
   * Repeat this run for each value of 'epsilon' up to 'run_up_to'
   */
  static void RandomUnicastExperiment(const string &s);
  /*
   * Constrained time ('epsilon' < infinity) vs. unconstrained time model ('epsilon' * -> infinity):
   * run random unicast model while measuring the average HVC size
   * for each 'absolute_time' given 'epsilon'.
   */
  static void RandomUnicastExperimentSnapshot(const string &s);

  static void RandomUnicastExperimentWithNormalDistribution(const string &s, double percent_std);

private:
  static double run();
};

#endif
