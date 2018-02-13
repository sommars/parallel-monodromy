#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <locale>

using namespace std;

extern mt19937 mt; //global RNG to avoid copying it
typedef struct HomotopyNode HomotopyNode;

//------------------------------------------------------------------------------
struct Correspondence
{
  int SourceSol;
  int DestSol;
  double TimeRequired;
  bool IsFailure;
};

//------------------------------------------------------------------------------
struct HomotopyDirectedEdge
{
  int SourceNodeID;
  int TargetNodeID;
  map<int, Correspondence> Correspondences;
  int ID;
  int OtherEdgeID;
  set<int> TrackableSolutions;
  double ExpectedValue;
  double ExpectedFailures;
  int TrackerCount;
  int SuccessfulCorrespondences;
  int SourceFailures;
  int NumberOfAttempts;
};

//------------------------------------------------------------------------------
struct HomotopyGraph
{
  int RootCount;
  vector<HomotopyNode> Nodes;
  vector<HomotopyDirectedEdge> Edges;
  int NumberOfCompleteNodes;
  bool NoFailures;
  double Alpha;
  double Lambda;
  set<int> EdgesBeingTrackedIDs;
  string EVType;
  HomotopyGraph(int RC): RootCount(RC), NumberOfCompleteNodes(0) {};
};

//------------------------------------------------------------------------------
struct HomotopyNode
{
  set<int> OutgoingEdgeIDs;
  set<int> IncomingEdgeIDs;
  vector<bool> Solutions;
  int SolutionCount;
  int ID;
  double ExpectedValue;
  map<int, int> InwardTaskCounts;
  HomotopyNode(HomotopyGraph * G): ID(G->Nodes.size()), SolutionCount(0), Solutions(vector<bool> (G->RootCount)) {};
};

//------------------------------------------------------------------------------
struct PathTracker
{
  int EdgeID;
  int StartSolution;
  Correspondence C;
  mutable long double TimeLeft;
  bool operator<( const PathTracker& other ) const
  {
    if (TimeLeft != other.TimeLeft)
      return (TimeLeft < other.TimeLeft);
    else if (EdgeID != other.EdgeID)
      return (EdgeID < other.EdgeID);
    else
      return (StartSolution < other.StartSolution);
  }

};

//------------------------------------------------------------------------------
HomotopyGraph InitializeEmptyGraphFromCompletedGraph(HomotopyGraph* CompletedG);

//------------------------------------------------------------------------------
HomotopyGraph InitializeGraphFromFile(string &FileName, int seed);

//------------------------------------------------------------------------------
void ComputeExpectedValues(HomotopyGraph* G, HomotopyNode* N);
