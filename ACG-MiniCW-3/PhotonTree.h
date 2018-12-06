#pragma once
#include "Photon.h"
#include <queue>
typedef struct BoundingBox {
	Vec3 maximum;
	Vec3 minimum;
} BoundingBox;

// Comparator for selecting the photon with the maximum distance from the specified point
class MaximumDistanceCompare
{
public:
	bool operator() (Photon* i, Photon* j)
	{
		return i->squaredDistFromSearchPoint < j->squaredDistFromSearchPoint;
	}
};

class PhotonTree
{
public:
	PhotonTree(std::vector<Photon*> photons);
	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> findNearestNeighbours(Vec3 position, int numberOfNeighbours);
	int getSize();
	virtual ~PhotonTree();

private:
	Photon** photons;
	int* nodeSplittingDimension;
	int num_photons;
	int maxNodeID;
	Photon* getLeftChild(int node, int* nodeId);
	Photon* getRightChild(int node, int* nodeId);
	Photon* getNode(int node);
	void setNode(int node, Photon* photonPtr);
	int setLeftChild(int node, Photon* photonPtr);
	int setRightChild(int node, Photon* photonPtr);
	int getRightChildID(int nodeId);
	int getLeftChildID(int nodeId);
	void locatePhotons(Vec3 position, float squaredMaxDistance, int nodeID, std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare>* queue, int numNeighbours);

	void buildBalancedTree(std::vector<Photon*> photons, int index);
};

