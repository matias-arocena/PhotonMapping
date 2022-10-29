#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <string>
#include "glm/glm.hpp"

struct Photon;

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
	std::vector<std::shared_ptr<Photon>> points;     // pointer to array of points

	static float distance2(const glm::vec3& p1, std::shared_ptr<Photon> p2);
	void buildNode(int* indices, int n_points, int depth);
	void searchKNearestNode(int nodeIdx, const glm::vec3& queryPoint, int k, KNNQueue& queue) const;
	void searchKNearestNode(int nodeIdx, const glm::vec3& queryPoint, float radius, KNNQueue& queue) const;
public:
	KdTree(){}
	void setPoints(std::vector<std::shared_ptr<Photon>> points);
	void buildTree();
	std::vector<int> searchKNearest(const glm::vec3& queryPoint, int k, float& maxDist2) const;
	std::vector<int> searchKNearest(const glm::vec3& queryPoint, float radius, float& maxDist2) const;

	void saveKdTreeToFile(const std::string& path);
};
