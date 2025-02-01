#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <istream>
#include <ostream>
#include <fstream>
#include <omp.h>
#include "inc/main.hxx"
#include "commands.hxx"

using namespace std;




#pragma region CONFIGURATION
#ifndef KEY_TYPE
/** Type of vertex ids. */
#define KEY_TYPE uint32_t
#endif
#ifndef EDGE_VALUE_TYPE
/** Type of edge weights. */
#define EDGE_VALUE_TYPE float
#endif
#ifndef MAX_THREADS
/** Maximum number of threads to use. */
#define MAX_THREADS 1
#endif
#ifndef VERSION
/** Version of the program. */
#define VERSION "1.0.0"
#endif
#pragma endregion




#pragma region IO
/**
 * Read the specified input graph.
 * @param a read graph (output)
 * @param file input file name
 * @param format input file format
 * @param weighted is graph weighted? [false]
 * @param symmetric is graph symmetric? [false]
 */
template <class G>
inline void readGraphW(G& a, const string& file, const string& format, bool weighted=false, bool symmetric=false) {
  ifstream stream(file.c_str());
  if (format=="mtx") readGraphMtxFormatOmpW(a, stream, weighted);
  else if (format=="coo") readGraphCooFormatOmpW(a, stream, weighted, symmetric);
  else if (format=="edgelist" || format=="csv" || format=="tsv") readGraphEdgelistFormatOmpW(a, stream, weighted, symmetric);
  else if (format=="adj") readGraphAdjFormatOmpW(a, stream);
  else throw std::runtime_error("Unknown input format: `" + format + "`");
  stream.close();
}


/**
 * Write the specified output graph.
 * @param x graph to write (input)
 * @param file output file name
 * @param format output file format
 * @param weighted is graph weighted? [false]
 * @param symmetric is graph symmetric? [false]
 */
template <class G>
inline void writeGraph(const G& x, const string& file, const string& format, bool sequential=false, bool weighted=false, bool symmetric=false) {
  ofstream stream(file.c_str());
  if (sequential) {
    if (format=="mtx") writeGraphMtxFormat(stream, x, weighted, symmetric);
    else if (format=="coo") writeGraphCooFormat(stream, x, weighted, symmetric);
    else if (format=="edgelist") writeGraphEdgelistFormat(stream, x, weighted, symmetric);
    else if (format=="csv") writeGraphEdgelistFormat(stream, x, weighted, symmetric, ',');
    else if (format=="tsv") writeGraphEdgelistFormat(stream, x, weighted, symmetric, '\t');
    else if (format=="adj") writeGraphAdjFormat(stream, x);
    else throw std::runtime_error("Unknown output format: \'" + format + "\'");
  }
  else {
    if (format=="mtx") writeGraphMtxFormatOmp(stream, x, weighted, symmetric);
    else if (format=="coo") writeGraphCooFormatOmp(stream, x, weighted, symmetric);
    else if (format=="edgelist") writeGraphEdgelistFormatOmp(stream, x, weighted, symmetric);
    else if (format=="csv") writeGraphEdgelistFormatOmp(stream, x, weighted, symmetric, ',');
    else if (format=="tsv") writeGraphEdgelistFormatOmp(stream, x, weighted, symmetric, '\t');
    else if (format=="adj") writeGraphAdjFormat(stream, x);
    else throw std::runtime_error("Unknown output format: \'" + format + "\'");
  }
  stream.close();
}


/**
 * Show certain properties of the graph.
 * @param x given graph
 * @param weighted is graph weighted? [false]
 * @param symmetric is graph symmetric? [false]
 */
template <class G>
inline void showGraphProperties(const G& x, bool weighted=false, bool symmetric=false) {
  size_t N = graphOrder(x);
  size_t M = graphSizeOmp(x, !symmetric);
  auto [dmin, dmax, davg] = minMaxAvgDegreeOmp(x);
  double density = double(M) / (N*(N-1));
  printf("> |V|: %zu, |E|: %zu, Weighted: %s, Symmetric: %s\n", N, M, weighted? "yes" : "no", symmetric? "yes" : "no");
  printf("> Density: %.2e, Avg degree: %.2f, Min degree: %zu, Max degree: %zu\n", density, davg, dmin, dmax);
  printf("> Self-loops: %zu, Dead-ends: %zu\n", countSelfLoopsOmp(x), countDeadEndsOmp(x));
  printf("\n");
}
#pragma endregion




