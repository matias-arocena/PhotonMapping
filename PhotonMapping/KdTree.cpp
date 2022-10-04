#include "KdTree.h"
#include <numeric>

#include "glm/glm.hpp"

#include "PhotonMap.h"


float KdTree::distance2(const Photon& p1, const Photon& p2)
{
	glm::vec3 diff = p2.position - p1.position;
	return glm::dot(diff, diff);
}

void KdTree::buildNode(int* indices, int n_points, int depth)
{
if (n_points <= 0) return;

    // choose separation axis
    const int axis = depth % 3;

    // sort indices by coordination in the separation axis.
    std::sort(indices, indices + n_points, [&](const int idx1, const int idx2) {
      return points[idx1].position[axis] < points[idx2].position[axis];
    });

    // index of middle element of indices
    const int mid = (n_points - 1) / 2;

    // add node to node array, remember index of current node(parent node)
    const int parentIdx = nodes.size();
    Node node;
    node.axis = axis;
    node.idx = indices[mid];
    nodes.push_back(node);

    // add left children to node array
    const int leftChildIdx = nodes.size();
    buildNode(indices, mid, depth + 1);

    // set index of left child on parent node
    // if size of nodes doesn't change, it means there is no left children
    if (leftChildIdx == nodes.size()) {
      nodes[parentIdx].leftChildIdx = -1;
    } else {
      nodes[parentIdx].leftChildIdx = leftChildIdx;
    }

    // add right children to node array
    const int rightChildIdx = nodes.size();
    buildNode(indices + mid + 1, n_points - mid - 1, depth + 1);

    // set index of right child on parent node
    // if size of nodes doesn't change, it means there is no right children
    if (rightChildIdx == nodes.size()) {
      nodes[parentIdx].rightChildIdx = -1;
    } else {
      nodes[parentIdx].rightChildIdx = rightChildIdx;
    }
}

void KdTree::searchKNearestNode(int nodeIdx, const Photon& queryPoint, int k, std::priority_queue<std::pair<float, int>>& queue) const
{
   if (nodeIdx == -1 || nodeIdx >= nodes.size()) return;

    const Node& node = nodes[nodeIdx];

    // median point
    const Photon& median = points[node.idx];

    // push to queue
    const float dist2 = distance2(queryPoint, median);
    queue.emplace(dist2, node.idx);

    // if size of queue is larger than k, pop queue
    if (queue.size() > k) {
      queue.pop();
    }

    // if query point is lower than median, search left child
    // else, search right child
    const bool isLower = queryPoint.position[node.axis] < median.position[node.axis];
    if (isLower) {
      searchKNearestNode(node.leftChildIdx, queryPoint, k, queue);
    } else {
      searchKNearestNode(node.rightChildIdx, queryPoint, k, queue);
    }

    // at leaf node, if size of queue is smaller than k, or queue's largest
    // minimum distance overlaps sibblings region, then search siblings
    const float dist_to_siblings = median.position[node.axis] - queryPoint.position[node.axis];
    if (queue.top().first > dist_to_siblings * dist_to_siblings) {
      if (isLower) {
        searchKNearestNode(node.rightChildIdx, queryPoint, k, queue);
      } else {
        searchKNearestNode(node.leftChildIdx, queryPoint, k, queue);
      }
    }
}

void KdTree::setPoints(const Photon* points, int nPoints)
{
    this->points = points;
    this->nPoints = nPoints;
}

void KdTree::buildTree()
{
    // setup indices of points
    std::vector<int> indices(nPoints);
    std::iota(indices.begin(), indices.end(), 0);

    // build tree recursively
    buildNode(indices.data(), nPoints, 0);
}

std::vector<int> KdTree::searchKNearest(const Photon& queryPoint, int k, float& maxDist2) const
{
    KNNQueue queue;
    searchKNearestNode(0, queryPoint, k, queue);

    std::vector<int> ret(queue.size());
    maxDist2 = 0;
    for (int i = 0; i < ret.size(); ++i) {
      const auto& p = queue.top();
      ret[i] = p.second;
      maxDist2 = std::max(maxDist2, p.first);
      queue.pop();
    }

    return ret;
}
