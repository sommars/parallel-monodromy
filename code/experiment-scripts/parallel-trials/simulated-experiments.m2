needs (currentDirectory() | "../../graph_creator.m2");
outputDirectory = currentDirectory()|"../../../experiments/"

RootCounts = {100,500,1000,5000,10000};
ThreadCounts = {1,2,4,8,16,32,64,128}
NodeCount = 3;
EdgeCount = 2;

TrialCount = 30;
RunTimes = {}

for RootCount in RootCounts do (
    for RandomSeed from 0 to TrialCount do (
        setRandomSeed RandomSeed;
        filename = RootCount | "Solution" | RandomSeed | "Seedfakegraph.graph";
        createFakeGraph(RootCount, NodeCount, EdgeCount, filename,UseUniformRandom=>false);
    );
);

for RootCount in RootCounts do (
    RunTimesForColumn = {};
    for ThreadCount in ThreadCounts do (
        TempValue = 0;
        for RandomSeed from 0 to TrialCount do (
            filename = RootCount | "Solution" | RandomSeed | "Seedfakegraph.graph";
            outfilename = filename | ThreadCount | "Threads.out";
            run ("../../pmonodromy " | filename | " -t " | ThreadCount | " -e Original -s 1 -s 2 -s 3 -s 4 -s 5 -s 6 -s 7 -s 8 -s 9 -s 10 > " | outfilename);
            TempValue = TempValue + value(substring((lines get outfilename)#-6,11));
            run("rm " | outfilename);
        );
        RunTimesForColumn = append(RunTimesForColumn, TempValue);
    );
    RunTimes = append(RunTimes, RunTimesForColumn);
);


file = openOut (outputDirectory | "parallelSimulatedTrial" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for e in RootCounts do file << "c|"
file << "}" << endl
file << "\\#Solutions"
for RootCount in RootCounts do
    file << " & " << RootCount;

file << "\\\\ \\hline" << endl;

for i from 0 to #ThreadCounts - 1 do (
    file << ThreadCounts#i;
    for j from 0 to #RootCounts - 1 do (
        file << " & " << round(2, RunTimes#j#0 / RunTimes#j#i / ThreadCounts#i * 100) << "\\%";
    );
    file << "\\\\" << endl;
);
file << "\\end{tabular}" << endl;
file << "\\caption{Efficiency for simulated polynomial systems with varied numbers of solutions.}" << endl;
file << "\\label{FakeParallelEfficiency}" << endl;
file << "\\end{table}"
close file
 
end
