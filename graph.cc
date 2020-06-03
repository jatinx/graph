#include "graph.h"
#include <iostream>

using namespace std;

int main() {
  Graph<int> g{DIRECTED};
  g.insertNode(1);
  g.insertNode(2);
  g.insertNode(3);
  g.insertNode(4);

  g.insertNode(5);
  g.insertNode(6);

  g.insertEdge(1, 4);
  g.insertEdge(1, 2);
  g.insertEdge(4, 3);
  g.insertEdge(2, 3);

  g.insertEdge(5, 6);

  g.setHead(1);
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
  cout << "Has Cycle:: " << hasCycle(g) << endl;
  cout << "Head:: " << g.getHead() << endl;
  int dist = getDistance(g, 3);
  cout << "Distance of 1 and 4 is:: " << dist << endl;

  cout << "Connnected Regions:: " << getConnectedRegions(g) << endl;
}
