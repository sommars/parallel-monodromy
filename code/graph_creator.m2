needsPackage "MonodromySolver";

createRealGraphM2timingsWithNoOverheadPart1 = method(Options=>{lambda=> 1})
createRealGraphM2timingsWithNoOverheadPart1 (PolySystem, ZZ, ZZ, String) := o -> (P,NodeCount,EdgeCount,filename) -> (
	NAGtrace 0;
	(V,npaths) = monodromySolve (P, EdgesSaturated => true, NumberOfNodes => NodeCount,
	    NumberOfEdges => EdgeCount, GraphInitFunction => completeGraphInit);

	NAGtrace 2;
	basePointToIndexMap = new HashTable from (for i from 0 to #V#Graph#Vertices - 1 list ({V#Graph#Vertices#(i)#BasePoint,i}));
	systemList = for i from 0 to #V#Graph#Vertices - 1 list (specializeSystem(V#Graph#Vertices#i#BasePoint,P));

	rootCount = length(V#PartialSols);
	for node in V#Graph#Vertices do
	if length(node#PartialSols) > rootCount then
	error "Not all nodes have the same number of known solutions.";
	
	R := ring P;	
	outFile = openOut filename;
	outFile << toString(coefficientRing coefficientRing R) << " " << toString(gens coefficientRing R) << " " << toString(gens R) << endl
	<< toExternalString(P) << endl
	<< "Root count = " << rootCount << ";" << endl
	<< "Node count = " << #(V#Graph#Vertices) << ";" << endl

	for i from 0 to #(V#Graph#Edges) - 1 do (
	    startSystemIndex = basePointToIndexMap#(V#Graph#Edges#i#Node1#BasePoint);
	    targetSystemIndex = basePointToIndexMap#(V#Graph#Edges#i#Node2#BasePoint);
	    
	    --Print the nodes being connected.
	    outFile << "(" << startSystemIndex << "," << targetSystemIndex << ");" << endl;
	    
	    foundIndices = new MutableList from for i from 1 to #(V#Graph#Edges#i#Correspondence12) list false;
	    for j from 0 to #(V#Graph#Edges#i#Correspondence12) - 1 do (
		startSols = {V#Graph#Vertices#startSystemIndex#PartialSols#j};
		trackTime = elapsedTiming trackHomotopy(V#Graph#Edges#i#"homotopy12",startSols);
		outFile << "{" << j << "," << V#Graph#Edges#i#Correspondence12#j << "," 
		<< "XXX" << "}";
		if (V#Graph#Edges#i#Correspondence12#j =!= null) then
		foundIndices#(V#Graph#Edges#i#Correspondence12#j) = true;
		if (j != #(V#Graph#Edges#i#Correspondence12) - 1) then outFile << ",\n";
		);
    	    if #foundIndices < 0.95 * rootCount then (
		NAGtrace 0;
		error "the rate of failures is higher than 95%";
		); 	    
	    for j from 0 to #foundIndices - 1 do (
		if not foundIndices#j then (
		    startSols = {V#Graph#Vertices#targetSystemIndex#PartialSols#j};
		    trackTime = elapsedTiming trackHomotopy(V#Graph#Edges#i#"homotopy21",startSols);
		    outFile << ",\n{null," << j << "," << "XXX" << "}";
		    );
		);
	    outFile << ";" << endl;
	    );
	outFile << close;
	NAGtrace 0;
);

createRealGraphM2timingsWithNoOverheadPart2 = method()
createRealGraphM2timingsWithNoOverheadPart2 (String, String) := (filename,logfilename) -> (
    aa := lines get filename; 
    bb := select(lines get logfilename, s->match("-- time of solveLinear calls", s) or match("-- time of evaluate calls", s)); 
    assert(#select(aa,s->match("XXX",s))==#bb//2);
    i := 0;
    apply(aa, a->if match("XXX",a) then (
	    t1 := value replace("[^0-9]", "", bb#(2*i));
	    t2 := value replace("[^0-9]", "", bb#(2*i+1));
	    t := toString (1e-9 * (t1+t2));
	    i = i+1;
	    replace("XXX", t, a)
	    ) else a
	)
    )

createRealGraph = method()
createRealGraph (PolySystem, ZZ, ZZ, String) := (P,NodeCount,EdgeCount,filename) -> (
    logfilename := filename | ".log";
    run ("M2 &>\""|logfilename|"\" <<!\npath = "|toExternalString path|"\nload \"graph_creator.m2\"\nR = "| toExternalString ring P |"\n"|  
    	"createRealGraphM2timingsWithNoOverheadPart1(" | toExternalString P |","|NodeCount|","|EdgeCount|","|toExternalString filename|");\n!\n"
    	);
    ll := createRealGraphM2timingsWithNoOverheadPart2(filename,logfilename);
    outFile = openOut filename;
    scan(ll, l -> outFile << l << if last l == "," then "" else endl);
    close outFile;	
    )

sampleNegativeBinomialDistribution = method(Options=>{OnlyFailure => false})
sampleNegativeBinomialDistribution (ZZ,RR) := o ->  (n, q) -> (
    if (q < 0) or (q > 1) then
        error "Parameter q must be between 0 and 1";
    if (n <= 0) then
        error "Parameter n must be a positive integer";
    successCount := 0;
    trialCount := 0;
    while successCount < n do (
        trialCount = trialCount + 1;
        if (random RR > q) then
            successCount = successCount + 1;
    );
	if o.OnlyFailure then trialCount = trialCount -n;
    return trialCount
)

createFakeGraph = method(Options=>{alpha => 1, n=>10, q=>0.3, OnlyFailure=>false, UseUniformRandom=>false, lambda=>1})
createFakeGraph (ZZ, ZZ, ZZ, String) := o -> (RootCount,NodeCount,EdgeCount,filename) -> (
	outFile = openOut filename;
	outFile << "Random graph." << endl 
	<< "Edge count = " << EdgeCount << ";" << endl
	<< "Root count = " << RootCount << ";" << endl
	<< "Node count = " << NodeCount << ";" << endl
	
	failCount = 0;
	corrCount = 0;

	Roots = for i from 0 to RootCount - 1 list i;
	for i from 0 to NodeCount - 1 do
	(
		for j from i + 1 to NodeCount - 1 do
		(
			for k from 1 to EdgeCount do
			(
				RandomRoots = random Roots;
				outFile << "(" << i << "," << j << ");" << endl;
				for l from 0 to #RandomRoots - 1 do
				(
					corrCount = corrCount + 1;
					-- This is where we simulate path failures.
					if (random RR > o.alpha) then
					(
						failCount = failCount + 1;
						outFile << "{" << l << ",null,"<< "0" << "}";
						outFile << "{null," << RandomRoots#l << ","<< "0" << "}";
					) else if (o.UseUniformRandom) then
					(
						outFile << "{" << l << "," << RandomRoots#l << "," << random RR << "}"
					) else
						outFile << "{" << l << "," << RandomRoots#l << "," << sampleNegativeBinomialDistribution(o.n, o.q, OnlyFailure => o.OnlyFailure) << "}";
					if l != #RandomRoots - 1 then outFile << ",";
				);
				outFile << ";" << endl;
			);
		);
	);
	outFile << close;
	-- << "actual alpha = " << 1.0 * (corrCount - failCount) / corrCount << endl;
);

extractTime = method()
extractTime (String) := (filename) -> (
	dataStr = get filename;
	dataStr = replace("\n\\{", "\n{{", dataStr);
	dataStr = replace("};", "}}", dataStr);
	dataStrLines = lines(dataStr);
		
	numberOfHeaderLines := 6; 
	numCorr := lift((#dataStrLines - numberOfHeaderLines)/2,ZZ);
	timeList = new MutableList; 
	
	for i from 0 to numCorr-1 do (
		corrList = value(dataStrLines#(numberOfHeaderLines+2*i+1));
		start := #timeList;
		timeList#(start+#corrList-1) = 0;
		scan(#corrList,j->(
			corr = corrList#j,			
			timeList#(start+j) = if (corr#0 === null) or (corr#1 === null) then -corr#2 else corr#2;
		));
	    );
	
	(
	    sort toList select(timeList,t->t>=0), 
	    -sort toList select(timeList,t->t<0)
	    )
	);

convertToFailures = method()
convertToFailures (String, String, RR) := (inFile, outFile, alpha) -> (
    (successTimeList,failureTimeList) := extractTime inFile;
    cutoff := successTimeList # (round(alpha*(#successTimeList+#failureTimeList))-1);
    dataStr = get inFile;
    numberOfHeaderLines := 6; 
    ss := lines(dataStr);
    g := openOut outFile;
    scan(take(ss,6), s-> g << s << newline); 
    ss = drop(ss,6);
    numCorr := lift(#ss/2,ZZ);
    for i from 0 to numCorr-1 do (
	g << ss#(2*i) << newline;
    	s := ss#(2*i+1);
	s = replace("\\{", "{{", s);
	s = replace(",\\{\\{", ",{", s);
    	s = replace("};", "}}", s);
	corrList := apply(value s, corr->		
	    if corr#0 === null then toString {"null", corr#1, corr#2} 
	    else if corr#1 === null then toString{corr#0, "null", corr#2}
	    else if corr#2 < cutoff then corr 
	    else toString{corr#0, "null", corr#2} |", "|toString{"null", corr#1, corr#2}   
	    );
	s = toString corrList;
	s = replace("\\{\\{", "{", s);
	s = replace("\\}\\}", "};", s);
	s = replace(" ","",s);
	g << s << endl;
	);
    g << close;
    )
--!!! this method should be in MonodromySolver or ExampleSystems !!!
sparseFamily = method()
sparseFamily PolySystem := PS-> (
    polys := flatten entries PS.PolyMap;                                                                   
    ind := flatten apply(#polys,i-> -- indices for parameters                                              
    	apply(exponents polys#i, t->(i,t))                                                                 
    	);                                                                                                 
    R := PS.PolyMap.ring;                                                                                  
    W := symbol W;                                                                                         
    AR := CC[apply(ind,i->W_i)][gens R];                                                                   
    polysP := for i to #polys-1 list -- system with parameteric coefficients and same support              
    sum(exponents polys#i, t->W_(i,t)*AR_(t));                                                             
    polySystem transpose matrix {polysP}
    )  

end

--R = CC[x_1..x_22][a,b,c,d,e];
--P = polySystem {x_1*a+x_2*b+x_3*c+x_4*d+x_5*e,x_6*a*b+x_7*b*c+x_8*c*d+x_9*a*e+x_10*d*e,x_11*a*b*c+x_12*b*c*d+x_13*a*b*e+x_14*a*d*e+x_15*c*d*e,x_16*a*b*c*d+x_17*a*b*c*e+x_18*a*b*d*e+x_19*a*c*d*e+x_20*b*c*d*e,x_21*a*b*c*d*e-x_22*1};

--(successTimeList,failureTimeList) = extractTime("../graphs/reimer5.1e-8.graph")

