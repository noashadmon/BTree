#include <iostream>
#include "InternalNode.h"

using namespace std;

InternalNode::InternalNode(int ISize, int LSize,
  InternalNode *p, BTreeNode *left, BTreeNode *right) :
  BTreeNode(LSize, p, left, right), internalSize(ISize)
{
  keys = new int[internalSize]; // keys[i] is the minimum of children[i]
  children = new BTreeNode*[ISize];

} // InternalNode::InternalNode()

int InternalNode::getMinimum()const
{
  if (count > 0)   // should always be the case
    return children[0]->getMinimum();
  else
    return 0;
} // InternalNode::getMinimum()


InternalNode * InternalNode::split(BTreeNode *max)
{
  InternalNode *newInNode = new InternalNode(internalSize, leafSize, parent, this, rightSibling);

  if (rightSibling)
  {
    rightSibling->setLeftSibling(newInNode);
  } // if right sibling exists it makes newnode the new left sibling and then becomes the right sibling 
  rightSibling = newInNode;

  for (int i = (internalSize + 1) / 2; i < internalSize; i++)
  {
    newInNode->children[newInNode->count] = children[i];
    newInNode->keys[newInNode->count++] = keys[i];
    children[i]->setParent(newInNode);
  } //inserts all the new elements into the new node except for value

  newInNode->children[newInNode->count] = max;
  newInNode->keys[newInNode->count++] = max->getMinimum();
  max->setParent(newInNode);
  count = (internalSize + 1) / 2;

  return newInNode;

}

int InternalNode::shift(int count, int pos)
{
  int numsAvailable;
  for (numsAvailable = count - 1; numsAvailable >= pos; numsAvailable--)
  {
    children[numsAvailable + 1] = children[numsAvailable];
    keys[numsAvailable + 1] = keys[numsAvailable];
  } //finds the position to put value in
  return numsAvailable;

}

//finds where the btree node ptr should be placed
BTreeNode * InternalNode::findPlace(BTreeNode *ptr, int pos)
{
  BTreeNode *max;
  if (pos == internalSize)
    return ptr;

  max = children[count - 1]; //sets the max
  shift(count - 1, pos); //shifts everything right
  children[pos] = ptr;
  keys[pos] = ptr->getMinimum();
  //count++;
  ptr->setParent(this);

  return max;
}

bool InternalNode::isFull(int count)
{
  if (count == internalSize)
  {
    return true;
  }
  return false;
}

void InternalNode::changeKeys(BTreeNode * newNode)
{
  for (int i = 0; i < count; i++)
  {
    if (children[i] == newNode)
    {
      keys[i] = children[i]->getMinimum();
      if (isTraverse(i, parent))
        parent->changeKeys(this);
      break;
    }
  }
} //changes keys if needed

bool InternalNode::isTraverse(int pos, BTreeNode * parent)
{
  if (pos == 0 && parent)
    return true;
  return false;
}

InternalNode* InternalNode::insert(int value) //ALSMOST DONE LOOK TO SEE WHY I BECOMES NEGATIVE COUNT >=2
{
  int i, pos;
  //count == 2;
  for (pos = count - 1; pos > 0 && keys[pos] > value; pos--); //finds where to insert

  BTreeNode* max;
  BTreeNode *newNode = children[pos]->insert(value);
	 
  if (!newNode)
  {
    return NULL;
  } // no inputs at all

  if (!isFull(count)) //(count < internalSize)//space for value
  {
    i = shift(count, pos + 1); //shifts right
    children[i + 1] = newNode;
    keys[i + 1] = newNode->getMinimum();
    count++;
    newNode->setParent(this);

    if (isTraverse(i, parent))
    {
      parent->changeKeys(this);
    }
    return NULL;
  }
	
  max = findPlace(newNode, pos + 1);

  if (leftSibling && leftSibling->getCount() < internalSize)
  {
    ((InternalNode *)leftSibling)->insert(children[0]);
    for (int pos = 0; pos < count - 1; pos++)
    {
      children[pos] = children[pos + 1];
      keys[pos] = keys[pos + 1];
    }
		//children[count - 1] = max;
  keys[count - 1] = max->getMinimum();
  max->setParent(this);
  if (parent)
    parent->changeKeys(this);
  return NULL;
  } //adoption by left
  else
  {
    if (rightSibling && rightSibling->getCount() < internalSize)
    {
      ((InternalNode *)rightSibling)->insert(max);
      if (newNode == children[0] && parent)
        parent->changeKeys(this);
      return NULL;
    } //right sibling adopts	
    else
      return split(max);
  }
} // InternalNode::insert()


void InternalNode::insert(BTreeNode *old, BTreeNode *newNode)
{ // Node must be the root, and node1
	//makes sure the siblings have pointers between each other!!!

  children[0] = old;
  children[1] = newNode;
  keys[0] = old->getMinimum();
  keys[1] = newNode->getMinimum();
  count = 2;
  for (int i = 0; i < count; i++)
  {
    if (i == 0)
    {
      children[i]->setLeftSibling(NULL);
      children[i]->setRightSibling(children[i + 1]);
    }
    else
    {
      children[i]->setLeftSibling(children[i - 1]);
      children[i]->setRightSibling(NULL);
    }
  }
  old->setParent(this);
  newNode->setParent(this);

} // InternalNode::insert()

void InternalNode::insert(BTreeNode *newNode) // from a sibling
{
  int position;
  //pass it to da left sibling
  if (newNode->getMinimum() <= keys[0])
    position = 0;
  else //pass it to the right sibling
    position = count;

  shift(count, position); //shifts
	
  children[position] = newNode;
  keys[position] = newNode->getMinimum();
  count++;
  newNode->setParent(this);

  if (isTraverse(position, parent))
    parent->changeKeys(this);
} // InternalNode::insert()

void InternalNode::print(Queue <BTreeNode*> &queue)
{
  int i;

  cout << "Internal: ";
  for (i = 0; i < count; i++)
    cout << keys[i] << ' ';
  cout << endl;

  for (i = 0; i < count; i++)
    queue.enqueue(children[i]);

} // InternalNode::print()