#pragma region RUN COMMANDS
/**
 * Run the count-disconnected-communities command.
 * @param argc argument count
 * @param argv argument values
 * @param i start index of arguments [1]
 * @returns zero on success, non-zero on failure
 */
inline int runCountDisconnectedCommunities(int argc, char **argv, int i=1) {
  using K = KEY_TYPE;
  using E = EDGE_VALUE_TYPE;
  using Options = OptionsCountDisconnectedCommunities;
  // Parse command-line arguments.
  Options o = parseCountDisconnectedCommunities(argc, argv, i);
  if (o.help) { helpCountDisconnectedCommunities(argv[0]); return 1; }
  if (!o.valid) return 1;
  // Read input graph.
  DiGraph<K, None, E> x;
  printf("Reading graph \'%s\' ...\n", o.inputFile.c_str());
  readGraphW(x, o.inputFile, o.inputFormat, o.weighted, o.symmetric);
  showGraphProperties(x, o.weighted, o.symmetric);
  // Symmetrize graph.
  if (!o.symmetric) {
    printf("Symmetrizing graph ...\n");
    symmetrizeOmpU(x);
    showGraphProperties(x, o.weighted, true);
  }
  // Read community membership.
  vector<K> membership(x.span());
  ifstream membershipStream(o.membershipFile.c_str());
  printf("Reading community membership \'%s\' ...\n", o.membershipFile.c_str());
  readVectorW(membership, membershipStream, o.membershipKeyed, o.membershipStart);
  // Count the number of disconnected communities.
  auto fc = [&](auto u) { return membership[u]; };
  size_t ncom = communities(x, membership).size();
  size_t ndis = countValue(communitiesDisconnectedOmp(x, membership), char(1));
  printf("Number of communities: %zu\n", ncom);
  printf("Number of disconnected communities: %zu\n", ndis);
  printf("\n");
  return 0;
}


/**
 * Run the make-undirected command.
 * @param argc argument count
 * @param argv argument values
 * @param i start index of arguments [1]
 * @returns zero on success, non-zero on failure
 */
inline int runMakeUndirected(int argc, char **argv, int i=1) {
  using K = KEY_TYPE;
  using E = EDGE_VALUE_TYPE;
  using Options = OptionsMakeUndirected;
  // Parse command-line arguments.
  Options o = parseOptionsMakeUndirected(argc, argv, i);
  if (o.help) { helpMakeUndirected(argv[0]); return 1; }
  if (!o.valid) return 1;
  // Read input graph.
  DiGraph<K, None, E> x;
  printf("Reading graph \'%s\' ...\n", o.inputFile.c_str());
  readGraphW(x, o.inputFile, o.inputFormat, o.inputWeighted, o.inputSymmetric);
  showGraphProperties(x, o.inputWeighted, o.inputSymmetric);
  // Symmetrize graph.
  if (!o.inputSymmetric) {
    printf("Symmetrizing graph ...\n");
    symmetrizeOmpU(x);
    showGraphProperties(x, o.inputWeighted, true);
  }
  // Write undirected graph.
  printf("Writing undirected graph \'%s\' ...\n", o.outputFile.c_str());
  writeGraph(x, o.outputFile, o.outputFormat, o.outputSequential, o.outputWeighted, o.outputSymmetric);
  printf("Undirected graph written to \'%s\'.\n", o.outputFile.c_str());
  printf("\n");
  return 0;
}


/**
 * Run the add-self-loops command.
 * @param argc argument count
 * @param argv argument values
 * @param i start index of arguments [1]
 * @returns zero on success, non-zero on failure
 */
