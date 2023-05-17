#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <random>
#include <algorithm> // for for_each
#include <iomanip>

#include "Process.h"
#include "utility.h"
#include "runsimulation.h"

using namespace std;

#define INF 1000000 // infinity

/*
  Define and assigned global variable in main.cpp
*/

int number_of_processes = 10; // number of processes/nodes

int delta = 1;	   // minimum message delay
int epsilon = 100; // uncertainty windows
int delta_max = delta;
double alpha = 0.05; // message rate

int run_up_to = 100000; // total number of physical clock cycles in simulation
int absolute_time;		// index of physical clock cycles

distance_matrix D;
probability_matrix prob_matrix;
string topology_file_name;
string prob_matrix_file_name;
string input_parameter_file;
string output_file;
int mode;
/* utility function for paring commandline argument */

enum optionCode
{
	PARAMETERS,
	OUTPUTFILE,
	MODE,
	UNDEFINED
};

optionCode getOptionCode(const string &optionString)
{
	if (optionString == "-p")
		return PARAMETERS;
	if (optionString == "-o")
		return OUTPUTFILE;
	if (optionString == "-m")
		return MODE;
	return UNDEFINED;
}

vector<string> splitArg(const string &s)
{
	vector<string> result;
	string item1, item2;

	item1 = s.substr(0, 2);
	result.push_back(item1);
	item2 = s.substr(3, string::npos);
	result.push_back(item2);

	// cout << "   item1 = " << item1 << "  item2 = " << item2 << endl;

	return result;
}

int parseArg(int argc, char *argv[])
{
	int i;
	vector<string> currentArg;

	for (i = 1; i < argc; i++)
	{
		// cout << "arg_" << i << " = " << argv[i] << endl;

		currentArg = splitArg(argv[i]);

		switch (getOptionCode(currentArg.at(0)))
		{
		case PARAMETERS:
			input_parameter_file = currentArg.at(1);
			cout.width(30);
			cout << "input_parameter_file = " << input_parameter_file << endl;
			break;
		case OUTPUTFILE:
			output_file = currentArg.at(1);
			cout.width(30);
			cout << "output_file = " << output_file << endl;
			break;
		case MODE:

			mode = atoi(currentArg.at(1).c_str());
			if (mode == 1)
			{
				cout << "running main simulation\n"
					 << endl;
			}
			else
			{
				// default run snapshot
				cout << "running simulation snapshot\n"
					 << endl;
			}

			break;
		default:
			cout << "ERROR: undefined option" << currentArg.at(0) << endl;
			exit(1);
		}
	}

	return 0;
}
void printHelp()
{
	printf("This program requires 3 parameters: -p=<parameter_file> -o=<output_file> -m=<1 or 2>\n");
	printf("--> mode 1  Main simulation: run random unicast model for a long time \n , e.g., 'run_up_to' = 10000, then measure the average HVC size \n for 'absolute_time' between 'epsilon' and 'run_up_to'. \n Repeat this run for each value of 'epsilon' up to 'run_up_to'\n");
	printf("--> mode 2  Constrained time ('epsilon' < infinity) vs. unconstrained time model ('epsilon' * -> infinity): \n run random unicast model while measuring the average HVC size \n for each 'absolute_time' given 'epsilon'.\n");
}
/* main function */

int main(int argc, char *argv[])
{

	// parsing commandline argument
	if (argc != 4)
	{
		printHelp();
		exit(0);
	}
	parseArg(argc, argv);

	RunSimulation::InitAllConfiguration(input_parameter_file);
	if (mode == 1)
	{
		RunSimulation::RandomUnicastExperiment(output_file);
	}
	else
	{
		RunSimulation::RandomUnicastExperimentSnapshot(output_file);
	}
	printf("DONE\n");
}
