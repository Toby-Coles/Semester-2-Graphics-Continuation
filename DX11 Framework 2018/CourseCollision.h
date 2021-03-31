#pragma once
#ifndef COURSE_COLLISION
#define COURSE_COLLISION


#include <vector>
#include "Vector.h"
#include <cstddef>
#include "RigidBody.h"


struct BoundingSphere
{
	Vector centre;
	float radius;

public:
	//Creates a bounding sphere
	BoundingSphere(const Vector &centre, float radius);

	//Creates a bounding sphere to enclose two given bounding spheres
	BoundingSphere(const BoundingSphere& one, const BoundingSphere& two);

	//Checks if two spheres overlap 
	bool CheckOverlap(const BoundingSphere* other)const;
};





//Stores potential contact to be checked later
struct PotentialContact
{
	RigidBody* contactBody[2];

};

//Template class for nodes in bounding bolume hierarchy
//Class uses a binary tree to store bounding volumes

template<class BoundingVolumeClass>
class BoundingHierarchyNode
{
public:
	BoundingHierarchyNode* children[2];

	//Holds a single bounding volume holding all the desendants of the node
	BoundingBolumeClass volume;

	//Holds the rigid body at this node in the hierarchy
	RigidBody* body;

	//Holds a single bounding volume holding the descendents of this ndoe
	BoundingVolumeClass volume;

	//Holds the node above (this) in the tree
	BoundingHierarchyNode* parent;

	~BoundingHierarchyNode();


	bool IsLeaf()const {
		return (body != NULL);
	}

	//Inserts a rigid body with the given bounding volume to the hierarchy.  
	void Insert(RigidBody* body const BoundingVolumeClass &volume);

	//Remove template
	template<class BoundingVolumeClass>
	BoundingHierarchyNode<BoundingVolumeClass>::~BoundingHierarchyNode()
	{
		//If there is no parent, ignore the sibling
		if (parent)
		{
			//Find sibling
			BoundingHierarchyNode<BoundingVolumeClass> *sibling;
			if (parent->children[0] == this)
			{
				sibling = parent->children[1];
			}
			else
			{
				sibling = parent->children[0];
			}

			//Write its data to the parent
			parent->volume = sibling->volue;
			parent->body = sibling->body;
			parent->children[0] = sibling->children[0];
			parent->children[1] = sibling->children[1];

			//Remove the sibling
			sibling->parent = NULL;
			sibling->body = NULL;
			sibling->children[0]= NULL;
			sibling->children[1]= NULL;
			delete sibling;

			parent->RecalculateBoundingVolume();
		}

		//Delete the children
		if (children[0])
		{
			children[0]->parent = NULL;
			delete children[0];
		}
		if (children[1])
		{
			children[1]->parent = NULL;
			delete children[1];
		}
	}

	//Insert Template
	template<class BoundingVolumeClass>
	void BoundingHierarchyNode<BoundingVolumeClass>::Insert(RigidBody* newBody, const BoundingVolumeClass& newVolume)
	{
		//If is a leaf, only option is to create two new children and place the body in one
		if (IsLeaf())
		{
			//Child one is a copy of this
			children[0] = new BoundingHierarchyNode<BoundingVolumeClass>(this, volume, body);

			//Child two holds the new rigid body
			children[1] = new new BoundingHierarchyNode<BoundingVolumeClass>(this, newVolume, newBody);

			//Now remove the body(volume is no longer a leaf)
			this->body = NULL;

			//Recalculate bounding volume
			RecalculateBoundingVolume();
		}
		//Else work out which child keeps the inserted body. Giving it to whatever would grow the least to take it on
		else
		{
			if ((children[0]->volume.GetGrowth(newVolume) <
				children[1]->volume.GetGrowth(newVolume))
			{
				children[0]->Insert(newBody, newVolume);
			}
			else
			{
				children[1]->Insert(newBody, newVolume);
			}
		}
	}


	//Checks the potential contacts from this node down in the hierarchy, adding them to an array of contacts
	//Will return the number of potential contacts if any are found
	unsigned GetPotentialcontacts(PotentialContact* contacts, unsigned limit) const;

	//Overlap Template
	template<class BoundingVolumeClass>
	bool BoundingHierarchyNode<BoundingVolumeClass>::overlaps(
		const BoundingHierarchyNode<BoundingVolumeClass>* other) const
	{
		return volume->overlaps(other->volume);
	}

	template<class BoundingVolumeClass>
	bool BoundingHierarchyNode<BoundingVolumeClass>GetPotentialcontacts(PotentialContact* contacts, unsigned limit)const;
	{
		//Early exit if there is no room for contact, or at a leaf node
		if (IsLeaf || limit == 0)
		{
			return;
		}

		//Get the potential contacts of one of our children with the other 
		return children[0]->GetPotentialContactsWith(children[1], constacts, limit);
	}

	template<class BoundingVolumeClass>
	unsigned BoundingHierarchyNode<BoundingVolumeClass>::GetPotentialContactsWith(const BoundingHierarchyNode<BoundingVolumeClass>* other, PotentialContact* contacts,
		unsigned limit)const;
	{
		//Early exit if there is no overlap or no space to report contacts
		if (!overlaps(other) ||limit == 0) 
		{
			return 0;
		}

		//If both leaf nodes, then report potential contact (return 1)
		if (IsLeaf() &&other->isLeaf())
		{
			contacts->body[0] = body;
			contacts->body[1] = other->body;
			return 1;
		}

		//Determine which node to descend into, if either is a leaf then descend the other, if both are branches we use the largest
		if (other->IsLeaf() || !IsLeaf() && volume->GetSize() >= other->volume->GetSize()))
		{
		unsigned count = children[0]->GetPotentialContactsWith(other, contacts, limit);

		//Check there is enough slots for the other side of the tree
		if (limit > count)
		{
			return count + children[1]->GetPotentialContactsWith(other, contacts + count, limit - count);
		}
		else {
			return count;
		}
		}
		else
		{
			//Recurse into other node
			unsigned count = GetPotentialContactsWith(other->children[0], contacts, limit);

			//Ensure there are enough slots to do the other side also
			if (limit > count)
			{
				return count + GetPotentialContactsWith(other->children[1], contacts + count, limit - count);

			}
			else
			{
				return count;
			}
		}
	}

private:

};


#endif