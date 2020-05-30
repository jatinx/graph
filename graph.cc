#include "graph.h"
#include <iostream>

using namespace std;

int main() {
  Graph<int> g;
  g.insertNode(1);
  g.insertNode(2);
  g.insertNode(3);
  g.insertNode(4);
  g.insertEdge(1, 4);
  g.insertEdge(1, 2);
  g.insertEdge(4, 3);
  auto v = g.getNodes();
  for (auto &i : v) {
    cout << i << " - ";
  }
  cout << endl;
  cout << "Shit connected with 1:: ";
  for (auto &i : g.getConnected(1)) {
    cout << i << " - ";
  }
  cout << endl;
  cout << "Shit connected with 4:: ";
  for (auto &i : g.getConnected(4)) {
    cout << i << " - ";
  }
  cout << endl;
  cout << "Shit connected with 1 with weights:: ";
  for (auto &i : g.getConnectedWithWeights(1)) {
    cout << "{" << i.first << " , " << i.second << "}"
         << " - ";
  }
  cout << endl;
  cout << "BFS:: ";
  auto _bfs = bfs(g);
  for (auto &i : _bfs) {
    cout << i << " - ";
  }
  cout << endl;
  cout << "DFS:: ";
  auto _dfs = dfs(g);
  for (auto &i : _dfs) {
    cout << i << " - ";
  }
  cout << endl;
}
