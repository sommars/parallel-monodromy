-- testing weight-potential lambda-family

restart
updateLambda = (filename,lambda) -> run ("./lambda-replace.sh 6 " | filename | " " |  toString(lambda))
-- needs (currentDirectory() | "convex_trials.m2")
codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
outputDirectory = currentFileDirectory|"../../../experiments/"
lambdas=apply(toList(0..30),x-> sub(x,RR)/10)
meshSize=length(lambdas)

iters=10--might need more to reduce variance

setRandomSeed 0;
exampleFiles = {"../../graph-examples/cyclic/cyclic-5-(N=5)-(m=1)-(seed=0).graph",
	"../../graph-examples/cyclic/cyclic-7-(N=5)-(m=1)-(seed=0).graph"}
dataFiles = {"cyclic-5.csv", "cyclic-7.csv"}
scan(#exampleFiles, 
    exampleN-> (
	example := exampleFiles#exampleN;
	filename := "temp.graph";
	a = 1_RR;
	outfilename := "temp.out";
	dataFileName := dataFiles#exampleN;
	M := new MutableList from meshSize:0;
	N := new MutableList from meshSize:0;
	elapsedTime scan(iters, it->(
		convertToFailures(example,filename,a);
		for i from 0 to (meshSize-1) do(
		    updateLambda(filename,lambdas#i);
		    run(codeDirectory|"pmonodromy "|filename|" 1 ConvexCombination > " |outfilename);  -- FAILS FOR 10 THREADS!!!
		    l=(lines get outfilename);
		    M#i = M#i + value substring(l#-5,17);
		    N#i = N#i + value substring(l#-6,11);
		    );
		));
	file = openOut (dataFileName);
	file << "lambda,ntracks,tottime" << endl;
	for i from 0 to (meshSize-1) do (
	    file << toString(lambdas#i) << "," << toString(sub(M#i/iters,RR)) << "," << toString(sub(N#i/iters,RR)) << endl;
	    );
	close file;
	));

end ---------------------------------------------

restart
elapsedTime load "lambda-trials.m2"

