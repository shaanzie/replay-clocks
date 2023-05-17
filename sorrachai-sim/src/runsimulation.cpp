/*
  runsimulation.cpp - implementation of runsimulation class
*/

#include "runsimulation.h"

/*
  InitiProcess() - initialize all processes with ID starting from 0
*/

vector<Process> RunSimulation::InitProcess(int )
{
	vector<Process> vp;
	for (int i = 0; i < number_of_processes; i++)
	{
		// Duong: here Process(i) is constructor
		Process new_process = Process(i);
		new_process.setInterval(interval);
		vp.push_back(Process(i));
	}
	return vp;
}

/*
  InitiAllConfiguration() - initialize simulation paramters from input file
							initialize global variables
*/

void RunSimulation::InitAllConfiguration(const string &parameter_file_name)
{
	int i;
	ifstream in(parameter_file_name); // param input file
	if (!in.is_open())
	{
		printf("Error: can't open parameter file\n");
		exit(1);
	}
	string content;
	while (getline(in, content))
	{
		vector<string> split_equalsign = Utility::splitstring(content, '=');
		for (string &s : split_equalsign)
		{
			cout << s << "  ";
		}
		printf("\n");
		string s0 = split_equalsign[0];
		string s1 = split_equalsign[1];
		if (s0 == "delta")
		{
			delta = atoi(s1.c_str());
		}
		else if (s0 == "epsilon")
		{
			epsilon = atoi(s1.c_str());
		}
		else if (s0 == "alpha")
		{
			alpha = atof(s1.c_str());
		}
		else if (s0 == "run_up_to")
		{
			run_up_to = atoi(s1.c_str());
		}
		else if (s0 == "number_of_processes")
		{
			number_of_processes = atoi(s1.c_str());
		}
		else if (s0 == "topology_file")
		{
			topology_file_name = s1;
		}
		else if (s0 == "probability_matrix_file")
		{
			prob_matrix_file_name = s1;
		}
		else
		{
			printf("else;\n");
		}
	}
	delta_max = delta;
	D = Utility::AllPairShortestPaths(Utility::ReadGraph(topology_file_name));
	prob_matrix = Utility::ReadProbabilityMatrix(prob_matrix_file_name);
}

double RunSimulation::run()
{
	vector<Process> vp = InitProcess();
	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++)
	{
		for_each(begin(vp), end(vp), [](Process &p)
				 {p.UnlockTime(); p.ReceiveMsg(); });
		for_each(begin(vp), end(vp), [&vp](Process &p)
				 { p.SendMsg(vp[Utility::GetIndexOfWeightedRandom(prob_matrix[p.GetId()])]); });
	}
	double num_events = 0;
	double sum_size_active_all = 0;
	for_each(begin(vp), end(vp), [&](const Process &p)
			 { num_events += p.GetNumEvents(); sum_size_active_all += p.GetSumSizeActive(); });
	return sum_size_active_all / (num_events * number_of_processes);
}

void RunSimulation::RandomUnicastExperiment(const string &s)
{
	ofstream out(s);
	for (int i = 1; i <= 1000; i++)
	{
		epsilon = i;
		double result = run();
		cout << i << " " << result << endl;
		out << result << endl;
	}
}

void RunSimulation::RandomUnicastExperimentSnapshot(const string &s)
{
	ofstream out(s);
	ofstream out_matrix("Matrix_" + s);

	vector<Process> vp = InitProcess();
	// program main loop here

	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++)
	{
		for_each(begin(vp), end(vp), [](Process &p)
				 {p.UnlockTime(); p.ReceiveMsg(); });
		for_each(begin(vp), end(vp), [&vp](Process &p)
				 { p.SendMsg(vp[Utility::GetIndexOfWeightedRandom(prob_matrix[p.GetId()])]); });
		double sum_size_active_all = 0;
		vector<double> current_size_distribution;
		for_each(begin(vp), end(vp), [&](Process &p)
				 {int  casn = p.CountActiveSizeNow();  sum_size_active_all += casn; out_matrix << casn / (float)number_of_processes << " "; });
		out_matrix << endl;
		out << sum_size_active_all / (vp.size() * vp.size()) << endl;
	}
}

