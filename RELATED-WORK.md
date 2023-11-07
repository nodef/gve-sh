### NetworkX Graph Generators

- graph_atlas
- graph_atlas_g
- balanced_tree
- barbell_graph
- binomial_tree
- complete_graph
- complete_multipartite_graph
- circular_ladder_graph
- circulant_graph
- cycle_graph
- dorogovtsev_goltsev_mendes_graph
- empty_graph
- full_rary_tree
- ladder_graph
- lollipop_graph
- null_graph
- path_graph
- star_graph
- tadpole_graph
- trivial_graph
- turan_graph
- wheel_graph
- margulis_gabber_galil_graph
- chordal_cycle_graph
- paley_graph
- grid_2d_graph
- grid_graph
- hexagonal_lattice_graph
- hypercube_graph
- triangular_lattice_graph
- LCF_graph
- bull_graph
- chvatal_graph
- cubical_graph
- desargues_graph
- diamond_graph
- dodecahedral_graph
- frucht_graph
- heawood_graph
- hoffman_singleton_graph
- house_graph
- house_x_graph
- icosahedral_graph
- krackhardt_kite_graph
- moebius_kantor_graph
- octahedral_graph
- pappus_graph
- petersen_graph
- sedgewick_maze_graph
- tetrahedral_graph
- truncated_cube_graph
- truncated_tetrahedron_graph
- tutte_graph
- fast_gnp_random_graph
- gnp_random_graph
- dense_gnm_random_graph
- gnm_random_graph
- erdos_renyi_graph
- binomial_graph
- newman_watts_strogatz_graph
- watts_strogatz_graph
- connected_watts_strogatz_graph
- random_regular_graph
- barabasi_albert_graph
- dual_barabasi_albert_graph
- extended_barabasi_albert_graph
- powerlaw_cluster_graph
- random_kernel_graph
- random_lobster
- random_shell_graph
- random_powerlaw_tree
- random_powerlaw_tree_sequence
- random_kernel_graph
- duplication_divergence_graph
- partial_duplication_graph
- configuration_model
- directed_configuration_model
- expected_degree_graph
- havel_hakimi_graph
- directed_havel_hakimi_graph
- degree_sequence_tree
- random_degree_sequence_graph
- random_clustered_graph
- gn_graph
- gnr_graph
- gnc_graph
- random_k_out_graph
- scale_free_graph
- geometric_edges
- geographical_threshold_graph
- navigable_small_world_graph
- random_geometric_graph
- soft_random_geometric_graph
- thresholded_random_geometric_graph
- waxman_graph
- line_graph
- inverse_line_graph
- ego_graph
- stochastic_graph
- random_internet_as_graph
- uniform_random_intersection_graph
- k_random_intersection_graph
- general_random_intersection_graph
- karate_club_graph
- davis_southern_women_graph
- florentine_families_graph
- les_miserables_graph
- caveman_graph
- connected_caveman_graph
- gaussian_random_partition_graph
- LFR_benchmark_graph
- planted_partition_graph
- random_partition_graph
- relaxed_caveman_graph
- ring_of_cliques
- stochastic_block_model
- windmill_graph
- spectral_graph_forge
- prefix_tree
- random_labeled_tree
- random_labeled_rooted_tree
- random_labeled_rooted_forest
- random_unlabeled_tree
- random_unlabeled_rooted_tree
- random_unlabeled_rooted_forest
- nonisomorphic_trees
- number_of_nonisomorphic_trees
- triad_graph
- is_valid_joint_degree
- joint_degree_graph
- is_valid_directed_joint_degree
- directed_joint_degree_graph
- mycielskian
- mycielski_graph
- hnm_harary_graph
- hkn_harary_graph
- random_cograph
- interval_graph
- sudoku_graph
- visibility_graph

<br>
<br>


### farkhor/PaRMAT

PaRMAT is a multi-threaded RMAT graph generator. Using PaRMAT, you can create very large directed or undirected RMAT graphs. PaRMAT is designed to exploit multiple threads and to avoid failing when there is not a lot of memory (RAM) available.


#### A little bit of background

