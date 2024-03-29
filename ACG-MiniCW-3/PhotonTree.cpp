#include "pch.h"
#include "PhotonTree.h"

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


Box PhotonTree::getBoundingBox(std::vector<Photon*> photons)
{
	Box b;

	for (auto const& photonPtr : photons) {
		b.expand(photonPtr->position);
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

	Box boundingBox = getBoundingBox(photons); //Bounds of all photon positions

	const int medianArrayPos = photons.size() / 2;

	//Sort points by size in largest dimension
	const int largestDim = boundingBox.largestDim();



	//Sort the array just until middle (median) posision is correct.
	//All elems preceding median are less than it, and all following median are greater than it.
	//Much faster than sort() with same eventual outcome (almost 2x speed)
	std::nth_element(photons.begin(), photons.begin() + medianArrayPos, photons.end(), CompareDimensions(largestDim)); 

	
	//This node becomes median
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
	this->num_photons = photonPtrs.size();
	this->maxNodeID = (2 * this->num_photons) + 1;
	this->nodeSplittingDimension = (int*)malloc(sizeof(int) * this->maxNodeID);
	this->photons = (Photon**) malloc(sizeof(Photon*) * this->maxNodeID);
	for (int i = 0; i < this->maxNodeID; i++) {
		this->photons[i] = NULL;
	}
	this->buildBalancedTree(photonPtrs, 1);
}

std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> PhotonTree::findNearestNeighbours(Vec3 position, int numberOfNeighbours)
{

	MaximumDistanceCompare compare(position);
	std::priority_queue<Photon*, std::vector<Photon*>, MaximumDistanceCompare> queue(compare);

	this->locatePhotons(position, INFINITY, 1, &queue, numberOfNeighbours);

	return queue;
}

int PhotonTree::getSize()
{
	return this->num_photons;
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
	if (node < 1) {
		std::cerr << "Error: KD-Tree root node is 1. Cannot access below 1. Attempted to access: " << node << std::endl;
		exit(1);
	}

	if (node >= this->maxNodeID) {
		//std::cerr << "Error: KD-Tree out of bounds: Min 1  Max maxNodeID-1" << std::endl;
		return NULL;
	}

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
	//const bool hasChildren = this->nodeHasChildren[nodeID];//this->getRightChildID(nodeID) < this->getSize();
	int i;
	Photon* leftChild = this->getLeftChild(nodeID, &i);
	Photon* rightChild = this->getRightChild(nodeID, &i);
	const bool hasLeftChild = (leftChild != NULL);
	const bool hasRightChild = (rightChild != NULL);

	//Get squared distance of current node from search position
	const float squaredDistFromSearchPoint = (position - currentNode->position).squaredNorm();


	if (squaredDistFromSearchPoint < squaredMaxDistance) {
		//Add to max heap h
		queue->push(currentNode);

		//Set new maxDistance to worst from heap
		if (queue->size() > numNeighbours) {
			//Remove worst from heap
			queue->pop();

			//New max distance is the same as the photon in the queue which is furthest away
			squaredMaxDistance = (queue->top()->position - position).squaredNorm();
		}
	}

	//go left or right until no children
	if (hasLeftChild || hasRightChild) {
		//get splitting dim
		const int splittingDim = this->nodeSplittingDimension[nodeID];

		//get distance from hyperplane
		const float hyperPlaneDistance = position(splittingDim) - currentNode->position(splittingDim);
		const float squaredHyperPlaneDistance = pow(hyperPlaneDistance, 2);

		if (hyperPlaneDistance < 0) {
			if (hasLeftChild) {
				//Go left
				this->locatePhotons(position, squaredMaxDistance, this->getLeftChildID(nodeID), queue, numNeighbours);
			}
			if (squaredHyperPlaneDistance < squaredMaxDistance && hasRightChild) {
				this->locatePhotons(position, squaredMaxDistance, this->getRightChildID(nodeID), queue, numNeighbours);
			}
		}
		else if (hasRightChild) {
			//Go right
			this->locatePhotons(position, squaredMaxDistance, this->getRightChildID(nodeID), queue, numNeighbours);
			if (squaredHyperPlaneDistance < squaredMaxDistance && hasLeftChild) {
				this->locatePhotons(position, squaredMaxDistance, this->getLeftChildID(nodeID), queue, numNeighbours);
			}
		}
	}
}
