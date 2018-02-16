nList = {5,6,7,8,9,10};
ThreadCounts = {1,2,4,8,16,32,64,128};
outputDirectory = currentDirectory()|"../../../experiments/"

TrialCount = 30;
RunTimes = {}
RandomSeeds = ""
for i from 1 to TrialCount do
    RandomSeeds = RandomSeeds | " -s " | i

for n in nList do (
    basefilename = "cyclic-" | n | ".graph";
    filename = "../../graph-examples/" | basefilename;
    RunTimesForColumn = {};
    for ThreadCount in ThreadCounts do (
        outfilename = basefilename|"-" | ThreadCount | "Threads.out";
        run ("../../pmonodromy " | filename | " -t " | ThreadCount | "-e  Original " | RandomSeeds | " > " | outfilename);
        RunTimesForColumn = append(RunTimesForColumn, value(substring((lines get outfilename)#-6,11)));
        run("rm " | outfilename)
    );
    RunTimes = append(RunTimes, RunTimesForColumn);
);

file = openOut (outputDirectory | "parallelCyclicTrial" | ".tex")
file << "\\begin{table}[h]" << endl
file << "\\centering" << endl
file << "\\begin{tabular}{l||"
for n in nList do
    file << "c|"
file << "}" << endl
file << "$n$"
for n in nList do
    file << " & " << n;

file << "\\\\ \\hline" << endl;

for i from 0 to #ThreadCounts - 1 do (
    file << ThreadCounts#i;
    for j from 0 to #nList - 1 do (
        file << " & " << round(2, RunTimes#j#0 / RunTimes#j#i / ThreadCounts#i * 100) << "\\%";
    );
    file << "\\\\" << endl;
);
file << "\\end{tabular}" << endl;
file << "\\caption{Efficiency for cyclic-$n$ polynomial systems.}" << endl;
file << "\\label{CyclicParallelEfficiency}" << endl;
file << "\\end{table}"
close file 


