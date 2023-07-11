#pragma once
#include <vector>
#include <cmath>
#include "_main.hxx"
#ifdef OPENMP
#include <omp.h>
#endif

using std::vector;
using std::pow;




#pragma region METHODS
#pragma region EDGE WEIGHT
/**
 * Find the total outgoing edge weight of a vertex.
 * @param x original graph
 * @param u given vertex
 * @returns total outgoing weight of a vertex
 */
template <class G, class K>
inline double edgeWeight(const G& x, K u) {
  double a = 0;
  x.forEachEdge(u, [&](auto v, auto w) { a += w; });
  return a;
}


/**
 * Find the total edge weight of a graph.
 * @param x original graph
 * @returns total edge weight (undirected graph => each edge considered twice)
 */
template <class G>
inline double edgeWeight(const G& x) {
  double a = 0;
  x.forEachVertexKey([&](auto u) { a += edgeWeight(x, u); });
  return a;
}

/**
 * Find the total edge weight of a graph in parallel.
 * @param x original graph
 * @returns total edge weight (undirected graph => each edge considered twice)
 */
template <class G>
inline double edgeWeightOmp(const G& x) {
  using K = typename G::key_type;
  double a = 0;
  size_t S = x.span();
  #pragma omp parallel for schedule(auto) reduction(+:a)
  for (K u=0; u<S; ++u) {
    if (!x.hasVertex(u)) continue;
    a += edgeWeight(x, u);
  }
  return a;
}
#pragma endregion




#pragma region DEGREES
/**
 * Find the outgoing degree of each vertex.
 * @param a degrees of each vertex (output)
 * @param x original graph
 * @returns outgoing degree of each vertex
 */
template <class G, class K>
inline void degreesW(vector<K>& a, const G& x) {
  x.forEachVertexKey([&](auto u) { a[u] = x.degree(u); });
}
#pragma endregion




#pragma region MODULARITY
/**
 * Find the modularity of a community C.
 * @param cin total weight of edges within community C
 * @param ctot total weight of edges of community C
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 * @see https://www.youtube.com/watch?v=0zuiLBOIcsw
 */
inline double modularityCommunity(double cin, double ctot, double M, double R=1) {
  ASSERT(cin>=0 && ctot>=0 && M>0 && R>0);
  return cin/(2*M) - R*pow(ctot/(2*M), 2);
}


