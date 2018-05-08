codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
needs(codeDirectory | "graph-examples/cyclic.m2")
outputDirectory = currentFileDirectory|"../../../experiments/"
rootCount=1000
nodeCount=3
edgeCount = 3
lambdas=apply(toList(0..60),x-> sub(x,RR)/600)
meshSize=length(lambdas)
iters=100
dataFileName = rootCount|"fabricated2.csv";
setRandomSeed 0;
M=new MutableList from meshSize:0;
N=new MutableList from meshSize:0;
for j from 0 to (iters-1) do(
	filename = "temp.graph";
	outfilename = "temp.graph.out";
	createFakeGraph(rootCount,nodeCount,edgeCount,filename);
	for i from 0 to (meshSize-1) do(
	    print lambdas#i;
	    run(codeDirectory|"pmonodromy "|filename|" -l " | toString lambdas#i | " -t 1 -e ConvexCombination > " |outfilename);
	    l=(lines get outfilename);
	    M#i = M#i + value substring(l#-5,17);
	    N#i = N#i + value substring(l#-6,11);
	    );
	file  = openOut (outputDirectory | dataFileName);
	file << "lambda,ntracks,tottime" << endl;
	for i from 0 to (meshSize-1) do (
	    file << toString(lambdas#i) << "," << toString(sub(M#i/iters,RR)) << "," << toString(sub(N#i/iters,RR)) << endl;
	    );
	close file;
	);
end ---------------------------------------------

restart
elapsedTime load "convex-trials.m2"