Those who work on graph processing solutions clearly know that [RMAT graphs](http://epubs.siam.org/doi/pdf/10.1137/1.9781611972740.43) usually imitate the structure of graphs that are extracted from real-world origins hence are of great importance. AFAIK, there are only two publicly avaiable RMAT graph generators out there: [GTgraph](http://www.cse.psu.edu/~madduri/software/GTgraph/) and [SNAP](http://snap.stanford.edu/snap/download.html). GTGraph, which is especifically built for RMAT graph generation purpose, fails during creation of very very large graphs (half a biilion edges on a machine with 4 GB of RAM was the furthest it could go). In addition, it only creates directed graphs and cannot avoid existence of duplicate edges in the graph. How about SNAP? SNAP is on the other hand a really big graph analysis and mining library. To generate RMAT graphs in SNAP, you've got to open up the source code (*snap/examples/graphgen/*) and add the capability of RMAT graph generation (well, that's the way I know, there may be a nicer way). It only creates undirected graphs and again fails to create very large RMAT graphs (with explicitly specified number of edges and vertices).


#### PaRMAT

PaRMAT is a piece of software designed to create large RMAT graphs, even on a machine with limited amount of available memory. PaRMAT divides the adjacency matrix into squares and the workload between multiple threads. PaRMAT provides a number of options for the RMAT graph: being directed or non-directed, disallowing duplicate edges, sorting the output, varying RMAT graph parameters, etc. Directecd and non-directed graphs generated by PaRMAT show the same degree distribution as GTGraph's and SNAP's. PaRMAT does not fail when the specified graph is very large and creates the graphs faster than GTGraph and SNAP.

<br>
<br>


### named-data/Hyperbolic-Graph-Generator

The Hyperbolic-Graph-Generator is a package of tools to generate graphs embedded in the hyperbolic plane, and to compute the efficiency of greedy forwarding in these graphs. More details on the graph models implemented in the package are available at http://arxiv.org/abs/1503.05180/.

This standalone package is also a core component of the hyperbolic routing package that is going to be integrated into the NDNSim (http://ndnsim.net). Hyperbolic routing is greedy geometric routing in a hyperbolically embedded network. Hyperbolic routing has attractive scalability and efficiency characteristics. In particular, applied to the AS Internet, hyperbolic routing is nearly optimal, i.e., maximally scalable (http://dx.doi.org/10.1038/ncomms1063). The NDN testbed has been recently mapped to the hyperbolic plane yielding similar routing efficiency (http://www.caida.org/research/routing/greedy_forwarding_ndn/). Because of its exceptional scalability and efficiency characteristics, hyperbolic routing is one of the routing strategies in the NDN architecture.

More information are available at http://named-data.github.io/Hyperbolic-Graph-Generator/

<br>
<br>


### manpen/hypergen

Streaming Generator for Hyperbolic Graphs. This repository contains a prototype implementation of "Generating practical random hyperbolic graphs in near-linear time and with sub-linear memory" [M. Penschuck, SEA 2017]. Please refer to the branch SEA17 to reproduce the results.

<br>
<br>


### Daniel-Allendorf/inc-powerlaw

Exact Uniform Powerlaw Graph Generator. This repository contains supplementary material for the conference paper "Engineering Uniform Sampling of Graphs with a Prescribed Power-law Degree Sequence" scheduled for presentation at ALENEX2022. There are two basic modes of operation; either a degree sequence is produced by the generator itself or externally provided.

<br>
<br>


### jgirald/modularityGraphs

Random Generator of Graphs with Modularity. Based on: Jesús Giráldez-Cru, Jordi Levy: Generating SAT Instances with Community Structure. Artificial Intelligence 238: 119-134 (2016).

```bash
$ ./modularityGraphs [-n <nodes>] [-m <edges>] [-c <communities>] [-Q <modularity>] [-s <seed>]

## Default values:
## nodes = 1000
## edges = 4000;
## communities = 10;
## modularity = 0.8;
## seed = 0;
```

<br>
<br>


### massive-graphs/tfp-pa

External Memory Graph Generators using Preferential Attachment. This repository contains preferential attachment-based generators for graphs that do not fit into main memory. The underlying idea is introduced in ["Generating Massive Scale-Free Networks under Resource Constraints", U.Meyer and M.Penschuck, ALENEX16](https://doi.org/10.1137/1.9781611974317.4). This implementation, however, uses vertex-based tokens rather edge based, which better fits the POD-constraints imposed by the STXXL.

<br>
<br>


### niemasd/NiemaGraphGen

NiemaGraphGen (NGG) is a collection of efficient C++ implementations of graph generators for use in sampling contact networks in global-scale pandemic simulations. Importantly, NGG avoids actually storing the graph in memory and instead is intended to be used in a data streaming pipeline. Currently, NGG implements generators for the following types of graphs:

* [Barabási–Albert model](https://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model) ([Barabási & Albert, 1999](https://doi.org/10.1126/science.286.5439.509))
* [Barbell graph](https://en.wikipedia.org/wiki/Barbell_graph)
* [Complete graph](https://en.wikipedia.org/wiki/Complete_graph)
* [Cycle graph](https://en.wikipedia.org/wiki/Cycle_graph)
* [Empty graph](https://en.wikipedia.org/wiki/Null_graph#Edgeless_graph) (not particularly useful)
* [Erdős–Rényi model](https://en.wikipedia.org/wiki/Erd%C5%91s%E2%80%93R%C3%A9nyi_model) (Erdős & Rényi, 1959)
* [Newman–Watts–Strogatz model](https://doi.org/10.1016/S0375-9601(99)00757-4) ([Newman & Watts, 1999](https://doi.org/10.1016/S0375-9601(99)00757-4))
* [Path graph](https://en.wikipedia.org/wiki/Path_graph)
* [Ring lattice](https://runestone.academy/runestone/books/published/complex/SmallWorldGraphs/RingLattice.html)

<br>
<br>


### s417-lama/rmatter

Rmatter: A Fast, Space-Efficient, and Deterministic R-MAT Graph Generator. Rmatter generates an [R-MAT graph](https://www.cs.cmu.edu/~christos/PUBLICATIONS/siam04.pdf) in a fast, space-efficient, and deterministic way.

- **Fast**: This generator was developed because existing tools were too slow to generate large graphs. Rmatter can generate an R-MAT27 graph (N=2^27, M=2^31) within 3 minutes on my laptop (much faster on many-core servers!).
    - If a sufficient number of cores are provided, the processing speed is mostly limited by the disk IO bandwidth.
- **Space-Efficient**: Rmatter can generate graphs that are much larger than the local RAM size by bounding the total memory usage.
    - This idea comes from another R-MAT graph generator [PaRMAT](https://github.com/farkhor/PaRMAT).
- **Deterministic**: No matter how many times you run it, it generates the same graph (thanks to [PCG random number generator](https://www.pcg-random.org)).
    - The output order is nondeterministic, but providing the same random seed will generate an identical graph.

<br>
<br>


### 0x01h/dijkstras-algorithm

Random Graph Generator with Dijkstra's Algorithm. Dijkstra's algorithm implementation to find shortest path distance using random graph generator on 4D array layered graph system. Users can choose input parameters to determine node sizes and maximum lengths between two nodes.

Parameters:
- Number of layers.
- Number of nodes for each layer.
- Maximum length between two nodes.
- How many times do you want the algorithm works?
