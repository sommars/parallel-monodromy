#include "our_strategy.h"

//------------------------------------------------------------------------------
struct HomotopyRunData {
  int RootCount;
  int DiscoveredRootCount;
  double TotalTime;
  int TotalPathTracks;
  int TracksTillNodeSolved;
  double TimeTillNodeSolved;
  double TimeIdle;
  int CorrespondenceCollisions;
  bool GraphIsComplete;
  bool ExistsCompleteNode;
  int PathFailures;
  string Message;
  // For printing it:
  friend std::ostream& operator <<(std::ostream& os, HomotopyRunData const& Data)
  {
    return os << "TrueRootCount: " << Data.RootCount << endl
              << "DiscoveredRootCount: " << Data.DiscoveredRootCount << endl
              << "TotalTime: " << Data.TotalTime << endl
              << "TotalPathTracks: " << Data.TotalPathTracks << endl
              << "TimeIdle: " << Data.TimeIdle << endl
              << "CorrespondenceCollisions: " << Data.CorrespondenceCollisions << endl
              << "GraphIsComplete: " << Data.GraphIsComplete << endl
              << "ExistsCompleteNode: " << Data.ExistsCompleteNode << endl
              << "PathFailures: " << Data.PathFailures << endl
              <<  endl << Data.Message << endl;
  }
};

//------------------------------------------------------------------------------
struct AverageHomotopyData {
  int RootCount;
  double DiscoveredRootCount;
  double TotalTime;
  double TotalPathTracks;
  double TracksTillNodeSolved;
  double TimeTillNodeSolved;
  double TimeIdle;
  double CorrespondenceCollisions;
  double PathFailures;
  double ExistsCompleteNode;
  // For printing it:
  friend std::ostream& operator <<(std::ostream& os, AverageHomotopyData const& Data)
  {
    return os << "RootCount: " << setprecision(0) << Data.RootCount << endl
              << "DiscoveredRootCount: " << setprecision(2) << Data.DiscoveredRootCount << endl
              << "TotalTime: " << setprecision(10) << Data.TotalTime << endl
              << "TotalPathTracks: " << setprecision(0) << Data.TotalPathTracks << endl
              << "TimeIdle: " << setprecision(2) << Data.TimeIdle << endl
              << "CorrespondenceCollisions: " << setprecision(0) << Data.CorrespondenceCollisions << endl
              << "PathFailures: " << setprecision(0) << Data.PathFailures << endl
              << "Percentage of trials that completed a node: " << setprecision(0) << Data.ExistsCompleteNode*100 <<"%" << endl;
  }
};

//------------------------------------------------------------------------------
struct HomotopyTestSetup
{
  int RootCount;
  int NodeCount;
  string FileName;
  int ThreadCount;
  int TrialCount;
  string EVType;
  int TrialIndex;
  vector<int64_t> Seeds;
  double Alpha;
  double Lambda;
  int ComputeEVOption;
  bool UseOldEVs;
  friend std::ostream& operator <<(std::ostream& os, HomotopyTestSetup const& Setup)
  {
    string SeedsStr;
    for (size_t i = 0; i != Setup.Seeds.size(); i++)
      SeedsStr += " " + to_string(Setup.Seeds[i]);
    return os << "---------HomotopyTestSetup---------" << endl
              << "RootCount: " << Setup.RootCount << endl
              << "NodeCount: " << Setup.NodeCount << endl
              << "ThreadCount: " << Setup.ThreadCount << endl
              << "TrialCount: " << Setup.TrialCount << endl
              << "Alpha: " << Setup.Alpha << endl
              << "Seeds:" << SeedsStr << endl
              << "EVType: " << Setup.EVType << endl
              << "Option:" << Setup.ComputeEVOption << endl
              << "UseOldEVs: " << Setup.UseOldEVs << endl << endl << endl; 
  }
};

//------------------------------------------------------------------------------
AverageHomotopyData AverageTests(HomotopyTestSetup* Setup);
