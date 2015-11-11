#ifndef LeafNodeH
#define LeafNodeH

#include "BTreeNode.h"

class LeafNode :public BTreeNode
{
  int *values;
public:
  LeafNode(int LSize, InternalNode *p, BTreeNode *left,
           BTreeNode *right);
  int getMinimum() const;
  bool isFull(int count);
  bool isTraverse(int value, BTreeNode * parent);
  int storePos(int count, int value);
  bool isSiblingAvailable(BTreeNode *sibling);
  LeafNode* split(int value, int max);
  LeafNode* insert(int value); // returns pointer to new Leaf if splits
	// else NULL
  void print(Queue <BTreeNode*> &queue);
}; //LeafNode class

#endif
