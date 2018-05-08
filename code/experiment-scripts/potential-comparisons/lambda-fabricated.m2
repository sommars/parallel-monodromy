codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
nodeCount = 5
edgeCount = 1
lambdas=apply(toList(0..49),x-> x*0.1)
meshSize=length(lambdas)
iters=10
dataFileName = rootCount|"fabricated.csv";
setRandomSeed 0;
M=new MutableList from meshSize:0;
N=new MutableList from meshSize:0;
for j from 0 to (iters-1) do(
	filename = "temp.graph";
	outfilename = "temp.graph.out";
	createFakeGraph(rootCount,nodeCount,edgeCount,filename);
	for i from 0 to (meshSize-1) do(
	    print lambdas#i;
			-- print (codeDirectory|"pmonodromy "|filename| " -s 0 -t 1 -e ConvexCombination -l " | toString lambdas#i | " > " |outfilename);
	    run(codeDirectory|"pmonodromy "|filename| " -s 0 -t 1 -e ConvexCombination -l " | toString lambdas#i | " > " |outfilename);
	    l=(lines get outfilename);
	    M#i = M#i + value substring(l#-5,17);
	    N#i = N#i + value substring(l#-6,11);
	    );
	file  = openOut (dataFileName);
	file << "lambda,ntracks,tottime" << endl;
	for i from 0 to (meshSize-1) do (
	    file << toString(lambdas#i) << "," << toString(sub(M#i/iters,RR)) << "," << toString(sub(N#i/iters,RR)) << endl;
	    );
	close file;
	);
end ---------------------------------------------

restart
rootCount = 70
elapsedTime load "lambda-fabricated.m2"
rootCount = 924
elapsedTime load "lambda-fabricated.m2"
rootCount = 11016
elapsedTime load "lambda-fabricated.m2"
