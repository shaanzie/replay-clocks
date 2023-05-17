/*
  utility.h - class Utility
              for utility functionality
*/

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream> // for cout
#include <vector>

#define _DEBUG

using namespace std;

typedef vector<vector<int>> adjacency_matrix;
typedef vector<vector<int>> distance_matrix;
typedef vector<vector<double>> probability_matrix;

extern int epsilon;
extern int number_of_processes;
extern int delta;
extern double alpha;
extern int run_up_to;
extern string prob_matrix_file_name;
extern string topology_file_name;

/*
  Utility - warpper class for utility function
*/

class Utility
{
public:
  static int GetRandomNumberInRange(int, int);
  double Variance(const vector<double> &vi);
  static vector<vector<double>> Transpose(const vector<vector<double>> &m_in);
  static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
  static std::vector<std::string> split(const std::string &s, char delim);
  static distance_matrix AllPairShortestPaths(const adjacency_matrix &G);
  static adjacency_matrix ReadGraph(const string &topo);
  static probability_matrix ReadProbabilityMatrix(const string &prob);

  // default param can be declared only once. It should be declared in header file
  // thus in utility.cpp, should not provide default value for param size of function
  // GetIndexOfWeightedRandom(). If we provide, it cause compile error or unpredictable runtime
  // http://www.learncpp.com/cpp-tutorial/77-default-parameters/
  static int GetIndexOfWeightedRandom(const vector<double> &arr, int size = number_of_processes);

  static int GetIndexOfWeightedNormalDistribution(double std_percent, int size);

  // CREDITS: http://stackoverflow.com/questions/236129/split-a-string-in-c
  static std::vector<std::string> &split_(const std::string &s, char delim, std::vector<std::string> &elems);
  static std::vector<std::string> splitstring(const std::string &s, char delim);
};
#endif
