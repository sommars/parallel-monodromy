#include "homotopy_graph_types.h"
mt19937 mt; //Standard mersenne_twister_engine seeded with rd()
bool Verbose;

//------------------------------------------------------------------------------
double parFailMass(int F, int d, int C, int T, double alpha, int curFailCount)
{
  return pow(alpha, T-curFailCount) * pow(1-alpha,curFailCount) * (F+curFailCount)/(d-C-T+curFailCount);
}
  

double parFailFactor(int F, int d, int C, int T, double alpha)
{
  if (d-C-T < 1)
    throw "bad potential update (division by int <= 0)";
  if (T==0)
    return 1 - F/(double)(d-C);
  double EV = 1;
  double binCoeff = 1;
  int mid = floor(T/(double)(2));
  for (int i = 0; i <= mid; i++)
  {
    if (i<mid || T%2==0)
      EV -= binCoeff * (parFailMass(F,d,C,T,alpha,i)+parFailMass(F,d,C,T,alpha,T-i));
    else
      EV -= binCoeff * parFailMass(F,d,C,T,alpha,i);
    binCoeff *= (T-i-1)/(i+1);
  };
  return EV;
}

void AddEdges(HomotopyGraph* G, HomotopyNode* N1, HomotopyNode* N2, string &CorrStr)
{
  HomotopyDirectedEdge E1;
  HomotopyDirectedEdge E2;
  string FirstVal, SecondVal, ThirdVal;
  bool OnFirstInt = false;
  bool OnSecondInt = false;
  for (string::iterator it=CorrStr.begin(); it!=CorrStr.end(); ++it)
  {
    if ((*it) == ';')
      break;
    else if ((*it) == '{') {
      OnFirstInt = true;
      OnSecondInt = false;
    } else if ((*it) == '}') {
      long double TimeRequired = stold(ThirdVal);
      if ((FirstVal != "null") && (SecondVal != "null")) {
        Correspondence C;
        C.SourceSol = stoi(FirstVal);
        C.DestSol = stoi(SecondVal);
        C.IsFailure = false;
        C.TimeRequired = TimeRequired;
        Correspondence OtherC;
        OtherC.SourceSol = C.DestSol;
        OtherC.DestSol = C.SourceSol;
        OtherC.TimeRequired = C.TimeRequired;
        OtherC.IsFailure = false;
        E1.Correspondences[C.SourceSol] = C;
        E2.Correspondences[OtherC.SourceSol] = OtherC;
      } else if (FirstVal == "null") {
        Correspondence OtherC;
        OtherC.SourceSol = stoi(SecondVal);
        OtherC.IsFailure = true;
        OtherC.TimeRequired = TimeRequired;
        E2.Correspondences[OtherC.SourceSol] = OtherC;
      } else if (SecondVal == "null") {
        Correspondence C;
        C.SourceSol = stoi(FirstVal);
        C.IsFailure = true;
        C.TimeRequired = TimeRequired;
        E1.Correspondences[C.SourceSol] = C;  
      } else
        throw invalid_argument("Invalid input. A triple can't have two nulls.");

      OnFirstInt = false;
      OnSecondInt = false;
      FirstVal.clear();
      SecondVal.clear();
      ThirdVal.clear();
    } else if ((*it) == ',') {
      if (OnFirstInt) {
        OnFirstInt = false;
        OnSecondInt = true;
      } else if (OnSecondInt) {
        OnFirstInt = false;
        OnSecondInt = false;
      } else
        continue; // This is the , between triples
    } else {
      if (OnFirstInt)
        FirstVal += (*it);
      else if (OnSecondInt)
        SecondVal += (*it);
      else
        ThirdVal += (*it);
    };
  };
  
  E1.TrackerCount = 0;
  E1.SourceFailures = 0;
  E1.ExpectedFailures = 0;
  E1.ExpectedValue = 0;
  E1.NumberOfAttempts = 0;
  E1.ID = G->Edges.size();
  E1.SourceNodeID = N1->ID;
  E1.TargetNodeID = N2->ID;
  E1.OtherEdgeID = E1.ID + 1;
  E1.SuccessfulCorrespondences = 0;
  G->Edges.push_back(E1);

  E2.TrackerCount = 0;
  E2.SourceFailures = 0;
  E2.ExpectedFailures = 0;
  E2.ExpectedValue = 0;
  E2.NumberOfAttempts = 0;
  E2.ID = G->Edges.size();
  E2.SourceNodeID = N2->ID;
  E2.TargetNodeID = N1->ID;
  E2.OtherEdgeID = E2.ID - 1;
  E2.SuccessfulCorrespondences = 0;
  G->Edges.push_back(E2);
  
  N1->OutgoingEdgeIDs.insert(E1.ID);
  N2->OutgoingEdgeIDs.insert(E2.ID);
  N2->IncomingEdgeIDs.insert(E1.ID);
  N1->IncomingEdgeIDs.insert(E2.ID);
};