/*
  // belows are code from Sorrachai that has not been used

double Variance(const vector<double>& vi) {
	double avg = accumulate(begin(vi), end(vi), 0.0) / vi.size();
	double ss = 0;
	for (auto x : vi) ss += (x - avg)*(x-avg);
	return ss / vi.size();
}

vector<vector<double>> Transpose(const vector<vector<double>>& m_in) {
	if (m_in.size() == 0) {
		printf("Warning Transpose empty matrix\n");
		return vector<vector<double>>();
	}
	int m = m_in[0].size();
	int n = m_in.size();
	vector<vector<double>> m_out(m, vector<double>(n, 0));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			m_out[j][i] = m_in[i][j];
	return m_out;
}


double inline RandomUnicastExperimentWithNormalDistribution(double percentstd) {
	vector<Process> vp = InitProcess();
	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++) {
	// Duong: what this line does? what does for_each do?
	// lambda expression
	// [var outside scope](param){statements}
	// [] means? nothing outside
	// [&] means? everythings outside
	// [&vp] means? just vp outside

//   for(auto & p: vp) {
//      p.UnlockTime();
//      p.ReceiveMsg();
//    }

		for_each(begin(vp), end(vp), [](Process& p) {p.UnlockTime(); p.ReceiveMsg(); });


//   for(auto & p: vp) {
//      p.SendMsg(vp[GetIndexOfWeightedNormalDistribution(percentstd)]);
//    }


		for_each(begin(vp), end(vp), [&](Process& p) {
			//	rintf("random : %f\n", PhysicalTime::GetRandomNumberInRange(1, 100)*1.0);
			//if (PhysicalTime::GetRandomNumberInRange(1, 100)*1.0 <= alpha * 100) {
			p.SendMsg(vp[GetIndexOfWeightedNormalDistribution(percentstd)]);
			//}
		});
	}

	double num_events = 0;
	double sum_size_active_all = 0;
	for_each(begin(vp), end(vp), [&](const Process& p) { num_events += p.GetNumEvents(); sum_size_active_all += p.GetSumSizeActive(); });

	//cout << "SUM CLOCK SIZE is " << sum_size_active_all << endl;
	//cout << "NUM EVENTS is " << num_events << endl;

	//cout << "AVERAGE CLOCK SIZE is " << sum_size_active_all / num_events << endl;
	cout << "AVERAGE CLOCK SIZE/n " << sum_size_active_all / (num_events *number_of_processes) << endl;

	return sum_size_active_all / (num_events *number_of_processes);
}

double inline RandomUnicastExperiment() {
	vector<Process> vp = InitProcess();
	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++) {
		for_each(begin(vp), end(vp), [](Process& p) {p.UnlockTime(); p.ReceiveMsg(); });
		for_each(begin(vp), end(vp), [&vp](Process& p) {
	//	rintf("random : %f\n", PhysicalTime::GetRandomNumberInRange(1, 100)*1.0);
			//if (PhysicalTime::GetRandomNumberInRange(1, 100)*1.0 <= alpha * 100) {
				p.SendMsg(vp[GetIndexOfWeightedRandom(prob_matrix[p.GetId()])]);
			//}
		});
	}

	double num_events = 0;
	double sum_size_active_all = 0;
	for_each(begin(vp), end(vp), [&](const Process& p) { num_events += p.GetNumEvents(); sum_size_active_all += p.GetSumSizeActive(); });

	//cout << "SUM CLOCK SIZE is " << sum_size_active_all << endl;
	//cout << "NUM EVENTS is " << num_events << endl;

	//cout << "AVERAGE CLOCK SIZE is " << sum_size_active_all / num_events << endl;
	cout << "AVERAGE CLOCK SIZE/n " << sum_size_active_all / (num_events *number_of_processes) << endl;

	return sum_size_active_all / (num_events *number_of_processes);
}


void inline RandomUnicastExperimentSnapshotVariance(const string& s) {
	ofstream out(s);
	vector<Process> vp = InitProcess();
	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++) {
		for_each(begin(vp), end(vp), [](Process& p) {p.UnlockTime(); p.ReceiveMsg(); });
		for_each(begin(vp), end(vp), [&vp](Process& p) {

			//if (PhysicalTime::GetRandomNumberInRange(1, 100)*1.0 <= alpha * 100) {
			p.SendMsg(vp[GetIndexOfWeightedRandom(prob_matrix[p.GetId()])]);
			//}
		});
		double sum_size_active_all = 0;
		vector<double> current_size_distribution;
		for_each(begin(vp), end(vp), [&](Process& p) {int  casn = p.CountActiveSizeNow(); current_size_distribution.push_back(casn / (float)number_of_processes); });
		out << Variance(current_size_distribution) << endl;
	}


}
void inline RandomUnicastExperimentSnapshotY2(const string& s) {
	ofstream out(s);
	vector<Process> vp = InitProcess();
	for (absolute_time = 0; absolute_time < run_up_to; absolute_time++) {
		for_each(begin(vp), end(vp), [](Process& p) {p.UnlockTime(); p.ReceiveMsg(); });
		for_each(begin(vp), end(vp), [&vp](Process& p) {

			//if (PhysicalTime::GetRandomNumberInRange(1, 100)*1.0 <= alpha * 100) {
			p.SendMsg(vp[GetIndexOfWeightedRandom(prob_matrix[p.GetId()])]);
			//}
		});
		double sum_sq_size_active_all = 0;
		vector<double> current_size_distribution;
		for_each(begin(vp), end(vp), [&](Process& p) {int  casn = p.CountActiveSizeNow();  sum_sq_size_active_all += casn*casn;  });
		//	out_matrix << endl;
		out << sum_sq_size_active_all / (vp.size()*vp.size()) << endl;
	}


}



void inline VaryEpsilon(const string& in, int epsupto) {
//string s = in + to_string(alpha*100)  string(".txt");

	string s = in +  string(".txt");
	ofstream out(s);

	for (int i = 1; i <= epsupto; i++) {
		printf("epsilon %d ", i);
		epsilon = i;
		out << RandomUnicastExperiment() << endl;
	}
}
void inline VaryEpsilonWithNormalDistribution(double stdpercent) {
//string s = string("normaldist-delta10alpha25") + to_string(alpha*100)  string(".txt");

	string s = string("normaldist-delta10alpha25") +   string(".txt");
	ofstream out(s);

	for (int i = 1; i <= 150; i++) {
		printf("epsilon %d ", i);
		epsilon = i;
		out << RandomUnicastExperimentWithNormalDistribution(stdpercent) << endl;
	}
}

void inline VaryDelta(const vector<int>& deltas, int eps_up_to) {
	vector<vector<double>> charts;
	for (int d : deltas) {
		delta =delta_max= d;
		printf("delta=%d\n", d);
		vector<double> plot_by_eps;
		for (int i = 1; i <= eps_up_to; i+=4) {
			epsilon = i;
			plot_by_eps.push_back(RandomUnicastExperiment());
		}
		charts.push_back(plot_by_eps);
		//cout << endl;
	}
	ofstream out("n=100,alpha=1,vdelta100.txt");
	charts = Transpose(charts);
	for (auto& row : charts) {
		for (double v : row) {
			out << v << " ";
		}
		out << endl;
	}
}

void inline VaryAlpha(const vector<double>& alphas, int eps_up_to) {
	vector<vector<double>> charts;
	for (double a : alphas) {
		printf("alpha=%f\n", a);
		alpha = a;
		vector<double> plot_by_eps;
		for (int i = 1; i <= eps_up_to; i += 4) {
			epsilon = i;
			plot_by_eps.push_back(RandomUnicastExperiment());
		}
		charts.push_back(plot_by_eps);
	}
	ofstream out("n=100,delta=30,varyalpha,+4to800.txt");
	charts = Transpose(charts);
	for (auto& row : charts) {
		for (double v : row) {
			out << v << " ";
		}
		out << endl;
	}
}

*/
