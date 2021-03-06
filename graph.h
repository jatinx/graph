// Single include graph header
// Only used for finding results to some graph problems my friends send me
// Might not be optimized for your use case

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

  T *head = nullptr;
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
  // Can be optimized
  void setHead(T a) {
    auto i = find(a);
    if (i != -1)
      head = &g[i].first;
  }
  T getHead() {
    if (head)
      return *head;
    return -1; // Maybe have some universal value
  }
  void insertNode(T a) {
    auto res = find(a);
    if (res == -1) {
      std::vector<std::pair<T, int>> v;
      g.push_back(std::make_pair(a, v));
      if (head == nullptr)
        head = &g[0].first;
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
  T head = g.getHead();
  std::set<T> visited;
  if (!head)
    return v;
  Container q;
  q.push(head);
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
template <typename Container, typename T>
bool contains(const Container &c, T a) {
  const T *end = &c.top() + 1;
  const T *begin = end - c.size();
  std::vector<T> v(begin, end);
  for (auto &i : v) {
    if (i == a)
      return true;
  }
  return false;
}
template <typename T, typename Container> bool _fsCycle(Graph<T> &g) {
  std::vector<T> v;
  T head = g.getHead();
  std::set<T> visited;
  if (!head)
    return false;
  Container q;
  q.push(head);
  while (!q.empty()) {
    v.push_back(q.top());
    auto c_ = g.getConnected(q.top());
    visited.insert(q.top());
    q.pop();
    if (c_.size() == 0)
      v.pop_back();
    for (auto &i : c_) {
      for (auto &j : v) {
        if (j == i)
          return true;
      }
      q.push(i);
    }
  }
  return false;
}

// BFS
template <typename T> std::vector<T> bfs(Graph<T> &g) {
  return _fs<T, std::queue<T>>(g);
}
// DFS
template <typename T> std::vector<T> dfs(Graph<T> &g) {
  return _fs<T, stack<T>>(g);
}
// Has cycle
template <typename T> bool _dfsCycle(Graph<T> &g) {
  return _fsCycle<T, std::stack<T, std::vector<T>>>(g);
}

// Has Cycle
template <typename T, typename Container> bool _hasCycle(Graph<T> &g) {
  auto nodes = g.getNodes();
  if (!nodes.size())
    return false;
  Container q;
  std::set<T> done;
  auto head = g.getHead();
  for (auto &i : nodes) {
    g.setHead(i);
    if (_dfsCycle(g))
      return true;
  }
  g.setHead(head);
  return false;
}

template <typename T> bool hasCycle(Graph<T> &g) {
  return _hasCycle<T, std::stack<T, std::vector<T>>>(g);
}

// Shortest Path
template <typename T> int getDistance(Graph<T> &g, T node) {
  T head = g.getHead();
  std::queue<std::pair<T, std::pair<T, int>>> q; // {child, {parent, distance}};
  std::set<T> visited;
  q.push(std::make_pair(head, std::make_pair(head, 0)));
  while (!q.empty()) {
    int d_ = q.front().second.second; // distance
    auto c_ = g.getConnected(q.front().first);
    for (auto &i : c_) {
      if (i == node) {
        return d_ + 1;
      }
      q.push(std::make_pair(i, std::make_pair(q.front().first, d_ + 1)));
    }
    q.pop();
  }
  return -1;
}

// Connected via for loop and BFS
template <typename T> int getConnectedRegions(Graph<T> &g) {
  int count = 0;
  auto allNodes = g.getNodes();
  std::set<T> visited;
  auto _head = g.getHead();
  for (auto &node : allNodes) {
    g.setHead(node);
    auto bfsResult = bfs(g);
    bool inserted = false;
    for (auto &i : bfsResult) {
      if (visited.find(i) == visited.end()) {
        visited.insert(i);
        inserted = true;
      }
    }
    if (inserted) {
      count++;
    }
  }
  g.setHead(_head);
  return count;
}

int index = 1;

template <typename T, typename S>
bool hasNonVisitedChildren(const T &c_, const S &s) {
  for (auto &i : c_) {
    if (s.find(i) == s.end())
      return true;
  }
  return false;
}

template <typename T> std::vector<std::pair<T, int>> _fsDAG(Graph<T> &g) {
  std::vector<std::pair<T, int>> v;
  T head = g.getHead();
  std::set<T> visited;
  if (!head)
    return v;
  int id = 0;
  std::stack<std::pair<T, int>> q;
  q.push(std::make_pair(head, id++));
  while (!q.empty()) {
    // v.push_back(q.top());
    auto c_ = g.getConnected(q.top().first);
    if (!hasNonVisitedChildren(c_, visited)) {
      v.push_back(std::make_pair(q.top().first, index--));
      q.pop();
      continue;
    }
    visited.insert(q.top().first);
    for (auto &i : c_) {
      if (visited.find(i) == visited.end()) {
        visited.insert(i);
        q.push(std::make_pair(i, id++));
      }
    }
  }
  return v;
}

template <typename T> std::vector<T> getTopologicalSort(Graph<T> &g) {
  std::vector<T> v;
  // if(hasCycle(g) == true) { std::cout << "Cycle" << std::endl; return v; }
  index = g.getNodes().size();
  v.reserve(g.getNodes().size());
  auto res = _fsDAG(g);
  sort(res.begin(), res.end(),
       [](auto &a, auto &b) { return a.second < b.second; });
  for (auto &i : res) {
    std::cout << i.first << " - " << i.second << std::endl;
    v.push_back(i.first);
  }
  return v;
}
