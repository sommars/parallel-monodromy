path = path | {currentFileDirectory|"../"}
needs "graph_creator.m2"
needsPackage "ExampleSystems"

average = x -> if #x > 0 then toRR sum x / #x else -infinity 
end ----------------------------------------------------------


----------------------------
-- generate "old" cyclic files
restart
load "cyclic.m2"

n = 5 -- set your n 
-- max done so far is n=10: there are some outliers (large timings) due to garbage collection

P = sparseFamily polySystem cyclic(n,QQ)

nodeCount = 3
edgeCount = 2

setRandomSeed 0
graphFile = currentFileDirectory|"cyclic-"|n|".graph"
elapsedTime createRealGraph(P,NodeCount,EdgeCount,graphFile)

------------------------------
-- analyze times
tt = elapsedTime (successTimeList,failureTimeList) = extractTime graphFile;
#failureTimeList

-- Look for timings over 1s. This is probably due to GC.
t1 = tt / (t->select(t,a->a<1.)); -- those that take < 1s  
t1 / average 

tt/length
t1/length

run ("ls -l "|currentFileDirectory|"cyclic-*")

---------------------------------------
-- generate cyclic/*

restart
load "cyclic.m2"

makeDirectory "cyclic"
-- reset all these according to your needs
for edgeCount from 1 to 1 do
for n from 5 to 9 do
for nodeCount from 5 to 9 do 
for seed from 0 to 4 do (
    P := sparseFamily polySystem cyclic(n,QQ);
    setRandomSeed seed;
    graphFile := currentFileDirectory|"cyclic/cyclic-"|n|
    "-(N="|nodeCount|")-(m="|edgeCount|")-(seed="|seed|").graph";
    print graphFile;
    elapsedTime 
    if not fileExists graphFile then 
    try createRealGraph(P,nodeCount,edgeCount,graphFile);
    {* if fileExists graphFile then CHECK that there are not too many failures *}
    )