//------------------------------------------------------------------------------
HomotopyGraph InitializeEmptyGraphFromCompletedGraph(HomotopyGraph* CompletedG)
{
  HomotopyGraph G(CompletedG->RootCount);
  G.NumberOfCompleteNodes = 0;
  G.EVType = CompletedG->EVType;
  G.Alpha = CompletedG->Alpha;
  G.Lambda = CompletedG->Lambda;
  for (size_t i = 0; i != CompletedG->Nodes.size(); i++)
  {
    HomotopyNode NewNode(&G);
    G.Nodes.push_back(NewNode);
    for (size_t j = 0; j != G.Nodes[i].Solutions.size(); j++)
      G.Nodes[i].Solutions[j] = false;
    G.Nodes[i].OutgoingEdgeIDs = CompletedG->Nodes[i].OutgoingEdgeIDs;
    G.Nodes[i].IncomingEdgeIDs = CompletedG->Nodes[i].IncomingEdgeIDs;
    G.Nodes[i].ExpectedValue = 0;
  };
  for (size_t i = 0; i != CompletedG->Edges.size(); i++)
  {
    HomotopyDirectedEdge E;
    E.SourceNodeID = CompletedG->Edges[i].SourceNodeID;
    E.TargetNodeID = CompletedG->Edges[i].TargetNodeID;
    E.ID = CompletedG->Edges[i].ID;
    E.OtherEdgeID = CompletedG->Edges[i].OtherEdgeID;
    E.ExpectedValue = 0;
    E.NumberOfAttempts = 0;
    E.TrackerCount = 0;
    E.SuccessfulCorrespondences = 0;
    E.SourceFailures = 0;
    E.ExpectedFailures = 0;
    G.Edges.push_back(E);
  };

  uniform_int_distribution<int> NodeChooser(0,G.Nodes.size()-1);
  int NodeWithKnownSolution = NodeChooser(mt);
  uniform_int_distribution<int> SolutionChooser(0,G.RootCount-1);
  for (size_t i = 0; i != G.Nodes.size(); i++)
  {
    // Only one node knows a solution
    HomotopyNode* N = &(G.Nodes[i]);
    if (i == NodeWithKnownSolution)
    {
      N->SolutionCount = 1;
      int KnownSolution = SolutionChooser(mt);
      N->Solutions[KnownSolution] = 1;
      
      for (auto& e : N->OutgoingEdgeIDs)
        G.Edges[e].TrackableSolutions.insert(KnownSolution);
      
    } else 
    {
      N->SolutionCount = 0;
    };
  }

  G.ComputeEVOption = CompletedG->ComputeEVOption;
  G.UseOldEVs = CompletedG->UseOldEVs;

  for (size_t i = 0; i != G.Nodes.size(); i++)
  {
    if (G.UseOldEVs == true)
      ComputeExpectedValuesOLDWITHNOFAILURESONLY(&G, &G.Nodes[i]);
    else
      ComputeExpectedValues(&G, &G.Nodes[i]);
  };
  
  if (Verbose)
    cerr << "-------------expected values initialized------------" << endl;
  return G;
}

//------------------------------------------------------------------------------
HomotopyGraph InitializeGraphFromFile(string &FileName, int seed)
{
  ifstream infile(FileName);
  if(!infile.good())
    throw invalid_argument("Please input a valid filename.");

  mt.seed(seed);
  string str;
  size_t EqualsIndex;
  size_t SemicolonIndex;
  getline(infile, str); // Eat the first line.
  getline(infile, str); // Eat the second line with the system or the edge count.
  
  getline(infile, str); // This should be the line with root count.
  EqualsIndex = str.find("=");
  SemicolonIndex = str.find(";");
  HomotopyGraph G(stoi(str.substr(EqualsIndex+1,SemicolonIndex - (EqualsIndex + 1))));
  G.NumberOfCompleteNodes = G.Nodes.size();

  getline(infile, str); // This should be the line with node count.
  EqualsIndex = str.find("=");
  SemicolonIndex = str.find(";");
  int NodeCount = stoi(str.substr(EqualsIndex+1,SemicolonIndex - (EqualsIndex + 1)));
  
  for (size_t i = 0; i != NodeCount; i++)
  {
    HomotopyNode NewNode(&G);
    G.Nodes.push_back(NewNode);
    G.Nodes[i].SolutionCount = G.RootCount;
    for (size_t j = 0; j != G.Nodes[i].Solutions.size(); j++)
      G.Nodes[i].Solutions[j] = true;
  };
  
  while (getline(infile, str)) {
    // Snag the correct nodes
    size_t OpenParenIndex = str.find("(");
    size_t CloseParenIndex = str.find(")");
    size_t CommaIndex = str.find(",");
    int SourceNodeIndex = stoi(str.substr(OpenParenIndex+1,CommaIndex - (OpenParenIndex + 1)));
    int DestNodeIndex = stoi(str.substr(CommaIndex+1,CloseParenIndex - (CommaIndex + 1)));
    
    getline(infile, str);
    AddEdges(&G,&G.Nodes[SourceNodeIndex],&G.Nodes[DestNodeIndex], str);
  };

  return G;
};

