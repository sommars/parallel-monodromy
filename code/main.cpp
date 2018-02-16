#include "testing.h"

//------------------------------------------------------------------------------
void PrintHelp()
{
  ifstream f("../README.md");
  cout << f.rdbuf();
};

//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if ((argc == 1) or (string(argv[1]) == "help"))
  {
    PrintHelp();
    return 1;
  };
  
  HomotopyTestSetup Setup;
  Setup.ThreadCount = 1;
  Setup.EVType = "Original";
  Setup.ComputeEVOption = 2;
  Setup.UseOldEVs = false;
  
  string FileName = string(argv[1]);
  ifstream f(FileName.c_str());
  if (!f.good())
    throw invalid_argument("Please input a valid filename.");
  Setup.FileName = FileName;
  
  try
  {
    for (size_t i = 2; i < argc; )
    {
      string Option = string(argv[i]);
      if (Option == "-old")
      {
        Setup.UseOldEVs = true;
      };
      
      if ((i+1) == argc)
        throw 1;
      if (Option == "-t")
      {
        Setup.ThreadCount = atoi(argv[i+1]);
        i++;
        i++;
      } else if (Option == "-e")
      {
        string EVType = argv[i+1];
        transform(EVType.begin(), EVType.end(), EVType.begin(), ::tolower);
        if (EVType == "original")
          Setup.EVType = "Original";
        else if (EVType == "weighttowardcompletenode")
          Setup.EVType = "WeightTowardCompleteNode";
        else if (EVType == "convexcombination")
          Setup.EVType = "ConvexCombination";
        else if (EVType == "mixedstrategy")
          Setup.EVType = "MixedStrategy";    
        else
          throw invalid_argument("Please input a valid EVType. Options are Original, WeightTowardCompleteNode, ConvexCombination, and MixedStrategy");
        i++;
        i++;
      } else if (Option == "-s")
      {
        Setup.Seeds.push_back(atoi(argv[i+1]));
        i++;
        i++;
      } else if (Option == "-EVOpt")
      {
        Setup.ComputeEVOption = atoi(argv[i+1]);
        i++;
        i++;
      } else if (Option == "-a")
      {
        Setup.Alpha = stold(argv[i+1]);
        i++;
        i++;
      } else if (Option == "-l")
      {
        Setup.Lambda = stold(argv[i+1]);
        i++;
        i++;
      };
    };
  } catch (...)
  {
    cout << "Invalid input, please see instructions." << endl << endl;
    usleep(100000);
    PrintHelp();
    abort();
  };
  
  if (Setup.Seeds.size() == 0)
  {
    random_device rd;
    Setup.Seeds.push_back(rd());
  };
  
  AverageHomotopyData Data = AverageTests(&Setup);
  cout << Setup;
  cout << fixed << Data;
}
