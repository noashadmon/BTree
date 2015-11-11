#ifndef InternalNodeH
#define InternalNodeH

#include "BTreeNode.h"

class InternalNode :public BTreeNode
{
	int internalSize;
	BTreeNode **children;
	int *keys;
public:
  InternalNode(int ISize, int LSize, InternalNode *p,
               BTreeNode *left, BTreeNode *right);
  int getMinimum()const; 
  bool isFull(int count);
  int shift(int count, int pos);
  InternalNode* split(BTreeNode *max);
  bool isTraverse(int position, BTreeNode * parent);
  BTreeNode * findPlace(BTreeNode *ptr, int pos);
  void changeKeys(BTreeNode *child);
  InternalNode* insert(int value); // returns pointer to new InternalNode
  // if it splits else NULL
  void insert(BTreeNode *oldRoot, BTreeNode *node2); // if root splits use this
  void insert(BTreeNode *newNode); // from a sibling
  void print(Queue <BTreeNode*> &queue);
}; // InternalNode class

#endif
