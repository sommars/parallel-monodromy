#include "generate_tables.h"

string EVType = "Original";

//------------------------------------------------------------------------------
void WriteRandomComparisonToFile()
{
	string FileName = "../experiments/randomcomparisontable.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  // (#vertices, edge multiplicity)
  vector<vector<int> > Configurations = {{3,2},{4,2},{5,2},{3,3},{4,3}};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(0) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "c|";
  s << "}" << endl;
  s << "\\#Threads";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "& (" << Configurations[i][0] << "," << Configurations[i][1] << ") ";
  s << "\\\\ \\hline" << endl;
  
  HomotopyTestSetup Setup;
	Setup.RootCount = 10000;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.ProbabilityOfFailure = 0;
	Setup.EVType = EVType;

  for (size_t i = 0; i != ThreadCounts.size(); i++)
  {
  
		Setup.ThreadCount = ThreadCounts[i];
		s << Setup.ThreadCount;
    for (size_t j = 0; j != Configurations.size(); j++)
    {
			Setup.NodeCount = Configurations[j][0];
			Setup.EdgeCount = Configurations[j][1];
			Setup.UseRandomStrategy = false;
			AverageHomotopyData Data = AverageTests(Setup, false);
			Setup.UseRandomStrategy = true;
			s << "& " << 100 * Data.TotalPathTracks / AverageTests(Setup, false).TotalPathTracks << "\\%";
    };
    s << "\\\\" << endl;
  };
  s << "\\end{tabular}" << endl;
  s << "\\caption{Ratio of number of path tracks necessary in new algorithm to number of path tracks required by a random algorithm. Each cell represents the average of " << Setup.TrialCount << " trials, for a system with " << Setup.RootCount << " solutions.}" << endl;
  s << "\\label{randomcomparisontable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();
};

//------------------------------------------------------------------------------
void WriteIdleTimeToFile()
{
	string FileName = "../experiments/idletable.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  // (#vertices, edge multiplicity)
  vector<vector<int> > Configurations = {{4,1},{6,1},{8,1},{10,1}};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(2) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "r|";
  s << "}" << endl;
  s << "\\#Threads";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "& (" << Configurations[i][0] << "," << Configurations[i][1] << ") ";
  s << "\\\\ \\hline" << endl;
  
  HomotopyTestSetup Setup;
	Setup.RootCount = 10000;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.ProbabilityOfFailure = 0;
	Setup.UseRandomStrategy = false;
	Setup.EVType = EVType;

  for (size_t i = 0; i != ThreadCounts.size(); i++)
  {
  
		Setup.ThreadCount = ThreadCounts[i];
		s << Setup.ThreadCount;
    for (size_t j = 0; j != Configurations.size(); j++)
    {
			Setup.NodeCount = Configurations[j][0];
			Setup.EdgeCount = Configurations[j][1];
			AverageHomotopyData Data = AverageTests(Setup, false);
			s << "& " << 100 * Data.TimeIdle / (Data.TotalTime * Setup.ThreadCount) << "\\%";
    };
    s << "\\\\" << endl;
  };
  s << "\\end{tabular}" << endl;
  s << "\\caption{Percent of time idle per processor for varying numbers of (\\#Vertices, \\#Edges) and numbers of threads. Each cell represents the average of " << Setup.TrialCount << " trials, for a system with " << Setup.RootCount << " solutions.}" << endl;
  s << "\\label{idletable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();

};

//------------------------------------------------------------------------------
void WriteEfficiencyExampleToFile()
{
	string FileName = "../experiments/efficiencytable.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  vector<int> RootCounts = {100,500,1000,5000, 10000};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(0) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != RootCounts.size(); i++)
    s << "r|";
  s << "}" << endl;
  s << "\\#Threads ";

  for (size_t i = 0; i != RootCounts.size(); i++)
    s << "& " << RootCounts[i];
  s << "\\\\ \\hline" << endl;


  HomotopyTestSetup Setup;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.EdgeCount = 2;
	Setup.NodeCount = 3;
	Setup.ProbabilityOfFailure = 0;
	Setup.EVType = EVType;

	Setup.UseRandomStrategy = false;
	vector<vector<double> > Timings(RootCounts.size());
	
	for (size_t i = 0; i != ThreadCounts.size(); i++)
	{
		s << ThreadCounts[i];
		for (size_t j = 0; j != RootCounts.size(); j++)
		{
			Setup.ThreadCount = ThreadCounts[i];
			Setup.RootCount = RootCounts[j];
			
			Timings[j].push_back(AverageTests(Setup, false).TotalTime);
			s << " & " << setprecision(2) << (Timings[j][0] / Timings[j][i]) / Setup.ThreadCount * 100 << "\\%";
		};
		s << "\\\\" << endl;
	};
  s << "\\end{tabular}" << endl;
  s << "\\caption{This table displays the efficiency of our algorithm for varying number of root counts and threads. Each cell represents the average of " << Setup.TrialCount << " trials.}" << endl;
  s << "\\label{efficiencytable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();

};

