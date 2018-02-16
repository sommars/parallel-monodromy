Instructions for using `pmonodromy`
===================================

### Creating a graph file

To use `pmonodoromy`, it is first necessary to create a graph file. These files
must follow the specification given in the ./graph-examples README file.
We have provided the Macaulay2 file graph_creator.m2 to automatically create graphs
that fit this specification.

There are two ways that graphs can be created: they can be either "fake" graphs
with randomly created correspondences or "real" graphs based on saturations
of homotopy graphs.

`createFakeGraph` is the function for creating "fake" complete graphs based on
the following input:
   * Root count
   * #Vertices
   * #Edges (in a complete graph per pair of vertices)
   * File name

`createRealGraph` is the function for creating "real" complete graphs based on
the following input:
   * Polynomial system
   * #Vertices
   * #Edges (in a complete graph per pair of vertices)
   * File name

Additionally, the user can set various parameters through
calling setDefault before `createRealGraph`

### Using `pmonodromy`

Now that a graph file has been created, we can use `pmonodromy` to experiment
with the graph (note that before it is first used, it must be made through
calling `make` in this directory). `pmonodromy` requires a series of 
arguments to run, the first of which is a file name. All
remaining parameters are optional; the default values are given in parentheses.
* `-t X` runs `pmonodromy` with `X` simulated threads. (1)
* `-e X` runs `pmonodromy` with potential function `X`. Options are Original, WeightTowardCompleteNode, ConvexCombination, and MixedStrategy. (Original)
* `-s X` runs `pmonodromy` with random seed `X`. If this is used multiple times, multiple random seeds will be used. (Random).
* `-a X` runs `pmonodromy` with Alpha = `X`. (1)
* `-l X` runs `pmonodromy` with Lambda = `X`.

Here is a sample run: 

```./pmonodromy sample.graph -e Original -s 123 -s 47 -t 16```

Calling this runs `pmonodromy` on a fictious file sample.graph, with 16
threads, using the Original potential function, for random seeds
123 and 47.