inline int runAddSelfLoops(int argc, char **argv, int i=1) {
  using K = KEY_TYPE;
  using E = EDGE_VALUE_TYPE;
  using Options = OptionsAddSelfLoops;
  // Parse command-line arguments.
  Options o = parseOptionsAddSelfLoops(argc, argv, i);
  if (o.help) { helpAddSelfLoops(argv[0]); return 1; }
  if (!o.valid) return 1;
  // Read input graph.
  DiGraph<K, None, E> x;
  printf("Reading graph \'%s\' ...\n", o.inputFile.c_str());
  readGraphW(x, o.inputFile, o.inputFormat, o.inputWeighted, o.inputSymmetric);
  showGraphProperties(x, o.inputWeighted, o.inputSymmetric);
  // Add self-loops.
  printf("Adding self-loops ...\n");
  auto ft = [](auto u) { return true; };
  addSelfLoopsOmpU(x, E(1), ft);
  showGraphProperties(x, o.inputWeighted, o.inputSymmetric);
  // Write graph with self-loops.
  printf("Writing graph with self-loops \'%s\' ...\n", o.outputFile.c_str());
  writeGraph(x, o.outputFile, o.outputFormat, o.outputWeighted, o.outputSymmetric);
  printf("Graph with self-loops written to \'%s\'.\n", o.outputFile.c_str());
  printf("\n");
  return 0;
}


/**
 * Run the no-operation command.
 * @param argc argument count
 * @param argv argument values
 * @param i start index of arguments [1]
 * @returns zero on success, non-zero on failure
 */
inline int runNoOperation(int argc, char **argv, int i=1) {
  using K = KEY_TYPE;
  using E = EDGE_VALUE_TYPE;
  using Options = OptionsNoOperation;
  // Parse command-line arguments.
  Options o = parseOptionsNoOperation(argc, argv, i);
  if (o.help) { helpNoOperation(argv[0]); return 1; }
  if (!o.valid) return 1;
  // Read input graph.
  DiGraph<K, None, E> x;
  printf("Reading graph \'%s\' ...\n", o.inputFile.c_str());
  readGraphW(x, o.inputFile, o.inputFormat, o.inputWeighted, o.inputSymmetric);
  showGraphProperties(x, o.inputWeighted, o.inputSymmetric);
  // Write output graph.
  printf("Writing graph \'%s\' ...\n", o.outputFile.c_str());
  writeGraph(x, o.outputFile, o.outputFormat, o.outputWeighted, o.outputSymmetric);
  printf("Graph written to \'%s\'.\n", o.outputFile.c_str());
  printf("\n");
  return 0;
}
#pragma endregion




#pragma region MAIN
/**
 * Show version of the main program.
 * @param name program name
 * @returns zero on success, non-zero on failure
 */
inline int showVersion(const char *name) {
  fprintf(stderr, "%s version %s\n\n", name, VERSION);
  return 0;
}


/**
 * Show help message for the main program.
 * @param name program name
 * @returns zero on success, non-zero on failure
 */
inline int helpMain(const char *name) {
  fprintf(stderr, "%s: A utility for analyzing and modifying graphs.\n", name);
  fprintf(stderr, "\n");
  fprintf(stderr, "Usage: %s <command> [options]\n", name);
  fprintf(stderr, "\n");
  fprintf(stderr, "Commands:\n");
  fprintf(stderr, "  count-disconnected-communities\n");
  fprintf(stderr, "  make-undirected\n");
  fprintf(stderr, "  add-self-loops\n");
  fprintf(stderr, "  no-operation\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  --help     Show this help message.\n");
  fprintf(stderr, "  --version  Show the version of the program.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "See `%s <command> --help` for more information on a specific command.\n", name);
  fprintf(stderr, "\n");
  return 0;
}


/**
 * Main function.
 * @param argc argument count
 * @param argv argument values
 * @returns zero on success, non-zero on failure
 */
int main(int argc, char **argv) {
  using K = KEY_TYPE;
  using E = EDGE_VALUE_TYPE;
  // Initialize OpenMP.
  if (MAX_THREADS) omp_set_num_threads(MAX_THREADS);
  // Run the appropriate command.
  string cmd = argc>1 ? argv[1] : "";
  if (cmd=="--help") return helpMain(argv[0]);
  else if (cmd=="--version") return showVersion(argv[0]);
  else if (cmd=="count-disconnected-communities") return runCountDisconnectedCommunities(argc, argv, 2);
  else if (cmd=="make-undirected")                return runMakeUndirected(argc, argv, 2);
  else if (cmd=="add-self-loops")                 return runAddSelfLoops(argc, argv, 2);
  else if (cmd=="no-operation")                   return runNoOperation(argc, argv, 2);
  fprintf(stderr, "Unknown command `%s`. See `%s --help` for a list of commands.\n\n", cmd.c_str(), argv[0]);
  return 1;
}
#pragma endregion
