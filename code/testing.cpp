#include "testing.h"

//------------------------------------------------------------------------------
HomotopyRunData SimulateRun(HomotopyGraph* CompletedG, HomotopyGraph* FuzzyG, int ThreadCount)
{
  set<PathTracker> CurrentTrackerSet;
  
  HomotopyRunData Data;
  Data.RootCount = CompletedG->RootCount;
  Data.TotalTime = 0;
  Data.TotalPathTracks = 0;
  Data.TracksTillNodeSolved = -1;
  Data.TimeTillNodeSolved = -1;
  Data.TimeIdle = 0;
  Data.CorrespondenceCollisions = 0;
  Data.GraphIsComplete = false;
  Data.ExistsCompleteNode = false;
  Data.PathFailures = 0;
  
  while (true)
  {
  
    // We have just completed a node
    if (FuzzyG->NumberOfCompleteNodes > 0)
    {
      cout << "Node root counts:" << endl;
      for(size_t i=0; i<FuzzyG->Nodes.size(); i++)
        cout << FuzzyG->Nodes[i].SolutionCount << ",";
      cout << endl;
      Data.ExistsCompleteNode = true;
      Data.DiscoveredRootCount = Data.RootCount;
      Data.TimeTillNodeSolved = Data.TotalTime;
      Data.TracksTillNodeSolved = Data.TotalPathTracks;
      Data.Message = "Filled out a node!";
        
      return Data;
    };
    // Filling the tracker list, or trying.
    while (ThreadCount > CurrentTrackerSet.size())
    {
      PathTracker T;
      if (not TryChoosePath(FuzzyG, CompletedG, &T))
        break;
      
      CurrentTrackerSet.insert(T);
      if (FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].TargetNodeID].InwardTaskCounts.count(T.EdgeID) > 1)
        FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].TargetNodeID].InwardTaskCounts[T.EdgeID] += 1;
      else
        FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].TargetNodeID].InwardTaskCounts[T.EdgeID] = 1;
      
      if (FuzzyG->UseOldEVs == true) {
        ComputeExpectedValuesOLDWITHNOFAILURESONLY(FuzzyG, &FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].TargetNodeID]);
        ComputeExpectedValuesOLDWITHNOFAILURESONLY(FuzzyG, &FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].SourceNodeID]);
      } else {
        ComputeExpectedValues(FuzzyG, &FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].TargetNodeID]);
        ComputeExpectedValues(FuzzyG, &FuzzyG->Nodes[FuzzyG->Edges[T.EdgeID].SourceNodeID]);
      };

    };
    if (CurrentTrackerSet.size() == 0)
    {
      Data.DiscoveredRootCount = 0;
      for(size_t i=0; i<FuzzyG->Nodes.size(); i++)
      {
        cout << FuzzyG->Nodes[i].SolutionCount << ",";
        if (Data.DiscoveredRootCount < FuzzyG->Nodes[i].SolutionCount)
          Data.DiscoveredRootCount = FuzzyG->Nodes[i].SolutionCount;
      };
      cout << endl;
      Data.Message = "Uh oh, current tracker set had size zero.";
      return Data;
    };

    PathTracker NextFinishedTracker = *(CurrentTrackerSet.begin());
    CurrentTrackerSet.erase(NextFinishedTracker);
    
    FuzzyG->Nodes[FuzzyG->Edges[NextFinishedTracker.EdgeID].TargetNodeID].InwardTaskCounts[NextFinishedTracker.EdgeID] -= 1;
  
    HomotopyDirectedEdge* EdgeInCompleteGraph = &(CompletedG->Edges[NextFinishedTracker.EdgeID]);
    HomotopyDirectedEdge* EdgeInFuzzyGraph = &(FuzzyG->Edges[NextFinishedTracker.EdgeID]);

    // Check if correspondence already exists. If so, it's a collision.
    if (EdgeInFuzzyGraph->Correspondences.find(NextFinishedTracker.StartSolution) != EdgeInFuzzyGraph->Correspondences.end())
      Data.CorrespondenceCollisions++;
    
    if (NextFinishedTracker.C.IsFailure)
      Data.PathFailures++;

    PathFinished(FuzzyG, &NextFinishedTracker);

    Data.TotalTime += NextFinishedTracker.TimeLeft;
    int IdleThreadCount = ThreadCount - CurrentTrackerSet.size() - 1;
    if (IdleThreadCount > 0)
      Data.TimeIdle = Data.TimeIdle + IdleThreadCount*NextFinishedTracker.TimeLeft;

    Data.TotalPathTracks++;
    
    for (set<PathTracker>::iterator it=CurrentTrackerSet.begin(); it!=CurrentTrackerSet.end(); it++)
      (*it).TimeLeft -= NextFinishedTracker.TimeLeft;
    }

  return Data;
}

