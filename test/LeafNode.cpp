#include <iostream>
#include "LeafNode.h"
#include "InternalNode.h"
#include "QueueAr.h"

using namespace std;


LeafNode::LeafNode(int LSize, InternalNode *p,
	BTreeNode *left, BTreeNode *right) : BTreeNode(LSize, p, left, right)
{

  values = new int[LSize];
}  // LeafNode()

int LeafNode::getMinimum()const
{
  if (count > 0)  // should always be the case
    return values[0];
  else
    return 0;

} // LeafNode::getMinimum()


LeafNode * LeafNode::split(int value, int max)
{
  LeafNode *newLeafNode = new LeafNode(leafSize, parent, this, rightSibling);

  if (rightSibling)
  {
    rightSibling->setLeftSibling(newLeafNode);
  } //if there was a right sibling it sets the newleaf to be its left sibling

  rightSibling = newLeafNode; //rightsibling is now the newleaf

  for (int i = (leafSize + 1) / 2; i < leafSize; i++){
    newLeafNode->values[newLeafNode->count++] = values[i];
  } //inserts all the new elements into the new node except for value

  newLeafNode->values[newLeafNode->count++] = max;
  count = (leafSize + 1) / 2;

  if (isTraverse(value, parent))
  {
    parent->changeKeys(this);
  }

  return newLeafNode;
}

bool LeafNode:: isTraverse(int value, BTreeNode * parent)
{
  if (value == values[0] && parent)
    return true;
  return false;
}

bool LeafNode::isFull(int count)
{
  if (count == leafSize)
  {
    return true;
  }

  return false;
}

int LeafNode::storePos(int count, int value)
{
  int numsAvailable;
  for (numsAvailable = count - 1; numsAvailable >= 0 && values[numsAvailable] > value; numsAvailable--)
  {
    values[numsAvailable + 1] = values[numsAvailable];
  } //finds the position to put value in
  return numsAvailable;
}
LeafNode* LeafNode::insert(int value)
{
  int numsAvailable, max;

  if (!isFull(count))
  {
    numsAvailable = storePos(count, value);
    values[numsAvailable + 1] = value;
    count++;

    if (isTraverse(value, parent))
      parent->changeKeys(this);
    return NULL;
  } //inserts if space in current node

  if (value > values[count - 1])
  {
    max = value;
  } //sets max
  else
  {
    max = values[count - 1];
    numsAvailable = storePos(count - 1, value);
    values[numsAvailable + 1] = value;
  }

	//if--else looks left first then right
  if (leftSibling && leftSibling->getCount() < leafSize)
  {

    leftSibling->insert(values[0]);
    for (int i = 0; i < count - 1; i++)
      values[i] = values[i + 1];

    values[count - 1] = max;
    if (parent)
      parent->changeKeys(this);
    return NULL;
  } //left sibling  adopts if room

  else
  {
    if (rightSibling && rightSibling->getCount() < leafSize)
    {
      rightSibling->insert(max);
      if (isTraverse(value, parent)) //value == values[0] && parent)
        parent->changeKeys(this);
      return NULL;
    } //right sibling adopts
    else
      return split(value, max);
  }
}  // LeafNode::insert()

void LeafNode::print(Queue <BTreeNode*> &queue)
{
  cout << "Leaf: ";
  for (int i = 0; i < count; i++)
    cout << values[i] << ' ';
  cout << endl;
} // LeafNode::print()
