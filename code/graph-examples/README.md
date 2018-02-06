Graph file specification
========================

Each file is given a user specified name.
The first two lines are not read by `pmonodromy`, so they can be
used to store pertinent data about the graph. The remainder of the file has a prescribed format;
we give a sample file which can be emulated:

```
Random graph. // not consumed by C++ code
Edge count = 1; // not consumed by C++ code
Root count = 4;
Node count = 2;
Alpha = 1;
Lambda = 1;
(0,1); // This says that we are connecting Node1 to Node2
{0,1,.000465301},{1,2,.000505952},{2,3,.000673292},{3,0,.000911903}; // Every 3-tuple contains the two solutions being connected and the time taken to connect them. (Node1 Sol, Node2 Sol, time)
```
The final two lines repeat as many times as necessary, until the entire
graph has been specified.

Contents of this folder
=======================

```*.graph``` are graph data files

```*.m2``` files
* ```cyclic.m2```  generates ```cyclic-<n>.graph``` examples
* ```reimer.m2``` generates ```reimer5*.graph``` examples
* ```random.m2``` generates ```random.graph``` examples
* ```convert2failures.m2``` converts  ```cyclic-<n>.graph``` examples to ```cyclic-<n>-(alpha=<alpha-value>).graph``` examples. It "fails" paths with longer completion times to roughly match a given value of alpha.

