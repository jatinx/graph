// Single include graph header

#pragma once

#if GDEBUG
#include <iostream>
#endif

#include <algorithm>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>

typedef enum Direction { UNDIRECTED = 0, DIRECTED } Direction;
typedef enum Weight { UNWEIGHTED = 0, WEIGHTED } Weight;

template <typename T> class Graph {
  std::vector<std::pair<T, std::vector<std::pair<T, int>>>>
      g; // [{node,[node, weight]}]
  Weight weighted;
  Direction directed;

  std::vector<std::pair<T, int>> emptyPair;
  std::vector<std::pair<T, int>> &_find(const T &a) {
    for (auto &&i : g) {
      if (i.first == a)
        return i.second;
    }
    return emptyPair;
  }
  int find(const T &a) {
    for (int i = 0; i < g.size(); i++)
      if (g[i].first == a)
        return i;
    return -1;
  }

public:
  Graph(Direction d = UNDIRECTED, Weight w = UNWEIGHTED, int sizeHint = 10)
      : weighted(w), directed(d) {
    g.reserve(sizeHint);
    emptyPair.push_back(std::make_pair(-1, -1));
  }
  void insertNode(T a) {
    auto res = find(a);
    if (res == -1) {
      std::vector<std::pair<T, int>> v;
      g.push_back(std::make_pair(a, v));
    }
  }
  void insertEdge(T a, T b, int weight = 0) {
    int a_ = find(a);
    int b_ = find(b);
    if (a_ == -1 || b == -1) {
#if GDEBUG
      std::cout << "shit's empty yo " << a_ << " " << b_ << std::endl;
#endif
      return; // G fucking G
    }
    auto _t = std::make_pair(b, weight);
    g[a_].second.push_back(_t);
    if (!directed) {
      auto _t = std::make_pair(a, weight);
      g[b_].second.push_back(_t);
    }
  }
  std::vector<T> getNodes() {
    std::vector<T> v;
    v.reserve(g.size());
    for (auto &i : g) {
      v.push_back(i.first);
    }
    return v;
  }
  std::vector<T> getConnected(T a) {
    std::vector<T> v;
    auto res = _find(a);
    v.reserve(res.size());
    for (auto &i : res) {
      v.push_back(i.first);
    }
    return v;
  }
  std::vector<std::pair<T, int>> getConnectedWithWeights(T a) {
    std::vector<std::pair<T, int>> v;
    auto res = _find(a);
    v.reserve(res.size());
    for (auto &i : res) {
      v.push_back(std::make_pair(i.first, i.second));
    }
    return v;
  }
};

// Algorithms
// Generic FS
template <typename T> class stack : public std::stack<T> {
public:
  T front() { return std::stack<T>::top(); }
};
template <typename T, typename Container> std::vector<T> _fs(Graph<T> &g) {
  std::vector<T> v;
  auto nodes = g.getNodes();
  v.reserve(nodes.size());
  std::set<T> visited;
  if (!nodes.size())
    return v;
  Container q;
  q.push(*nodes.begin());
  while (!q.empty()) {
    v.push_back(q.front());
    auto c_ = g.getConnected(q.front());
    visited.insert(q.front());
    q.pop();
    for (auto &i : c_) {
      if (visited.find(i) == visited.end()) {
        visited.insert(i);
        q.push(i);
      }
    }
  }
  return v;
}

// BFS
template <typename T> std::vector<T> bfs(Graph<T> &g) {
  return _fs<T, std::queue<T>>(g);
}
// DFS
template <typename T> std::vector<T> dfs(Graph<T> &g) {
  return _fs<T, stack<T>>(g);
}

