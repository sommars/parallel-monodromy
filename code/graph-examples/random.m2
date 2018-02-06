needs (currentFileDirectory|"../graph_creator.m2")

NodeCount = 3
EdgeCount = 2
RootCount = 100

setRandomSeed 0
createFakeGraph(RootCount,NodeCount,EdgeCount,"random.graph",alpha=>1);
-- createFakeGraph(RootCount,NodeCount,EdgeCount,"random.graph",alpha=>0.9);
