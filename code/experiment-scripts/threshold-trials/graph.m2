iterations=1

simulate = (a,inFilename,rootCount,nodeCount,e) -> ( --no more arguments please
	if a> 1 then (
		print("WARNING:not a valid failure probability");
		return("NA"); -- kludge for easier table formatting
		);
	tot:=0;
	if a<1 then (
		simInFile="fail.graph";
		createFakeGraph(rootCount,nodeCount,e,simInFile,alpha=>a);
		)
	else simInFile=inFilename;
	outFilename = simInFile | ".out";
	for i from 0 to (iterations -1) do (
		run (codeDirectory|"pmonodromy "|simInFile|" > " | outFilename);
		l=(lines get outFilename);
		output:=last l;
		percentage:=replace("[^0-9]", "", output);
		print percentage; -- verbose
		ntracks =  value substring(l#-5,17);
		totaltime = value substring(l#-6,11);
		tot=tot+value percentage;
		);
	(ntracks,totaltime,tot/iterations)
	)

codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
outputDirectory = currentFileDirectory|"../../../experiments/"
rootCount=1000
nodeCount=3
alphas=apply(0..100,i->sub(i/100,RR))
edgeCounts = {2,3,5,10,25,50}

setRandomSeed 0
for e in edgeCounts do(
	i=0;
	nsuccesses=0;
	noFailureFile = "plot" | e | ".graph";
	outfilename = noFailureFile|".out";
	--table for above threshold
	file = openOut (outputDirectory | e | "edges.csv");
	file << "ntracks,tottime,suc,alpha" << endl;
	createFakeGraph(rootCount,nodeCount,e,noFailureFile);
	acc=0;
	counter=0;
	for a in reverse alphas do (
		t=elapsedTime simulate(a,noFailureFile,rootCount,nodeCount,e);
		counter = counter +1;
		acc = acc + t#2;
		print(toString(acc/sub(counter,RR)) | " % of trials succeeded ");
		if t#2 >= iterations/2 then color = 2
		else if t#2 >= iterations/4 then color =1
		  else color=0;
		print color;
		print(e | " edges, " |  i |  "% done w/ current trial \n");
		print t;
		i=i+1;
		file << first t << "," << t#1 << "," << color << "," << toString(a) << endl;
		);
	close file 
	)

end

restart
load "graph.m2"

