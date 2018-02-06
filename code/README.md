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
arguments to run:
* The first argument is a file name.
* The second is the number of simulated threads. (1)
* The third is a type of potential function. Options are Original, WeightTowardCompleteNode, ConvexCombination, and MixedStrategy. (Original)
* The "fourth" is a list of random seeds. This is not very robust: it should be a list of positive integers separated by spaces. (If not specified, a single random seed will be used)

Note that it is not possible to set the fifth option without setting the third, for example. The default values are given in parentheses.
Here is a sample run: 

```./pmonodromy sample.graph 16 Original 123 47 1 10094```

Calling this runs `pmonodromy` on a fictious file sample.graph, with 16
threads, using the Original potential function, for random seeds
123 47 1 10094.
