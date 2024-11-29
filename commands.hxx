#pragma once
#include <cstdint>
#include <cstdio>
#include <string>

using std::string;




#pragma region COMMON
/**
 * Show help message for supported graph formats.
 */
inline void helpGraphFormats() {
  fprintf(stderr, "Supported graph formats:\n");
  fprintf(stderr, "  mtx       Matrix Market format.\n");
  fprintf(stderr, "  coo       Coordinate format.\n");
  fprintf(stderr, "  edgelist  Edgelist format.\n");
  fprintf(stderr, "  csv       Comma-separated values format.\n");
  fprintf(stderr, "  tsv       Tab-separated values format.\n");
  fprintf(stderr, "\n");
}


/**
 * Show help message for command-line option.
 * @param details details of the option, e.g, "-i, --input <file>"
 */
inline void showUsage(const char *details) {
  fprintf(stderr, "\n");
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s\n", details);
  fprintf(stderr, "\n");
}


/**
 * Validate the input file.
 * @param x input file name
 * @param details details of the option
 * @returns true if the input file is valid
 */
inline bool isInputFile(const string &x, const char *details="-i, --input <file>") {
  if (!x.empty()) return true;
  fprintf(stderr, "No input file specified\n");
  showUsage(details);
  return false;
}


/**
 * Validate the output file.
 * @param x output file name
 * @param details details of the option
 * @returns true if the output file is valid
 */
inline bool isOutputFile(const string &x, const char *details="-o, --output <file>") {
  if (!x.empty()) return true;
  fprintf(stderr, "No output file specified\n");
  showUsage(details);
  return false;
}


/**
 * Validate the graph format.
 * @param x graph format
 * @param details details of the option
 * @returns true if the graph format is valid
 */
inline bool isGraphFormat(const string &x, const char *details="-f, --format <format>") {
  if (x=="mtx" || x=="coo" || x=="csv" || x=="tsv") return true;
  fprintf(stderr, "Unknown graph format `%s`\n", x.c_str());
  showUsage(details);
  helpGraphFormats();
  return false;
}
#pragma endregion




#pragma region COUNT DISCONNECTED COMMUNITIES
/**
 * Command-line options for the count-disconnected-communities command.
 */
struct OptionsCountDisconnectedCommunities {
  /** Show help message? */
  bool help = false;
  /** Is it valid? */
  bool valid = false;
  /** Input file name. */
  string inputFile = "";
  /** Input file format (see supported formats). */
  string inputFormat = "mtx";
  /** Community membership file name. */
  string membershipFile = "";
  /** Whether the community membership file is keyed. */
  bool membershipKeyed = false;
  /** Community membership start index. */
  int membershipStart = 0;
  /** Whether the input graph is weighted. */
  bool weighted = false;
  /** Whether the input graph is symmetric. */
  bool symmetric = false;
};


/**
 * Show help message for the count-disconnected-communities command.
 * @param name program name
 */
inline void helpCountDisconnectedCommunities(const char *name) {
  fprintf(stderr, "%s count-disconnected-communities:\n", name);
  fprintf(stderr, "Count the number of disconnected communities in a graph, for the given community membership of each vertex.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h, --help                   Show this help message.\n");
  fprintf(stderr, "  -i, --input <file>           Input file name.\n");
  fprintf(stderr, "  -f, --input-format <format>  Input file format.\n");
  fprintf(stderr, "  -m, --membership <file>      Community membership file name.\n");
  fprintf(stderr, "  -k, --membership-keyed       Community membership file is keyed.\n");
  fprintf(stderr, "  -r, --membership-start       Community membership start index.\n");
  fprintf(stderr, "  -w, --weighted               Input graph is weighted.\n");
  fprintf(stderr, "  -s, --symmetric              Input graph is symmetric.\n");
  fprintf(stderr, "\n");
  helpGraphFormats();
}


/**
 * Parse command line arguments.
 * @param argc argument count
 * @param argv argument values
 * @param i starting index
 * @returns options
 */