//------------------------------------------------------------------------------
void WriteRandomEfficiencyExampleToFile()
{
	string FileName = "../experiments/randomefficiencytable.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  vector<int> RootCounts = {100,500,1000,5000, 10000};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(0) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != RootCounts.size(); i++)
    s << "r|";
  s << "}" << endl;
  s << "\\#Threads ";

  for (size_t i = 0; i != RootCounts.size(); i++)
    s << "& " << RootCounts[i];
  s << "\\\\ \\hline" << endl;


  HomotopyTestSetup Setup;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.EdgeCount = 2;
	Setup.NodeCount = 3;
	Setup.ProbabilityOfFailure = 0;
	Setup.EVType = EVType;

	Setup.UseRandomStrategy = true;
	vector<vector<double> > Timings(RootCounts.size());
	
	for (size_t i = 0; i != ThreadCounts.size(); i++)
	{
		s << ThreadCounts[i];
		for (size_t j = 0; j != RootCounts.size(); j++)
		{
			Setup.ThreadCount = ThreadCounts[i];
			Setup.RootCount = RootCounts[j];
			
			Timings[j].push_back(AverageTests(Setup, false).TotalTime);
			s << " & " << setprecision(2) << (Timings[j][0] / Timings[j][i]) / Setup.ThreadCount * 100 << "\\%";
		};
		s << "\\\\" << endl;
	};
  s << "\\end{tabular}" << endl;
  s << "\\caption{This table displays the efficiency of a random algorithm for varying number of root counts and threads. Each cell represents the average of " << Setup.TrialCount << " trials.}" << endl;
  s << "\\label{randomefficiencytable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();
};

//------------------------------------------------------------------------------
void WriteGlobalFailureToFile()
{
	string FileName = "../experiments/globalfailure.tex";
	
  vector<int> NodeCounts = {4,6,8,10};
  vector<int> SolCounts = {100,500,1000,5000,10000};
  double Precision = 0.00001;
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(6) <<  "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != NodeCounts.size(); i++)
    s << "c|";
  s << "}" << endl;
  s << "\\#Solutions";
  for (size_t i = 0; i != NodeCounts.size(); i++)
    s << " & "<< NodeCounts[i];
  s << "\\\\ \\hline" << endl;
  
  HomotopyTestSetup Setup;
	Setup.GraphName = "complete";
	Setup.ThreadCount = 1;
	Setup.EdgeCount = 1;
	Setup.EVType = EVType;

	Setup.UseRandomStrategy = false;
  for (size_t i = 0; i != SolCounts.size(); i++)
  {
  
		Setup.RootCount = SolCounts[i];
		s << Setup.RootCount;
    for (size_t j = 0; j != NodeCounts.size(); j++)
    {
			Setup.NodeCount = NodeCounts[j];
			s << "& " << setprecision(2) << FindGlobalFailurePoint(Setup, Precision, false) * 100 << "\\%";
    };
    s << "\\\\" << endl;
  };
  s << "\\end{tabular}" << endl;
  s << "\\caption{For various numbers of vertices and root counts, this table contains the resulting values of $p$.}" << endl;
  s << "\\label{globalfailure}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();
};

