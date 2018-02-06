codeDirectory = currentFileDirectory|"../../"
needs (codeDirectory|"graph_creator.m2")
tempfilename := "temp.graph";
    
searchAlpha = (RootCount,NodeCount,EdgeCount,nSeeds,prec,filename,outfilename) ->
    for r from 1 to nSeeds list (
    A := 0.; B := 1.; 
    while B-A > prec do (
    	a := (B+A)/2;
        --
    	-- step 1
    	setRandomSeed r;
    	if filename == "random.graph"
	then createFakeGraph(RootCount,NodeCount,EdgeCount,tempfilename,alpha=>a)
	else convertToFailures(filename,tempfilename,a);
    	--
        -- step 2
    	run (codeDirectory|"pmonodromy "|tempfilename|" 1 > "|outfilename);
	--
    	-- step 3
    	output := last lines get outfilename;
    	percentage := replace("[^0-9]", "", output); 
    	success := (value percentage > 0);
      	--  
    	if success then B = a else A = a;
    	);
    (A+B)/2
    )

end ----------------------------
restart
load "alpha-threshold.m2"
prec = 0.0001;
nSeeds = 10
NodeCount = 2
EdgeCount = 3
RootCount = 100
filename = "random.graph"
outfilename = "temp.out"
results = searchAlpha(RootCount,NodeCount,EdgeCount,nSeeds,prec,filename,outfilename) 

filename = "../../graph-examples/cyclic-5.graph"
results = searchAlpha(RootCount,NodeCount,EdgeCount,nSeeds,prec,filename,outfilename) 

sum results / length results

