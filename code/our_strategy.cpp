#define VERBOSE
#include "our_strategy.h"

//------------------------------------------------------------------------------
void PathFinished(HomotopyGraph* G, PathTracker* Tracker)
{
  HomotopyDirectedEdge* E = &(G->Edges[Tracker->EdgeID]);
  HomotopyDirectedEdge* OtherE = &(G->Edges[E->OtherEdgeID]);
  HomotopyNode* TargetNode = &(G->Nodes[E->TargetNodeID]);
  HomotopyNode* SourceNode = &(G->Nodes[E->SourceNodeID]);
  cerr << "PathFinished >>> ("<< E->SourceNodeID << "," << E->TargetNodeID << ")" << endl;
  E->Correspondences[Tracker->StartSolution] = Tracker->C;
  if (!Tracker->C.IsFailure)  //i.e. not a failed track
  {
    Correspondence OtherC;
    OtherC.IsFailure = false;
    OtherC.DestSol = Tracker->C.SourceSol;
    OtherC.SourceSol = Tracker->C.DestSol;
    OtherE->Correspondences[Tracker->C.DestSol] = OtherC;
    if (TargetNode->Solutions[Tracker->C.DestSol] == false) // we found a new solution
    {
      E->SuccessfulCorrespondences++;
      OtherE->SuccessfulCorrespondences++;
      TargetNode->Solutions[Tracker->C.DestSol] = true;
      TargetNode->SolutionCount++;
      for (set<int>::iterator it=TargetNode->OutgoingEdgeIDs.begin(); it!=TargetNode->OutgoingEdgeIDs.end(); it++)
      {
        HomotopyDirectedEdge &E = G->Edges[*it]; 
        //cerr << "inserting " << Tracker->C.DestSol << " into ("<< E.SourceNodeID << "," << E.TargetNodeID << ")\n";
        //if (E.Correspondences[Tracker->C.SourceSol].DestSol != -1) { // putting this in creates "collisions" unexplainably!!!
        E.TrackableSolutions.insert(Tracker->C.DestSol); // don't perform this insertion if this solution has already failed along E
        //} else {
        //cerr << E.Correspondences[Tracker->C.SourceSol].DestSol << endl;
        //  }
      }
      if (TargetNode->SolutionCount == G->RootCount)
        G->NumberOfCompleteNodes++;
    }
    OtherE->TrackableSolutions.erase(Tracker->C.DestSol);
  }
  if (Tracker->C.IsFailure)
  {
    E->SourceFailures++; // failures are one-way
  }

    
  E->TrackerCount--;
  E->NumberOfAttempts++;

#ifdef VERBOSE
  cerr << ">>> #Q: ";
  for (auto& v : G->Nodes)
    cerr << v.SolutionCount << " ";
  cerr << "  #C: ";
  for (auto& E : G->Edges)
    cerr << "(" << E.SuccessfulCorrespondences << E.TrackerCount << ") ";
  cerr << endl;
#endif	

  if (G->UseOldEVs == true) {
    if (G->ComputeEVOption == 1)
    {
      ComputeExpectedValuesOLDWITHNOFAILURESONLY(G, TargetNode);
    } else if (G->ComputeEVOption == 2)
    {
      ComputeExpectedValuesOLDWITHNOFAILURESONLY(G, TargetNode);
      ComputeExpectedValuesOLDWITHNOFAILURESONLY(G, SourceNode);
      
    } else if (G->ComputeEVOption == 3)
    {
      for (size_t i = 0; i != G->Nodes.size(); i++)
        ComputeExpectedValuesOLDWITHNOFAILURESONLY(G, &G->Nodes[i]);
    }
    else 
      throw invalid_argument("Please input a valid EV Option.");
  } else {
    if (G->ComputeEVOption == 1)
    {
      ComputeExpectedValues(G, TargetNode);
    } else if (G->ComputeEVOption == 2)
    {
      ComputeExpectedValues(G, TargetNode);
      ComputeExpectedValues(G, SourceNode);
      
    } else if (G->ComputeEVOption == 3)
    {
      for (size_t i = 0; i != G->Nodes.size(); i++)
        ComputeExpectedValues(G, &G->Nodes[i]);
    }
    else 
      throw invalid_argument("Please input a valid EV Option.");
  }
  
};

//------------------------------------------------------------------------------

bool TryChoosePath(HomotopyGraph* G, HomotopyGraph* CompletedG, PathTracker* Tracker)
{
  double MaxExpectedVal = 0;
  set<int> MaxEdgeIDs;
#ifdef VERBOSE
  cerr << "-- trying to choose...\n";
#endif
  for (size_t i = 0; i != G->Edges.size(); i++)
  {
    HomotopyDirectedEdge* E = &(G->Edges[i]);
    if (E->TrackableSolutions.size() == 0)
      continue;
#ifdef VERBOSE
    cerr << "Edge with ID " << E->ID << " had E("<< E->SourceNodeID << "," << E->TargetNodeID << ")->EV = " << E->ExpectedValue << endl;
#endif
    if (E->ExpectedValue > MaxExpectedVal)
    {
      MaxExpectedVal = E->ExpectedValue;
      MaxEdgeIDs.clear();
      MaxEdgeIDs.insert(i);
    } else if (E->ExpectedValue == MaxExpectedVal)
    {
      MaxEdgeIDs.insert(i);
    }
  }
  if (MaxEdgeIDs.size() == 0)
    return false;

  int TargetCount =0; // for tie-breaking
  HomotopyDirectedEdge* E = &(G->Edges[*MaxEdgeIDs.begin()]);
  for (set<int>::iterator it=MaxEdgeIDs.begin(); it!=MaxEdgeIDs.end(); it++)
  {
    if (G->Nodes[G->Edges[*it].TargetNodeID].SolutionCount > TargetCount)
    {
      E = &(G->Edges[*it]);
      TargetCount = G->Nodes[E->TargetNodeID].SolutionCount;
    }
  }
  E->TrackerCount++;
  
  int SolutionToTrack = *(E->TrackableSolutions.begin());
  E->TrackableSolutions.erase(SolutionToTrack);

  Tracker->StartSolution = SolutionToTrack;
  Tracker->EdgeID = E->ID;
  
  Correspondence CompleteC = CompletedG->Edges[E->ID].Correspondences[SolutionToTrack];

  Correspondence C;
  C.SourceSol = CompleteC.SourceSol;
  C.IsFailure = CompleteC.IsFailure; //!!! this line was not there before
  if (!C.IsFailure)
    C.DestSol = CompleteC.DestSol;
  C.TimeRequired = CompleteC.TimeRequired;
  Tracker->C = C;
  Tracker->TimeLeft = C.TimeRequired;
#ifdef VERBOSE
  cerr << "chose (" << E->SourceNodeID << "," << E->TargetNodeID << ")\n";
#endif
  return true;
};
