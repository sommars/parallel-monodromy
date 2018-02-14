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
  Setup.FileName = argv[1];
  
  if (argc > 2)
    Setup.ThreadCount = atoi(argv[2]);
  else
    Setup.ThreadCount = 1;
    
  if (argc > 3)
  {
    // Let's not require the correct casing.
    string EVType = argv[3];
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
  } else
    Setup.EVType = "Original";

  if (argc > 4)
    Setup.ComputeEVOption = stoi(argv[4]);
    
  if (argc > 5 && string(argv[5]) == "old")
    Setup.UseOldEVs = true;
  else
    Setup.UseOldEVs = false;
    
  if (argc > 6)
  {
    for (size_t i = 6; i != argc; i ++)
      Setup.Seeds.push_back(stoi(argv[i]));
  } else
    Setup.Seeds.push_back(-1);
  
  AverageHomotopyData Data = AverageTests(&Setup);
  cout << Setup;
  cout << fixed << Data;
}
