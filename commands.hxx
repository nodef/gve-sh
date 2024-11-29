#pragma once
#include <string>
#include <string_view>
#include <unordered_map>
#include <cstdint>

using std::string;
using std::string_view;
using std::unordered_map;
using std::stoi;




#pragma region OPTIONS
/** Command-line options for the program. */
typedef unordered_map<string, string> Options;


/**
 * Check if a string is a boolean.
 * @param x string to check
 * @returns true if the string is a boolean, false otherwise
 */
inline bool isBool(string_view x) {
  return x=="0" || x=="1" || x=="true" || x=="false";
}


/**
 * Check if a string is an integer.
 * @param x string to check
 * @returns true if the string is an integer, false otherwise
 */
inline bool isInt(string_view x) {
  char *end;
  strtol(x.data(), &end, 10);
  return end == x.data() + x.size();
}


/**
 * Check if a string is a graph id.
 * @param x string to check
 * @returns true if the string is a graph id, false otherwise
 */
inline bool isGraphId(string_view x) {
  return x.size()>=2 && (x[0]=='G' || x[0]=='g') && isInt(x.substr(1));
}


/**
 * Check if a string is a double.
 * @param x string to check
 * @returns true if the string is a double, false otherwise
 */
inline bool isDouble(string_view x) {
  char *end;
  strtod(x.data(), &end);
  return end == x.data() + x.size();
}


/**
 * Extract the extension from a path.
 * @param path path to extract extension from
 * @returns extension of the path
 */
inline string_view pathExtname(string_view path) {
  auto   idx = path.rfind('.');
  return idx==string::npos? "" : path.substr(idx);
}


/**
 * Parse file format from a string.
 * @param x string to parse
 * @returns parsed file format
 */
inline const char* parseFileFormat(string_view x) {
  if (!x.empty() && x[0]=='.') x = x.substr(1);
  if (x=="") return "";
  if (x=="edgelist"      || x=="edges") return "edgelist";
  if (x=="matrix-market" || x=="mtx")   return "matrix-market";
  if (x=="snap-temporal" || x=="temporal" || x=="txt") return "temporal";
}


/**
 * Read a command from the command-line arguments.
 * @param o options to read into (updated)
 * @param argc number of arguments
 * @param argv array of arguments
 * @param i index of the current argument
 * @returns index of the next argument to read
 * @details
 * Available commands:
 * $ serve
 * Start a server to listen for commands.
 *
 * $ load -i <input-file>
 * Load a graph from a file into memory.
 *
 * $ unload <input-graph>
 * Unload a graph from memory.
 *
 * $ save <input-graph> -o <output-file>
 * Save a graph from memory to a file.
 *
 * $ convert -i <input-file> -o <output-file>
 * Convert a graph from one format to another.
 *
 * $ measure <subcommand> <input-graph>
 * $ measure <subcommand> -i <input-file>
 * Measure a property of a graph.
 */
inline int readCommandU(Options& o, int argc, const char **argv, int i) {
  string_view c = o["command"];
  if (c=="unload" || c=="save") {
    if  (o["input-graph"]=="") o["input-graph"] = argv[i];
    else o["error"] = "Input graph `" + o["input-graph"] + "` already specified";
  }
  return i;
}


/**
 * Verify the options for a command.
 * @param o options to verify
 * @returns error message if any
 */
inline string verifyOptions(Options& o) {
  string_view c = o["command"];
  if (c=="unload" || c=="save") {
    string_view g = o["input-graph"];
    if (g=="")         return "No input graph specified";
    if (!isGraphId(g)) return "Invalid input graph `" + string(g) + "` (must be G<int>)";
  }
  if (c=="load" || c=="convert") {
    if (o["input-file"]=="")    return "No input file specified";
    if (o["input-format"]=="")  return "No input format specified";
  }
  if (c=="save" || c=="convert") {
    if (o["output-file"]=="")   return "No output file specified";
    if (o["output-format"]=="") return "No output format specified";
  }
  return "";
}


/**
 * Read command-line options into an Options struct.
 * @param argc number of arguments
 * @param argv array of arguments
 * @returns parsed Options
 */
inline Options readOptions(int argc, const char **argv) {
  Options o;
  // Read each option one by one.
  for (int i=1; i<argc; ++i) {
    string_view k = argv[i];
    if (k=="--help") o["help"] = "1";
    else if (k=="-i" || k=="--input")  o["input-file"]    = argv[++i];
    else if (k=="-o" || k=="--output") o["output-file"]   = argv[++i];
    else if (k=="-f" || k=="--format") o["input-format"]  = argv[++i];
    else if (k=="--input-file")        o["input-file"]    = argv[++i];
    else if (k=="--output-file")       o["output-file"]   = argv[++i];
    else if (k=="--input-format")      o["input-format"]  = argv[++i];
    else if (k=="--output-format")     o["output-format"] = argv[++i];
    else if (k[0]=='-')        o["error"]   = "Unknown option: " + string(k);
    else if (o["command"]=="") o["command"] = k;
    else i = readCommandU(o, argc, argv, i);
  }
  // Preliminary checks.
  if (o["command"]=="") o["error"] = "No command specified";
  if (o["error"]!="")   return o;
  // Obtain default input/output formats.
  if (o["input-format"]=="")  o["input-format"]  = parseFileFormat(pathExtname(o["input-file"]));
  if (o["output-format"]=="") o["output-format"] = parseFileFormat(pathExtname(o["output-file"]));
  if (o["output-format"]=="") o["output-format"] = o["input-format"];
  // Verify options.
  o["error"] = verifyOptions(o);
  return o;
}
#pragma endregion




#pragma region HELP
/**
 * Generate a help message for the program.
 * @returns help message
 */
inline const char* helpMessage() {
  return "For usage details, please try the following URL:\n"
  "https://github.com/ionicf/gve.sh";
}
#pragma endregion