//------------------------------------------------------------------------------
void WriteParallelFailureToFile()
{
	string FileName = "../experiments/parallelfailure.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  // (#vertices, edge multiplicity)
  vector<double> FailureProbabilities = {0.01,0.025,0.05,0.10};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(2) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != FailureProbabilities.size(); i++)
    s << "c|";
  s << "}" << endl;
  s << "\\#Threads";
  for (size_t i = 0; i != FailureProbabilities.size(); i++)
    s << "& " << FailureProbabilities[i] * 100 << "\\%";
  s << "\\\\ \\hline" << endl;
  
  HomotopyTestSetup Setup;
	Setup.RootCount = 10000;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.UseRandomStrategy = false;
	Setup.EVType = EVType;
	Setup.NodeCount = 4;
	Setup.EdgeCount = 1;
	vector<double> PathTracks;
	
  for (size_t i = 0; i != ThreadCounts.size(); i++)
  {
  
		Setup.ThreadCount = ThreadCounts[i];
		s << Setup.ThreadCount;
    for (size_t j = 0; j != FailureProbabilities.size(); j++)
    {
			Setup.ProbabilityOfFailure = FailureProbabilities[j];
			AverageHomotopyData Data = AverageTests(Setup, false);
			if (i == 0)
				PathTracks.push_back(Data.TotalPathTracks);
			s << "& " << Data.TotalPathTracks / PathTracks[j] * 100 << "\\%";
    };
    s << "\\\\" << endl;
  };
  s << "\\end{tabular}" << endl;
  s << "\\caption{For various values of $p$, this table contains the ratio of the number of path tracks necessary with a given number of threads to the number of path tracks with one thread. Each cell represents the average of " << Setup.TrialCount << " trials, for a system with " << Setup.RootCount << " solutions.}" << endl;
  s << "\\label{parallelfailuretable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();

};

//------------------------------------------------------------------------------
void WritePotentialComparisonToFile()
{
	string FileName = "../experiments/potentialcomparisontable.tex";
	
  vector<int> ThreadCounts = {1,2,3,4,8,16,32,64,128};
  // (#vertices, edge multiplicity)
  vector<vector<int> > Configurations = {{3,2},{4,2},{5,2},{3,3},{4,3}};
  
  stringstream s;
  s.imbue(std::locale(""));
  s << fixed << setprecision(0) << "\\begin{table}[h]" << endl;
  s << "\\centering" << endl;
  s << "\\begin{tabular}{l||";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "c|";
  s << "}" << endl;
  s << "\\#Threads";
  for (size_t i = 0; i != Configurations.size(); i++)
    s << "& (" << Configurations[i][0] << "," << Configurations[i][1] << ") ";
  s << "\\\\ \\hline" << endl;
  
  HomotopyTestSetup Setup;
	Setup.RootCount = 10000;
	Setup.GraphName = "complete";
	Setup.TrialCount = 30;
	Setup.ProbabilityOfFailure = 0;
	Setup.UseRandomStrategy = false;

  for (size_t i = 0; i != ThreadCounts.size(); i++)
  {
  
		Setup.ThreadCount = ThreadCounts[i];
		s << Setup.ThreadCount;
    for (size_t j = 0; j != Configurations.size(); j++)
    {
			Setup.NodeCount = Configurations[j][0];
			Setup.EdgeCount = Configurations[j][1];
			Setup.EVType = "WeightTowardCompleteNode";
			AverageHomotopyData Data = AverageTests(Setup, false);
			Setup.EVType = "Original";
			s << "& " << 100 * Data.TotalPathTracks / AverageTests(Setup, false).TotalPathTracks << "\\%";
    };
    s << "\\\\" << endl;
  };
  s << "\\end{tabular}" << endl;
  s << "\\caption{Ratio of number of path tracks necessary weighting toward a complete node to number of path tracks when filling up full graph. Each cell represents the average of " << Setup.TrialCount << " trials, for a system with " << Setup.RootCount << " solutions.}" << endl;
  s << "\\label{potentialcomparisontable}" << endl;
  s << "\\end{table}" << endl;

	ofstream OutFile (FileName);
	OutFile << s.str();
	OutFile.close();
};

//------------------------------------------------------------------------------
void GenerateTables(string TableToGen)
{
	if (TableToGen == "random")
	{
		WriteRandomComparisonToFile();
	} else if (TableToGen == "idle")
	{
		WriteIdleTimeToFile();
	} else if (TableToGen == "failure")
	{
		WriteGlobalFailureToFile();
	} else if (TableToGen == "parallelfailure")
	{
		WriteParallelFailureToFile();
	} else if (TableToGen == "efficiency")
	{
		WriteEfficiencyExampleToFile();
	} else if (TableToGen == "randomefficiency")
	{
		WriteRandomEfficiencyExampleToFile();
	} else if (TableToGen == "potential")
	{
		WritePotentialComparisonToFile();
	} else
		throw invalid_argument("Invalid option. Try running ./pmonodromy help");
};
