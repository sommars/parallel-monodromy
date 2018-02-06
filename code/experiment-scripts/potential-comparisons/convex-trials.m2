{* testing potential ConvexCombination:
    (1-lambda) potE* + lambda potE  
need to have output from convert2failures 
*}

restart
updateLambda = (filename,lambda) -> run ("./lambda-replace.sh 6 " | filename | " " |  toString(lambda))
-- needs (currentDirectory() | "convex_trials.m2")
codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
needs(codeDirectory | "graph-examples/cyclic.m2")
outputDirectory = currentFileDirectory|"../../../experiments/"
rootCount=70
nodeCount=5
lambdas=apply(toList(0..60),x-> sub(x,RR)/20)
meshSize=length(lambdas)
edgeCount = 5
-- numVars = 5
-- P = sparseFamily polySystem cyclic(numVars,QQ)


iters=10--might need more to reduce variance

setRandomSeed 0;
scan({"fabricated","real"}, opt-> (
	filename = "temp.graph";
	for j from 10 to 10 do (
	    a:=sub(j/10,RR);
	    outfilename = "temp.out";
	    if opt == "real" then (
		dataFileName = j|"cyclic-5.csv";
		)
	    else (
		dataFileName = j|"fabricated.csv";
		);
	    M=new MutableList from meshSize:0;
	    N=new MutableList from meshSize:0;
    	    elapsedTime scan(iters, it->(
		    if opt == "real" then (
			--createRealGraph(P,nodeCount,edgeCount,filename); -- ???
			convertToFailures("../../graph-examples/cyclic/cyclic-5-(N=5)-(m=1)-(seed=0).graph",filename, a);
			)
		    else (
			createFakeGraph(rootCount,nodeCount,edgeCount,filename,alpha=>a,n=>5,q=>0.3,OnlyFailure=>true);
			);
		    for i from 0 to (meshSize-1) do(
			updateLambda(filename,lambdas#i);
			run(codeDirectory|"pmonodromy "|filename|" 1 ConvexCombination > " |outfilename);
			l=(lines get outfilename);
			M#i = M#i + value substring(l#-5,17);
			N#i = N#i + value substring(l#-6,11);
			);
		    ));
    	    file  = openOut (outputDirectory | dataFileName);
    	    file << "lambda,ntracks,tottime" << endl;
    	    for i from 0 to (meshSize-1) do (
		file << toString(lambdas#i) << "," << toString(sub(M#i/iters,RR)) << "," << toString(sub(N#i/iters,RR)) << endl;
		);
    	    close file;
    	    );
	));

end ---------------------------------------------

restart
elapsedTime load "convex-trials.m2"