//------------------------------------------------------------------------------
void PrintEdgeSelectionDetails(HomotopyGraph* G, HomotopyDirectedEdge* E, HomotopyNode* N, double Probability)
{
  cerr << "-- updating potential for edges directed towards node " << N->ID << endl;
  cerr << ">>> #Q: ";
  for (auto& v : G->Nodes)
    cerr << v.SolutionCount << " ";
  cerr << "  #C: ";
  for (auto& e : G->Edges)
    cerr << e.SuccessfulCorrespondences << " ";
  cerr << endl;
  cerr <<  "E.ID = " << E->ID << " from " << E->SourceNodeID << " to " << E->TargetNodeID << " w/ prob " << Probability << " : |Ce|, |E_v| = " << E->SuccessfulCorrespondences << " , " << N->ExpectedValue << endl;
};

//------------------------------------------------------------------------------
// Computes the expected values for all edges going to node N.
void ComputeExpectedValues(HomotopyGraph* G, HomotopyNode* N)
{
  map<int,double> EdgeIncrements; // potE for each edge
  N->ExpectedValue = N->SolutionCount; // that's it, when we're in serial
  
#ifdef DEBUGGING
  // debugging
  for (auto& i : N->InwardTaskCounts)
  {
    //cerr << i.first << " ID  w " << i.second << "count" << endl;
    if (i.second > 0)
      cerr << G->Edges[i.first].ID << " has an inward task that contributes prob " << 1- G->Alpha * (double)(i.second) / (G->RootCount - G->Edges[i.first].SuccessfulCorrespondences) << endl;
  }
#endif
  
  // a loop that updates the expected value at node N when G->Alpha==1 and the expected number of failures at each edge (these are dependent when G->Alpha < 1)
  for (auto& e : N->IncomingEdgeIDs)
  {
    HomotopyDirectedEdge& E = G->Edges[e];
    E.ExpectedFailures = E.SourceFailures + (1-G->Alpha)*E.TrackerCount;
    int Denominator = G->RootCount - E.SuccessfulCorrespondences;
    if (Denominator == 0 && G->Alpha == 1.0) // not a robust comparison for floats
    {
      // If the denominator is zero and there are no failures, this means that what is currently in progress
      // plus what is already in the graph _should_ finish the node.
      N->ExpectedValue = G->RootCount;
      break;
      }
    N->ExpectedValue += (E.TrackerCount) * (G->Alpha) * (double)(G->RootCount - N->ExpectedValue) / Denominator; // ie, zero in serial      
  }
  
  // a loop that updates potE at incoming edges
  for (auto& e : N->IncomingEdgeIDs)
  {
    HomotopyDirectedEdge& E = G->Edges[e];
    EdgeIncrements[E.ID] = (G->RootCount == E.SuccessfulCorrespondences + E.TrackerCount + E.SourceFailures) ?
    -1 : // this edge is dead and the denominator below is 0
      (G->Alpha) * (double)(G->RootCount - N->ExpectedValue) * parFailFactor(E.SourceFailures,G->RootCount,E.SuccessfulCorrespondences,E.TrackerCount,G->Alpha) / (G->RootCount - E.SuccessfulCorrespondences - E.TrackerCount - E.SourceFailures);
    if (EdgeIncrements[E.ID] != -1 && EdgeIncrements[E.ID] <0)
      //cerr << " first " << (double)(G->RootCount - N->ExpectedValue - E.ExpectedFailures) << " second " << (1 - E.ExpectedFailures / (double)(G->RootCount-E.SuccessfulCorrespondences)) << endl;
                                                               //cerr << E.ID << " has crsps, fails, node ev" << E.SuccessfulCorrespondences << " , " << E.SourceFailures << " , " << N->ExpectedValue << endl;
      abort(); // this clearly should not happen

    if (Verbose)
      PrintEdgeSelectionDetails(G,&E,N,EdgeIncrements[E.ID]);
  
    double pOriginal = EdgeIncrements[E.ID];
    double pWeightTowardCompleteNode = 1/(E.TargetNodeID+1.0); // pot-lex
    if (G->EVType=="Original")
      E.ExpectedValue = pOriginal;
    else if (G->EVType=="WeightTowardCompleteNode") // !!! should rename
      E.ExpectedValue = pWeightTowardCompleteNode;
    else if(G->EVType=="ConvexCombination") // !!! should rename
      E.ExpectedValue = pow((double)G->Nodes[E.TargetNodeID].SolutionCount/(double)G->RootCount,G->Lambda) * pOriginal; 
    else 
      cerr << "Invalid option in ComputeExpectedValues: " << G->EVType << endl;
  if (Verbose)
    cerr << "Edge with ID " << E.ID << " tracking from "<< E.SourceNodeID << " to " << E.TargetNodeID << " has E.V. = " << E.ExpectedValue << "\n";
  }
};

