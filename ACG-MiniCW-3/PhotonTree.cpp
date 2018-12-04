#include "pch.h"
#include "PhotonTree.h"

//Returns the largest dimension (0, 1, 2) of a box
int largestBoxDim(BoundingBox* b) {
	float largestSize = -INFINITY;
	int largestDim = NULL;

	for (int d = 0; d < 3; d++) {
		const float size = b->maximum(d) - b->minimum(d);
		if (size > largestSize) {
			largestSize = size;
			largestDim = d;
		}
	}

	return largestDim;
}

// Comparator for sorting photons based on a given dimension of their position vector
struct CompareDimensions {
	CompareDimensions(int dimension) {
		this->dimension = dimension;
	}

	bool operator () (Photon* i, Photon* j) {
		return (i->position(dimension) < j->position(dimension));
	}

	int dimension;
};


BoundingBox getBoundingBox(std::vector<Photon*> photons)
{
	BoundingBox b;

	b.maximum = Vec3(-INFINITY, -INFINITY, -INFINITY);
	b.minimum = Vec3(INFINITY, INFINITY, INFINITY);

	for (auto const& photonPtr : photons) {
		const Vec3* point = &(photonPtr->position);

		for (int d = 0; d < 3; d++) {
			if ((*point)(d) < b.minimum(d)) {
				b.minimum(d) = (*point)(d);
			}

			if ((*point)(d) > b.maximum(d)) {
				b.maximum(d) = (*point)(d);
			}
		}
	}

	return b;
}

//Returns a slice of a vector
//template <typename T>
std::vector<Photon*> sliceVec(int startPos, int endPos, std::vector<Photon*> vector) {
	std::vector<Photon*> out;

	for (int i = startPos; i <= endPos; i++) {
		out.push_back(vector[i]);
	}

	return out;
}

//Builds a balanced KD-Tree, stored as an array
//Head node nodeID must be 1
//Left child nodeID is 2N
//Right child nodeID is 2N + 1
void PhotonTree::buildBalancedTree(std::vector<Photon*> photons, int nodeID)
{
	assert(nodeID != 0);

	BoundingBox boundingBox = getBoundingBox(photons); //Bounds of all photon positions

	//Sort points by size in largest dimension
	const int largestDim = largestBoxDim(&boundingBox);
	sort(photons.begin(), photons.end(), CompareDimensions(largestDim));

	//This node becomes median
	const int medianArrayPos = photons.size() / 2;
	
	this->photons[nodeID] = photons.at(medianArrayPos);
	this->nodeSplittingDimension[nodeID] = largestDim;

	if (photons.size() > 1) {
		if (medianArrayPos - 1 >= 0) {
			//Left tree becomes all photons less than median
			this->buildBalancedTree(sliceVec(0, medianArrayPos - 1, photons), this->getLeftChildID(nodeID));
		}
		else {
			this->setLeftChild(nodeID, NULL);
		}

		if (medianArrayPos + 1 < photons.size()) {
			//Right tree becomes all photons greater than median
			this->buildBalancedTree(sliceVec(medianArrayPos + 1, photons.size() - 1, photons), this->getRightChildID(nodeID));
		}
		else {
			this->setRightChild(nodeID, NULL);
		}
	}
}

PhotonTree::PhotonTree(std::vector<Photon*> photonPtrs)
{
	this->size = photonPtrs.size();
	const int maxNodeID = (2 * this->size) + 1;
	this->nodeSplittingDimension = (int*)malloc(sizeof(int) * maxNodeID);
	this->photons = (Photon**) malloc(sizeof(Photon*) * maxNodeID);

	this->buildBalancedTree(photonPtrs, 1);
}

std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> PhotonTree::findNearestNeighbours(Vec3 position, int numberOfNeighbours)
{
	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> queue;

	this->locatePhotons(position, INFINITY, 1, &queue, numberOfNeighbours);

	return queue;
}

int PhotonTree::getSize()
{
	return this->size;
}

PhotonTree::~PhotonTree()
{
	free(this->photons);
	free(this->nodeSplittingDimension);
}

Photon * PhotonTree::getLeftChild(int node, int * nodeId)
{
	*nodeId = this->getLeftChildID(node);
	return this->getNode(*nodeId);
}

Photon * PhotonTree::getRightChild(int node, int * nodeId)
{
	*nodeId = this->getRightChildID(node);
	return this->getNode(*nodeId);
}

Photon * PhotonTree::getNode(int node)
{
	return this->photons[node];
}

void PhotonTree::setNode(int node, Photon * photonPtr)
{
	this->photons[node] = photonPtr;
}

int PhotonTree::setLeftChild(int node, Photon * photonPtr)
{
	const int childNodeId = this->getLeftChildID(node);
	this->setNode(childNodeId, photonPtr);
	return childNodeId;
}

int PhotonTree::setRightChild(int node, Photon * photonPtr)
{
	const int childNodeId = this->getRightChildID(node);
	this->setNode(childNodeId, photonPtr);
	return childNodeId;
}

int PhotonTree::getRightChildID(int nodeId)
{
	return (nodeId * 2) + 1;
}

int PhotonTree::getLeftChildID(int nodeId)
{
	return nodeId * 2;
}

void PhotonTree::locatePhotons(Vec3 position, float squaredMaxDistance, int nodeID, std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare>* queue, int numNeighbours)
{
	//Get node
	Photon* currentNode = this->getNode(nodeID);

	//TODO make sure this check is accurate
	const bool hasChildren = this->getRightChildID(nodeID) < this->getSize();

	//go left or right until no children
	if (hasChildren) {
		//get splitting dim
		const int splittingDim = this->nodeSplittingDimension[nodeID];

		//get distance from hyperplane
		const float hyperPlaneDistance = position(splittingDim) - currentNode->position(splittingDim);
		const float squaredHyperPlaneDistance = pow(hyperPlaneDistance, 2);

		if (hyperPlaneDistance < 0) {
			//Go left
			this->locatePhotons(position, squaredMaxDistance, this->getLeftChildID(nodeID), queue, numNeighbours);
			if (squaredHyperPlaneDistance < squaredMaxDistance) {
				this->locatePhotons(position, squaredMaxDistance, this->getRightChildID(nodeID), queue, numNeighbours);
			}
		}
		else {
			//Go right
			this->locatePhotons(position, squaredMaxDistance, this->getRightChildID(nodeID), queue, numNeighbours);
			if (squaredHyperPlaneDistance < squaredMaxDistance) {
				this->locatePhotons(position, squaredMaxDistance, this->getLeftChildID(nodeID), queue, numNeighbours);
			}
		}
	}

	//Get squared distance of current node from search position
	currentNode->squaredDistFromSearchPoint = (position - currentNode->position).squaredNorm();


	if (currentNode->squaredDistFromSearchPoint < squaredMaxDistance) {
		//Add to max heap h
		queue->push(currentNode);

		//Adjust max distance if heap is full to best in the queue
		if (queue->size() >= numNeighbours) {
			//New max distance is the same as the photon in the queue which is furthest away
			squaredMaxDistance = (queue->top()->position - position).squaredNorm();
		}
	}

}