/**
 * Find the modularity of a set of communities.
 * @param cin total weight of edges within each community
 * @param ctot total weight of edges of each community
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class V>
inline double modularityCommunities(const vector<V>& cin, const vector<V>& ctot, double M, double R=1) {
  ASSERT(M>0 && R>0);
  double a = 0;
  for (size_t i=0, I=cin.size(); i<I; ++i)
    a += modularityCommunity(cin[i], ctot[i], M, R);
  return a;
}

#ifdef OPENMP
/**
 * Find the modularity of a set of communities in parallel.
 * @param cin total weight of edges within each community
 * @param ctot total weight of edges of each community
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class V>
inline double modularityCommunitiesOmp(const vector<V>& cin, const vector<V>& ctot, double M, double R=1) {
  ASSERT(M>0 && R>0);
  double a = 0;
  size_t C = cin.size();
  #pragma omp parallel for schedule(static) reduction(+:a)
  for (size_t i=0; i<C; ++i)
    a += modularityCommunity(cin[i], ctot[i], M, R);
  return a;
}
#endif


/**
 * Find the modularity of a graph, based on community membership function.
 * @param cin total weight of edges within each community (updated)
 * @param ctot total weight of edges of each community (updated)
 * @param x original graph
 * @param fc community membership function of each vertex (u)
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class G, class V, class FC>
inline double modularityByW(vector<V>& cin, vector<V>& ctot, const G& x, FC fc, double M, double R=1) {
  ASSERT(M>0 && R>0);
  x.forEachVertexKey([&](auto u) {
    size_t c = fc(u);
    x.forEachEdge(u, [&](auto v, auto w) {
      size_t d = fc(v);
      if (c==d) cin[c] += w;
      ctot[c] += w;
    });
  });
  return modularityCommunities(cin, ctot, M, R);
}

#ifdef OPENMP
/**
 * Find the modularity of a graph, based on community membership function, in parallel.
 * @param cin total weight of edges within each community (updated)
 * @param ctot total weight of edges of each community (updated)
 * @param x original graph
 * @param fc community membership function of each vertex (u)
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class G, class V, class FC>
inline double modularityByOmpW(vector2d<V>& cin, vector2d<V>& ctot, const G& x, FC fc, double M, double R=1) {
  using K = typename G::key_type;
  ASSERT(M>0 && R>0);
  size_t S = x.span();
  int    T = omp_get_max_threads();
  #pragma omp parallel
  {
    int t = omp_get_thread_num();
    fillValueU(cin [t], 0.0);
    fillValueU(ctot[t], 0.0);
  }
  #pragma omp parallel for schedule(dynamic, 2048)
  for (K u=0; u<S; ++u) {
    int t = omp_get_thread_num();
    if (!x.hasVertex(u)) continue;
    size_t c = fc(u);
    x.forEachEdge(u, [&](auto v, auto w) {
      size_t d = fc(v);
      if (c==d) cin[t][c] += w;
      ctot[t][c] += w;
    });
  }
  #pragma omp parallel for schedule(auto)
  for (size_t c=0; c<S; ++c) {
    for (int t=1; t<T; ++t) {
      cin [0][c] += cin [t][c];
      ctot[0][c] += ctot[t][c];
    }
  }
  return modularityCommunitiesOmp(cin[0], ctot[0], M, R);
}
#endif


/**
 * Find the modularity of a graph, based on community membership function.
 * @param x original graph
 * @param fc community membership function of each vertex (u)
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class G, class FC>
inline double modularityBy(const G& x, FC fc, double M, double R=1) {
  size_t S = x.span();
  vector<double> cin(S);
  vector<double> ctot(S);
  return modularityByW(cin, ctot, x, fc, M, R);
}

#ifdef OPENMP
/**
 * Find the modularity of a graph, based on community membership function, in parallel.
 * @param x original graph
 * @param fc community membership function of each vertex (u)
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns modularity [-0.5, 1]
 */
template <class G, class FC>
inline double modularityByOmp(const G& x, FC fc, double M, double R=1) {
  size_t S = x.span();
  int    T = omp_get_max_threads();
  // Limit memory usage to 64GB.
  size_t VALUES = 64ULL*1024*1024*1024 / 8;
  int    TADJ   = int(max(VALUES / (2*S), size_t(1)));
  vector2d<double> cin (TADJ, vector<double>(S));
  vector2d<double> ctot(TADJ, vector<double>(S));
  // Run in parallel with limited threads
  omp_set_num_threads(TADJ);
  double Q = modularityByOmpW(cin, ctot, x, fc, M, R);
  omp_set_num_threads(T);
  return Q;
}
#endif
#pragma endregion




#pragma region DELTA MODULARITY
/**
 * Find the change in modularity when moving a vertex from community D to C.
 * @param vcout total weight of edges from vertex v to community C
 * @param vdout total weight of edges from vertex v to community D
 * @param vtot total weight of edges from vertex v
 * @param ctot total weight of edges from community C
 * @param dtot total weight of edges from community C
 * @param M total weight of "undirected" graph (1/2 of directed graph)
 * @param R resolution (0, 1]
 * @returns delta-modularity [-0.5, 1]
 * @see https://gist.github.com/wolfram77/a3c95cd94a38a100f9b075594a823928
 */
inline double deltaModularity(double vcout, double vdout, double vtot, double ctot, double dtot, double M, double R=1) {
  ASSERT(vcout>=0 && vdout>=0 && vtot>=0 && ctot>=0 && dtot>=0 && M>0 && R>0);
  return (vcout-vdout)/M - R*vtot*(vtot+ctot-dtot)/(2*M*M);
}
#pragma endregion
#pragma endregion
