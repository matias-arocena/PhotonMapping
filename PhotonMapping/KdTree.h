#pragma once
#include <vector>
#include <queue>

class Photon;

class KdTree {
 private:
  struct Node {
    int axis;           // separation axis(x=0, y=1, z=2)
    int idx;            // index of median point
    int leftChildIdx;   // index of left child
    int rightChildIdx;  // index of right child

    Node() : axis(-1), idx(-1), leftChildIdx(-1), rightChildIdx(-1) {}
  };

  typedef std::priority_queue<std::pair<float, int>> KNNQueue;

  std::vector<Node> nodes;  // array of tree nodes
  const Photon* points;     // pointer to array of points
  int nPoints;              // number of points

  static float distance2(const Photon& p1, const Photon& p2);
  void buildNode(int* indices, int n_points, int depth);
  void searchKNearestNode(int nodeIdx, const Photon& queryPoint, int k, KNNQueue& queue) const;

 public:
  KdTree() {}
  void setPoints(const Photon* points, int nPoints);
  void buildTree();
  std::vector<int> searchKNearest(const Photon& queryPoint, int k, float& maxDist2) const;
};