//------------------------------------------------------------------------------
// Computes the expected values for all edges going to node N.
void ComputeExpectedValuesOLDWITHNOFAILURESONLY(HomotopyGraph* G, HomotopyNode* N)
{
  map<int,double> EdgeIncrements; // potE for each edge
  N->ExpectedValue = N->SolutionCount; // that's it, when we're in serial

  if (G->Alpha < 1.0)
  {
    cout << "Error: ComputeExpectedValuesOLDWITHNOFAILURESONLY called with alpha <> 1" << endl;
    abort();
  };

#ifdef DEBUGGING
  // debugging
  for (auto& i : N->InwardTaskCounts)
    {
      //cerr << i.first << " ID  w " << i.second << "count" << endl;
      if (i.second > 0)
  cerr << G->Edges[i.first].ID << " has an inward task that contributes prob " << 1- G->Alpha * (double)(i.second) / (G->RootCount - G->Edges[i.first].SuccessfulCorrespondences) << endl;
    }
#endif
  
  // a loop that updates the expected value at node N
  for (auto& e : N->IncomingEdgeIDs)
  {
    HomotopyDirectedEdge& E = G->Edges[e];
    int Denominator = G->RootCount - E.SuccessfulCorrespondences;
    if (Denominator == 0)
    {
      // If the denominator is zero and there are no failures, this means that what is currently in progress
      // plus what is already in the graph _should_ finish the node.
      N->ExpectedValue = G->RootCount;
      break;
    }
    N->ExpectedValue += (E.TrackerCount) * ((double)(G->RootCount - N->ExpectedValue))/Denominator; // ie, zero in serial
  }      
  
  // a loop that updates potE at incoming edges, plus N->ExpectedValue when G->Alpha <1 (even though not currently used)
  for (auto& e : N->IncomingEdgeIDs)
  {
    HomotopyDirectedEdge& E = G->Edges[e];
    EdgeIncrements[E.ID] = (G->RootCount == E.SuccessfulCorrespondences + E.TrackerCount) ?
      -1 : // this edge is dead and the denominator below is 0
      (double)(G->RootCount - N->ExpectedValue) / (double)(G->RootCount - E.SuccessfulCorrespondences - E.TrackerCount);
    if (EdgeIncrements[E.ID] > 1)
      abort();
      
    if (Verbose)
      PrintEdgeSelectionDetails(G,&E,N,EdgeIncrements[E.ID]);
    
    double pOriginal = EdgeIncrements[E.ID];
    double pWeightTowardCompleteNode = 1/(E.TargetNodeID+1.0); // pot-lex
    if (G->EVType=="Original")
      E.ExpectedValue = pOriginal;
    else if (G->EVType=="WeightTowardCompleteNode") // !!! should rename
      E.ExpectedValue = pWeightTowardCompleteNode;
    else if(G->EVType=="ConvexCombination") // !!! should rename
      E.ExpectedValue = pow((double)G->Nodes[E.TargetNodeID].SolutionCount/(double)G->RootCount,G->Lambda) * pOriginal; 
    else 
      cerr << "Invalid option in ComputeExpectedValues: " << G->EVType << endl;
    if (Verbose)
      cerr << "Edge with ID " << E.ID << " tracking from "<< E.SourceNodeID << " to " << E.TargetNodeID << " has E.V. = " << E.ExpectedValue << "\n";
  }
}
