codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
filename = "random.graph"
outfilename = filename|".out"
simulate = (rootCount,edgeCount,nodeCount,a,iters) -> ( --no more arguments please
	if a>= 1 then (
		print("WARNING:not a valid failure probability");
		return("NA"); -- kludge for easier table formatting
		);
	tot:=0;
	for i from 0 to (iters-1) do(
		createFakeGraph(rootCount,nodeCount,edgeCount,filename,alpha=>a);
		run (codeDirectory|"pmonodromy "|filename|" 4 WeightTowardCompleteNode > "|outfilename);
		output:=last lines get outfilename;
		percentage:=replace("[^0-9]", "", output);
		print percentage; -- verbose
		tot=tot+value percentage;
		);
	(tot/iters,a)
	)
end


-- START HERE, run M2 from directory containing this file
-- will generate four tables
restart
needs (currentDirectory() | "trials.m2")
codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
filename = "random.graph"
outfilename = filename|".out"

-- change these to make more impressive tables
rootCounts={50,100,200}
iters=10
-- more than five edgeCounts will make tables spill over
edgeCounts = {5,10,20,50,100}
emax = max edgeCounts
outputDirectory = currentFileDirectory|"../../../experiments/"


setRandomSeed 0
--table for above threshold
file = openOut (outputDirectory | "aboveThreshold" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for e in edgeCounts do file << "c|"
file << "}" << endl
file << "RootCount"
for e in edgeCounts do file << "&" << "$\\alpha (" << e << ")$ , \\% comp."
file << "\\\\" << endl 
file << "\\hline" <<endl 
for rootCount in rootCounts do(
	print("now entering a new root count == " | rootCount);
	file << rootCount << "&";
	-- fix a 'moderate' node count for each rootCount
	nodeCount=floor(log(rootCount));
	for edgeCount in edgeCounts do (
		a=sub(log(rootCount)/edgeCount,RR);
		elapsedTime x=simulate(rootCount,edgeCount,nodeCount,a,iters);
		if member(class(x),ancestors(Sequence)) then file << last x << ", " << first x << "\\%"; --better way to implement conditional?
		if (edgeCount != emax) then file << "&";
		);
	file << "\\\\" << endl;
	);
file << "\\end{tabular}" << endl
file << "\\caption{Percentage of successful runs using $\\alpha (\\textrm{EdgeCount}) = \\log (\\textrm{RootCount})/\\textrm{EdgeCount}$ as the tracking success rate on a graph with $\\lfloor \\log \\text{RootCount} \\rfloor $ nodes ("
file << iters << " runs for each cell.)}" << endl
file << "\\end{table}"
close file 

-- table for near threshold
file = openOut (outputDirectory | "nearThreshold" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for e in edgeCounts do file << "c|"
file << "}" << endl
file << "RootCount"
for e in edgeCounts do file << "&" << "$\\alpha (" << e << ")$ , \\% comp."
file << "\\\\" << endl 
file << "\\hline" <<endl 
for rootCount in rootCounts do(
	file << rootCount << "&";
	-- fix a 'moderate' node count for each rootCount
	nodeCount=floor(log(rootCount));
	for edgeCount in edgeCounts do (
		a=sub(log(rootCount)^(1/4)/(edgeCount),RR);
		x=simulate(rootCount,edgeCount,nodeCount,a,iters);
		if member(class(x),ancestors(Sequence)) then file << last x << ", " << first x << "\\%";
		if (edgeCount != emax) then file << "&";
		);
	file << "\\\\" << endl;
	);
file << "\\end{tabular}" << endl
file << "\\caption{Percentage of successful runs using $\\alpha (\\textrm{EdgeCount}) = \\log^{1/4} (\\textrm{RootCount})/\\textrm{EdgeCount}$ as the tracking success rate on a graph with $\\lfloor \\log \\text{RootCount} \rfloor $ nodes ("
file << iters << " runs for each cell.)}" << endl
file << "\\end{table}"
close file 



{*
Other threshold-related experiments

-- table for anotherNear threshold
file = openOut (outputDirectory | "anotherNearThreshold" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for e in edgeCounts do file << "c|"
file << "}" << endl
file << "RootCount"
for e in edgeCounts do file << "&" << "$\\alpha (" << e << ")$ , \\% comp."
file << "\\\\" << endl 
file << "\\hline" <<endl 
for rootCount in rootCounts do(
	file << rootCount << "&";
	-- fix a 'moderate' node count for each rootCount
	nodeCount=floor(log(rootCount));
	for edgeCount in edgeCounts do (
		a=sub(log(nodeCount)/(edgeCount),RR);
		x=simulate(rootCount,edgeCount,nodeCount,a,iters);
		if member(class(x),ancestors(Sequence)) then file << last x << ", " << first x << "\\%";
		if (edgeCount != emax) then file << "&";
		);
	file << "\\\\" << endl;
	);
file << "\\end{tabular}" << endl
file << "\\caption{Percentage of successful runs using $\\alpha (\\textrm{EdgeCount}) = \\log (\\textrm{NodeCount})/\\textrm{EdgeCount}$ as the tracking success rate on a graph with $\\lfloor \\log \\text{RootCount} \rfloor $ nodes ("
file << iters << " runs for each cell.)}" << endl
file << "\\end{table}"
close file 



-- table for at lower threshold
file = openOut (outputDirectory | "belowThreshold" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for e in edgeCounts do file << "c|"
file << "}" << endl
file << "RootCount"
for e in edgeCounts do file << "&" << "$\\alpha (" << e << ")$ , \\% comp."
file << "\\\\" << endl 
file << "\\hline" <<endl 
for rootCount in rootCounts do(
	file << rootCount << "&";
	-- fix a 'moderate' node count for each rootCount
	nodeCount=floor(log(rootCount));
	for edgeCount in edgeCounts do (
		a=sub(1/(edgeCount*nodeCount),RR);
		x=simulate(rootCount,edgeCount,nodeCount,a,iters);
		if member(class(x),ancestors(Sequence)) then file << last x << ", " << first x << "\\%";
		if (edgeCount != emax) then file << "&";
		);
	file << "\\\\" << endl;
	);
file << "\\end{tabular}" << endl
file << "\\caption{Percentage of successful runs using $\\alpha (\\textrm{EdgeCount}) = 1/(\\textrm{EdgeCount} \\times \\log (\\textrm{RootCount})) \\rfloor )$ as the tracking success rate on a graph with $\\lfloor \\log \\textrm{RootCount} \\rfloor $ nodes ("
file << iters << " runs for each cell.)}" << endl
file << "\\end{table}"
close file
*}