inline OptionsCountDisconnectedCommunities parseCountDisconnectedCommunities(int argc, char **argv, int i=1) {
  OptionsCountDisconnectedCommunities o;
  // Parse command-line arguments.
  for (; i<argc; ++i) {
    string k = argv[i];
    if (k=="") continue;
    else if (k=="-h" || k=="--help") o.help = true;
    else if (k=="-i" || k=="--input")  o.inputFile  = argv[++i];
    else if (k=="-m" || k=="--membership") o.membershipFile  = argv[++i];
    else if (k=="-f" || k=="--input-format")  o.inputFormat  = argv[++i];
    else if (k=="-k" || k=="--membership-keyed") o.membershipKeyed = true;
    else if (k=="-r" || k=="--membership-start") o.membershipStart = atoi(argv[++i]);
    else if (k=="-w" || k=="--weighted")  o.weighted  = true;
    else if (k=="-s" || k=="--symmetric") o.symmetric = true;
    else {
      fprintf(stderr, "Unknown option: %s\n", k.c_str());
      o.help = true;
    }
  }
  // Validate options.
  if (!isInputFile(o.inputFile)) return o;
  if (!isGraphFormat(o.inputFormat, "-f, --input-format <format>")) return o;
  if (o.help) { helpCountDisconnectedCommunities(argv[0]); return o; }
  o.valid = true;
  return o;
}
#pragma endregion




#pragma region MAKE UNDIRECTED
/**
 * Command-line options for the make-undirected command.
 */
struct OptionsMakeUndirected {
  /** Show help message? */
  bool help = false;
  /** Is it valid? */
  bool valid = false;
  /** Input file name. */
  string inputFile = "";
  /** Output file name. */
  string outputFile = "";
  /** Input file format (see supported formats). */
  string inputFormat = "mtx";
  /** Output file format (see supported formats). */
  string outputFormat = "mtx";
  /** Whether the input graph is weighted. */
  bool inputWeighted = false;
  /** Whether the output graph is weighted. */
  bool outputWeighted = false;
  /** Whether the input graph is symmetric. */
  bool inputSymmetric = false;
  /** Whether the output graph is symmetric. */
  bool outputSymmetric = false;
};


/**
 * Show help message for the make-undirected command.
 * @param name program name
 */
inline void helpMakeUndirected(const char *name) {
  fprintf(stderr, "%s make-undirected:\n", name);
  fprintf(stderr, "Convert a directed graph to an undirected graph.\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -h, --help                    Show this help message.\n");
  fprintf(stderr, "  -i, --input <file>            Input file name.\n");
  fprintf(stderr, "  -o, --output <file>           Output file name.\n");
  fprintf(stderr, "  -f, --input-format <format>   Input file format [mtx].\n");
  fprintf(stderr, "  -g, --output-format <format>  Output file format [mtx].\n");
  fprintf(stderr, "  -w, --input-weighted          Input graph is weighted [false].\n");
  fprintf(stderr, "  -x, --output-weighted         Output graph is weighted [false].\n");
  fprintf(stderr, "  -s, --input-symmetric         Input graph is symmetric [false].\n");
  fprintf(stderr, "  -t, --output-symmetric        Output graph is symmetric [false].\n");
  fprintf(stderr, "\n");
  helpGraphFormats();
}


/**
 * Parse command line arguments.
 * @param argc argument count
 * @param argv argument values
 * @param i starting index
 * @returns options
 */
inline OptionsMakeUndirected parseOptionsMakeUndirected(int argc, char **argv, int i=1) {
  OptionsMakeUndirected o;
  // Parse command-line arguments.
  for (; i<argc; ++i) {
    string k = argv[i];
    if (k=="") continue;
    else if (k=="-h" || k=="--help") o.help = true;
    else if (k=="-i" || k=="--input")  o.inputFile  = argv[++i];
    else if (k=="-o" || k=="--output") o.outputFile = argv[++i];
    else if (k=="-f" || k=="--input-format")  o.inputFormat  = argv[++i];
    else if (k=="-g" || k=="--output-format") o.outputFormat = argv[++i];
    else if (k=="-w" || k=="--input-weighted")   o.inputWeighted   = true;
    else if (k=="-x" || k=="--output-weighted")  o.outputWeighted  = true;
    else if (k=="-s" || k=="--input-symmetric")  o.inputSymmetric  = true;
    else if (k=="-t" || k=="--output-symmetric") o.outputSymmetric = true;
    else {
      fprintf(stderr, "Unknown option: %s\n", k.c_str());
      o.help = true;
    }
  }
  // Validate options.
  if (!isInputFile(o.inputFile)) return o;
  if (!isOutputFile(o.outputFile)) return o;
  if (!isGraphFormat(o.inputFormat, "-f, --input-format <format>")) return o;
  if (!isGraphFormat(o.outputFormat, "-g, --output-format <format>")) return o;
  if (o.help) { helpMakeUndirected(argv[0]); return o; }
  o.valid = true;
  return o;
}
#pragma endregion