//------------------------------------------------------------------------------
HomotopyRunData SetUpAndRun(HomotopyTestSetup* Setup)
{
  
  if (Setup->Seeds[Setup->TrialIndex] == -1)
  {
    random_device rd;
    Setup->Seeds[Setup->TrialIndex] = rd();
   };
  HomotopyGraph CompletedGraph = InitializeGraphFromFile(Setup->FileName, Setup->Seeds[Setup->TrialIndex]);
  CompletedGraph.EVType = Setup->EVType;
  CompletedGraph.ComputeEVOption = Setup->ComputeEVOption;
  CompletedGraph.UseOldEVs = Setup->UseOldEVs;
  Setup->RootCount = CompletedGraph.RootCount;
  Setup->NodeCount = CompletedGraph.Nodes.size();
  Setup->Alpha = CompletedGraph.Alpha;
  Setup->Lambda = CompletedGraph.Lambda;

  HomotopyGraph EmptyGraph = InitializeEmptyGraphFromCompletedGraph(&CompletedGraph);
  
  HomotopyRunData Data = SimulateRun(&CompletedGraph, &EmptyGraph, Setup->ThreadCount);
  
  return Data;
}

//------------------------------------------------------------------------------
AverageHomotopyData AverageTests(HomotopyTestSetup* Setup)
{
  vector<HomotopyRunData> Data;
  for (size_t i = 0; i != Setup->Seeds.size(); i++)
  {
    Setup->TrialIndex = i;
    Data.push_back(SetUpAndRun(Setup));
  };
  
  Setup->TrialCount = Setup->Seeds.size();
  
  AverageHomotopyData AvgData;
  AvgData.RootCount = 0;
  AvgData.DiscoveredRootCount = 0;
  AvgData.TotalTime = 0;
  AvgData.TotalPathTracks = 0;
  AvgData.TracksTillNodeSolved = 0;
  AvgData.TimeTillNodeSolved = 0;
  AvgData.TimeIdle = 0;
  AvgData.CorrespondenceCollisions = 0;
  AvgData.PathFailures = 0;
  AvgData.ExistsCompleteNode = 0;

  for (size_t i = 0; i != Data.size(); i++)
  {
    AvgData.RootCount += Data[i].RootCount;
    AvgData.DiscoveredRootCount += Data[i].DiscoveredRootCount;
    AvgData.TotalTime += Data[i].TotalTime;
    AvgData.TotalPathTracks += Data[i].TotalPathTracks;
    AvgData.TracksTillNodeSolved += Data[i].TracksTillNodeSolved;
    AvgData.TimeTillNodeSolved += Data[i].TimeTillNodeSolved;
    AvgData.TimeIdle += Data[i].TimeIdle;
    AvgData.CorrespondenceCollisions += Data[i].CorrespondenceCollisions;
    AvgData.PathFailures += Data[i].PathFailures;
    AvgData.ExistsCompleteNode += Data[i].ExistsCompleteNode;
  };


  AvgData.RootCount = AvgData.RootCount/Setup->Seeds.size();
  AvgData.DiscoveredRootCount = AvgData.DiscoveredRootCount/Setup->Seeds.size();
  AvgData.TotalTime = AvgData.TotalTime/Setup->Seeds.size();
  AvgData.TotalPathTracks = AvgData.TotalPathTracks/Setup->Seeds.size();
  AvgData.TracksTillNodeSolved = AvgData.TracksTillNodeSolved/Setup->Seeds.size();
  AvgData.TimeTillNodeSolved = AvgData.TimeTillNodeSolved/Setup->Seeds.size();
  AvgData.TimeIdle = AvgData.TimeIdle/Setup->Seeds.size();
  AvgData.CorrespondenceCollisions = AvgData.CorrespondenceCollisions/Setup->Seeds.size();
  AvgData.PathFailures = AvgData.PathFailures/Setup->Seeds.size();
  AvgData.ExistsCompleteNode = AvgData.ExistsCompleteNode/Setup->Seeds.size();

  return AvgData;
}
