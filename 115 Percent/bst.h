/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    <your names here>
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
template <typename T>
class BST
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class TT>
   friend class custom::set;

   template <class KK, class VV>
   friend class custom::map;
public:
   //
   // Construct
   //

   BST();
   BST(const BST &  rhs);
   BST(      BST && rhs);
   BST(const std::initializer_list<T>& il);
   ~BST();

   //
   // Assign
   //

   BST & operator = (const BST &  rhs);
   BST & operator = (      BST && rhs);
   BST & operator = (const std::initializer_list<T>& il);
   void swap(BST & rhs);


   //
   // Iterator
   //

   class iterator;
   iterator   begin() const noexcept;
   iterator   end()   const noexcept { return iterator(nullptr); }

   //
   // Access
   //

   iterator find(const T& t);

   //
   // Insert
   //

   std::pair<iterator, bool> insert(const T&  t, bool keepUnique = false);
   std::pair<iterator, bool> insert(      T&& t, bool keepUnique = false);

   //
   // Remove
   //

   iterator erase(iterator& it);
   void   clear() noexcept;

   //
   // Status
   //

   bool   empty() const noexcept { return size() == 0; }
   size_t size()  const noexcept { return numElements;   }
   
private:

   class BNode;

   void deleteNode(BNode*& pDelete, bool toRight);
   void deleteBinaryTree(BNode*& pDelete) noexcept;
   void copyBinaryTree(const BNode* pSrc, BNode *& pDest);


   BNode * root;              // root node of the binary search tree
   size_t numElements;        // number of elements currently in the tree
};


/*****************************************************************
 * BINARY NODE
 * A single node in a binary tree. Note that the node does not know
 * anything about the properties of the tree so no validation can be done.
 *****************************************************************/
template <typename T>
class BST <T> :: BNode
{
public:
   //
   // Construct
   //
   BNode(): pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(), isRed(true)
   {
   }
   BNode(const T &  t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(t), isRed(true)
   {
   }
   BNode(T && t) : pLeft(nullptr), pRight(nullptr), pParent(nullptr), data(std::move(t)), isRed(true)
   {

   }

   //
   // Insert
   //
   void addLeft (BNode * pNode);
   void addRight(BNode * pNode);
   void addLeft (const T &  t);
   void addRight(const T &  t);
   void addLeft(       T && t);
   void addRight(      T && t);




   //
   // Status
   //
   bool isRightChild(BNode * pNode) const { return pRight == pNode; }
   bool isLeftChild( BNode * pNode) const { return pLeft == pNode; }

   // balance the tree
   void balance();

#ifdef DEBUG
   //
   // Verify
   //
   std::pair <T,T> verifyBTree() const;
   int findDepth() const;
   bool verifyRedBlack(int depth) const;
   int computeSize() const;
#endif // DEBUG

   //
   // Data
   //
   T data;                  // Actual data stored in the BNode
   BNode* pLeft;          // Left child - smaller
   BNode* pRight;         // Right child - larger
   BNode* pParent;        // Parent
   bool isRed;              // Red-black balancing stuff
};

/**********************************************************
 * BINARY SEARCH TREE ITERATOR
 * Forward and reverse iterator through a BST
 *********************************************************/
template <typename T>
class BST <T> :: iterator
{
   friend class ::TestBST; // give unit tests access to the privates
   friend class ::TestSet;
   friend class ::TestMap;

   template <class KK, class VV>
   friend class custom::map;
public:
   // constructors and assignment
   iterator(BNode * p = nullptr)
   {
      pNode = p;
   }
   iterator(const iterator & rhs)
   {
      pNode = rhs.pNode;
   }
   iterator & operator = (const iterator & rhs)
   {
      pNode = rhs.pNode;
      return *this;
   }

   // compare
   bool operator == (const iterator & rhs) const
   {
      return rhs.pNode == pNode;
   }
   bool operator != (const iterator & rhs) const
   {
      return rhs.pNode != pNode;
   }

   // de-reference. Cannot change because it will invalidate the BST
   const T & operator * () const
   {
      return pNode->data;
   }

   // increment and decrement
   iterator & operator ++ ();
   iterator   operator ++ (int postfix)
   {
      iterator itReturn = *this;
      ++(*this);
      return itReturn;
   }
   iterator & operator -- ();
   iterator   operator -- (int postfix)
   {
      iterator itReturn = *this;
      --(*this);
      return itReturn;
   }

   // must give friend status to remove so it can call getNode() from it
   friend BST <T> :: iterator BST <T> :: erase(iterator & it);

private:
   
    // the node
    BNode * pNode;
};


/*********************************************
 *********************************************
 *********************************************
 ******************** BST ********************
 *********************************************
 *********************************************
 *********************************************/


 /*********************************************
  * BST :: DEFAULT CONSTRUCTOR
  ********************************************/
template <typename T>
BST <T> ::BST() : root(nullptr), numElements(0)
{

}

/*********************************************
 * BST :: COPY CONSTRUCTOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST ( const BST<T>& rhs) : root(nullptr), numElements(0)
{
   *this = rhs;
}

/*********************************************
 * BST :: MOVE CONSTRUCTOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> :: BST(BST <T> && rhs) : root(nullptr), numElements(0)
{
   root = rhs.root;
   rhs.root = nullptr;

   numElements = rhs.numElements;
   rhs.numElements = 0;

}

/*********************************************
 * BST :: DESTRUCTOR
 ********************************************/
template <typename T>
BST <T> :: ~BST()
{
   clear();
}


/*********************************************
 * BST :: ASSIGNMENT OPERATOR
 * Copy one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const BST <T> & rhs)
{
   copyBinaryTree(rhs.root, this->root) ;
   assert(nullptr == this->root || this->root->pParent == nullptr);
   
   this->numElements = rhs.numElements;
   
   
   return *this;
}

/*********************************************
 * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
 * Copy nodes onto a BTree
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (const std::initializer_list<T>& il)
{
   
   deleteBinaryTree(root);
   numElements = 0;
   
   for (auto&& element : il)
   {
      insert(element);
   }

      
   return *this;
}

/*********************************************
 * BST :: ASSIGN-MOVE OPERATOR
 * Move one tree to another
 ********************************************/
template <typename T>
BST <T> & BST <T> :: operator = (BST <T> && rhs)
{
   
   clear();

   swap(rhs);
   
   return *this;
}

/*********************************************
 * BST :: SWAP
 * Swap two trees
 ********************************************/
template <typename T>
void BST <T> :: swap (BST <T>& rhs)
{
   std::swap(rhs.root, root);
   std::swap(rhs.numElements, numElements);
}

/*****************************************************
 * BST :: INSERT
 * Insert a node at a given location in the tree
 ****************************************************/
template <typename T>
std::pair<typename BST <T> :: iterator, bool> BST <T> :: insert(const T & t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);

   try
   {
      if (root == nullptr)
      {
         assert(numElements == 0);
         root = new BNode(t);
         root->isRed = false;
         numElements = 1;
         pairReturn.first = iterator(root);
         pairReturn.second = true;
         return pairReturn;
      }


      BNode* node = root;
      bool done = false;

      while(!done)
      {
         if (keepUnique && t == node->data)
         {
            pairReturn.first = iterator(node);
            pairReturn.second = false;
            return pairReturn;
         }

         if (t < node->data)
         {
            if (node->pLeft)
            {
               node = node->pLeft;
            }
            else
            {
               node->addLeft(t);
               done = true;
               pairReturn.first = iterator(node->pLeft);
               pairReturn.second = true;
            }
         }
         else
         {
            if (node->pRight)
            {
               node = node->pRight;
            }
            else
            {
               node->addRight(t);
               done = true;
               pairReturn.first = iterator(node->pRight);
               pairReturn.second = true;
            }
         }
      }

      assert(root != nullptr);
      numElements++;
      

      while (root->pParent != nullptr)
      {
         root = root->pParent;
      }
      assert(root->pParent == nullptr);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
   return pairReturn;



}

template <typename T>
std::pair<typename BST <T> ::iterator, bool> BST <T> ::insert(T && t, bool keepUnique)
{
   std::pair<iterator, bool> pairReturn(end(), false);
   try
   {
      if (root == nullptr)
      {
         assert(numElements == 0);
         root = new BNode(std::move(t));
         root->isRed = false;
         numElements = 1;
         pairReturn.first = iterator(root);
         pairReturn.second = true;
         return pairReturn;
      }


      BNode* node = root;
      bool done = false;
      while (!done)
      {
         if (keepUnique && t == node->data)
         {
            pairReturn.first = iterator(node);
            pairReturn.second = false;
            return pairReturn;
         }

         if (t < node->data)
         {
            if (node->pLeft)
            {
               node = node->pLeft;
            }
            else
            {
               node->addLeft(std::move(t));
               done = true;
               pairReturn.first = iterator(node->pLeft);
               pairReturn.second = true;
            }
         }
         else
         {
            if (node->pRight)
            {
               node = node->pRight;
            }
            else
            {
               node->addRight(std::move(t));
               done = true;
               pairReturn.first = iterator(node->pRight);
               pairReturn.second = true;
            }
         }
      }

      assert(root != nullptr);
      numElements++;


      while (root->pParent != nullptr)
      {
         root = root->pParent;
      }
      assert(root->pParent == nullptr);
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
   return pairReturn;
}

/*************************************************
 * BST :: ERASE
 * Remove a given node as specified by the iterator
 ************************************************/
template <typename T>
typename BST <T> ::iterator BST <T> :: erase(iterator & it)
{
   if (it == end())
   {
      return end();
   }


   iterator itNext = it;
   BNode* pDelete = it.pNode;


   if (pDelete->pLeft == nullptr)
   {
      ++itNext;
      deleteNode(pDelete, true);
   }

   else if (pDelete->pRight == nullptr)
   {
      ++itNext;
      deleteNode(pDelete, false);
   }

   else
   {
      BNode* pIOS = pDelete->pRight;
      while (pIOS->pLeft != nullptr)
      {
         pIOS = pIOS->pLeft;
      }

      assert(pIOS->pLeft == nullptr);
      pIOS->pLeft = pDelete->pLeft;

      if (pDelete->pLeft)
      {
         pDelete->pLeft->pParent = pIOS;
      }



      if (pDelete->pRight != pIOS)
      {
         if (pIOS->pRight)
         {
            pIOS->pRight->pParent = pIOS->pParent;
         }
         pIOS->pParent->pLeft = pIOS -> pRight;


         assert(pDelete->pRight != nullptr);
         pIOS->pRight = pDelete->pRight;
         pDelete->pRight->pParent = pIOS;
      }



      pIOS->pParent = pDelete->pParent;

      if (pDelete->pParent && pDelete->pParent->pLeft == pDelete)
      {
         pDelete->pParent->pLeft = pIOS;
      }
      if (pDelete->pParent && pDelete->pParent->pRight == pDelete)
      {
         pDelete->pParent->pRight = pIOS;
      }

      if (root == pDelete)
      {
         root = pIOS;
      }

      itNext = iterator(pIOS);
   }


   numElements--;
   delete pDelete;
   return itNext;
}

/*****************************************************
 * BST :: CLEAR
 * Removes all the BNodes from a tree
 ****************************************************/
template <typename T>
void BST <T> ::clear() noexcept
{

   if (root)
   {
      deleteBinaryTree(root);
   }
   numElements = 0;

}

/*****************************************************
 * BST :: BEGIN
 * Return the first node (left-most) in a binary search tree
 ****************************************************/
template <typename T>
typename BST <T> :: iterator custom :: BST <T> :: begin() const noexcept
{
   
   
   if (root == nullptr)
   {
      return end();
   }
   
   BNode* p = root;
   while (p->pLeft)
   {
      p = p->pLeft;
   }
   
   
   
   return iterator(p);
}


/****************************************************
 * BST :: FIND
 * Return the node corresponding to a given value
 ****************************************************/
template <typename T>
typename BST <T> :: iterator BST<T> :: find(const T & t)
{
   
   for (BNode* p = root; p != nullptr; p = (t < p->data ? p->pLeft: p->pRight) )
   {
      if (p->data == t)
      {
         return iterator(p);
      }
   }
   
   
   return end();
}



template<typename T>
void BST<T>::deleteBinaryTree(BNode*  &pDelete ) noexcept
{

   if (pDelete == nullptr)
   {
      return;
   }
   deleteBinaryTree(pDelete->pLeft);
   deleteBinaryTree(pDelete->pRight);

   delete pDelete;
   pDelete = nullptr;

}


template <typename T>
void BST <T> ::copyBinaryTree(const BNode* pSrc, BNode *& pDest)
{
   if (nullptr == pSrc)
   {
      deleteBinaryTree(pDest);
      return;
   }

   assert(pSrc);


   try
   {
      if (pDest == nullptr)
      {
         pDest = new BST::BNode(pSrc->data);
      }
      else
      {
         pDest->data = pSrc->data;
      }
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
   assert(pDest != nullptr);


   pDest->isRed = pSrc->isRed;

   copyBinaryTree(pSrc->pLeft, pDest->pLeft);
   if (pSrc->pLeft)
   {
      pDest->pLeft->pParent = pDest;
   }

   copyBinaryTree(pSrc->pRight, pDest->pRight);
   if (pDest->pRight)
   {
      pDest->pRight->pParent = pDest;
   }


}



template <typename T>
void BST < T> ::deleteNode(BNode*& pDelete, bool toRight)
{
   BNode* pNext = (toRight ? pDelete->pRight : pDelete->pLeft);

   if (pDelete != root)
   {
      if (pDelete->pParent->pLeft == pDelete)
      {
         pDelete->pParent->pLeft = nullptr;
         pDelete->pParent->addLeft(pNext);
      }
      else
      {
         pDelete->pParent->pRight = nullptr;
         pDelete->pParent->addRight(pNext);
      }

   }
   else
   {
      root = pNext;
      pNext->pParent = nullptr;
   }

}


/******************************************************
 ******************************************************
 ******************************************************
 *********************** B NODE ***********************
 ******************************************************
 ******************************************************
 ******************************************************/

 
/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addLeft (BNode * pNode)
{
   pLeft = pNode;
   if (pNode)
   {
      pNode->pParent = this;
   }



}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (BNode * pNode)
{
   pRight = pNode;
   if (pNode)
   {
      pNode->pParent = this;
   }
}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> :: BNode :: addLeft (const T & t)
{
   assert(pLeft == nullptr);

   try
   {
      BNode* pNode = new BNode(t);
      addLeft(pNode);
      pNode->balance();
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }



}

/******************************************************
 * BINARY NODE :: ADD LEFT
 * Add a node to the left of the current node
 ******************************************************/
template <typename T>
void BST<T> ::BNode::addLeft(T && t)
{

   assert(pLeft == nullptr);

   try
   {
      BNode* pNode = new BNode(std::move(t));
      addLeft(pNode);
      pNode->balance();
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }


}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: addRight (const T & t)
{
   assert(pRight == nullptr);

   try
   {
      BNode* pNode = new BNode(t);
      addRight(pNode);
      pNode->balance();
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

/******************************************************
 * BINARY NODE :: ADD RIGHT
 * Add a node to the right of the current node
 ******************************************************/
template <typename T>
void BST <T> ::BNode::addRight(T && t)
{
   assert(pRight == nullptr);

   try
   {
      BNode* pNode = new BNode(std::move(t));
      addRight(pNode);
      pNode->balance();
   }
   catch (...)
   {
      throw "ERROR: Unable to allocate a node";
   }
}

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
template <typename T>
int BST <T> :: BNode :: findDepth() const
{
   // if there are no children, the depth is ourselves
   if (pRight == nullptr && pLeft == nullptr)
      return (isRed ? 0 : 1);

   // if there is a right child, go that way
   if (pRight != nullptr)
      return (isRed ? 0 : 1) + pRight->findDepth();
   else
      return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
 * BINARY NODE :: VERIFY RED BLACK
 * Do all four red-black rules work here?
 ***************************************************/
template <typename T>
bool BST <T> :: BNode :: verifyRedBlack(int depth) const
{
   bool fReturn = true;
   depth -= (isRed == false) ? 1 : 0;

   // Rule a) Every node is either red or black
   assert(isRed == true || isRed == false); // this feels silly

   // Rule b) The root is black
   if (pParent == nullptr)
      if (isRed == true)
         fReturn = false;

   // Rule c) Red nodes have black children
   if (isRed == true)
   {
      if (pLeft != nullptr)
         if (pLeft->isRed == true)
            fReturn = false;

      if (pRight != nullptr)
         if (pRight->isRed == true)
            fReturn = false;
   }

   // Rule d) Every path from a leaf to the root has the same # of black nodes
   if (pLeft == nullptr && pRight && nullptr)
      if (depth != 0)
         fReturn = false;
   if (pLeft != nullptr)
      if (!pLeft->verifyRedBlack(depth))
         fReturn = false;
   if (pRight != nullptr)
      if (!pRight->verifyRedBlack(depth))
         fReturn = false;

   return fReturn;
}


/******************************************************
 * VERIFY B TREE
 * Verify that the tree is correctly formed
 ******************************************************/
template <typename T>
std::pair <T, T> BST <T> :: BNode :: verifyBTree() const
{
   // largest and smallest values
   std::pair <T, T> extremes;
   extremes.first = data;
   extremes.second = data;

   // check parent
   if (pParent)
      assert(pParent->pLeft == this || pParent->pRight == this);

   // check left, the smaller sub-tree
   if (pLeft)
   {
      assert(!(data < pLeft->data));
      assert(pLeft->pParent == this);
      pLeft->verifyBTree();
      std::pair <T, T> p = pLeft->verifyBTree();
      assert(!(data < p.second));
      extremes.first = p.first;

   }

   // check right
   if (pRight)
   {
      assert(!(pRight->data < data));
      assert(pRight->pParent == this);
      pRight->verifyBTree();

      std::pair <T, T> p = pRight->verifyBTree();
      assert(!(p.first < data));
      extremes.second = p.second;
   }

   // return answer
   return extremes;
}

/*********************************************
 * COMPUTE SIZE
 * Verify that the BST is as large as we think it is
 ********************************************/
template <typename T>
int BST <T> :: BNode :: computeSize() const
{
   return 1 +
      (pLeft  == nullptr ? 0 : pLeft->computeSize()) +
      (pRight == nullptr ? 0 : pRight->computeSize());
}
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
template <typename T>
void BST <T> :: BNode :: balance()
{
   // Case 1: if we are the root, then color ourselves black and call it a day.
   if (pParent == nullptr)
   {
      isRed = false;
      return;
   }

   // Case 2: if the parent is black, then there is nothing left to do
   if (pParent->isRed == false)
   {
      return;
   }

   assert(pParent->pParent != nullptr);

   BNode* pGranny = pParent->pParent;
   BNode* pGreatG = pGranny->pParent;
   BNode* pSibling = pParent->isRightChild(this) ? pParent->pLeft : pParent->pRight;
   BNode* pAunt = pGranny->isRightChild(pParent) ? pGranny->pLeft : pGranny->pRight;



   assert(pGranny != nullptr);
   assert(pGranny->isRed == false);

   // Case 3: if the aunt is red, then just recolor


   if (pAunt != nullptr && pAunt->isRed == true)
   {
      pGranny->isRed = true;
      pParent->isRed = false;
      pAunt->isRed = false;
      pGranny->balance();
      return;
   }



   // Case 4: if the aunt is black or non-existant, then we need to rotate

   assert(pParent->isRed == true && pGranny->isRed == false && (pAunt == nullptr || pAunt->isRed == false));

   BNode* pHead = nullptr;

   // Case 4a: We are mom's left and mom is granny's left

   if (pParent->isLeftChild(this) && pGranny->isLeftChild(pParent))
   {
      assert(pParent->pLeft == this);
      assert(pGranny->pRight == pAunt);
      assert(pGranny->isRed == false);

      pParent->addRight(pGranny);
      pGranny->addLeft(pSibling);
      pHead = pParent;


      pParent->isRed = false;
      pGranny->isRed = true;

   }

   // case 4b: We are mom's right and mom is granny's right
   
   else if (pParent->isRightChild(this) && pGranny->isRightChild(pParent))
   {
      assert(pParent->pRight == this);
      assert(pGranny->pLeft == pAunt);
      assert(pGranny->isRed == false);

      pParent->addLeft(pGranny);
      pGranny->addRight(pSibling);
      pHead = pParent;


      pParent->isRed = false;
      pGranny->isRed = true;
   }

   // Case 4c: We are mom's right and mom is granny's left

   else if (pParent->isRightChild(this) && pGranny->isLeftChild(pParent))
   {
      //assert(pParent->pRight == pAunt);
      //assert(pGranny->pLeft == pSibling);
      //assert(pGranny->isRed == true);


      pGranny->addLeft(this->pRight);
      pParent->addRight(this->pLeft);
      this->addRight(pGranny);
      this->addLeft(pParent);

      pHead = this;
      this->isRed = false;
      pGranny->isRed = true;
   }

   // case 4d: we are mom's left and mom is granny's right
   else if (pParent->isLeftChild(this) && pGranny->isRightChild(pParent))
   {
      //assert(pParent->pLeft == pAunt);
      //assert(pGranny->pRight == pParent);
      //assert(pParent->pRight == pSibling);


      pGranny->addRight(this->pLeft);
      pParent->addLeft(this->pRight);
      this->addLeft(pGranny);
      this->addRight(pParent);

      pHead = this;
      this->isRed = false;
      pGranny->isRed = true;
   }
   else
   {
      assert(false);
   }

   if (pGreatG == nullptr)
   {
      pHead->pParent = nullptr;
   }
   else if (pGreatG->pRight == pGranny)
   {
      pGreatG->addRight(pHead);
   }
   else if(pGreatG->pLeft == pGranny)
   {
      pGreatG->addLeft(pHead);
   }
}

/*************************************************
 *************************************************
 *************************************************
 ****************** ITERATOR *********************
 *************************************************
 *************************************************
 *************************************************/

/**************************************************
 * BST ITERATOR :: INCREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator ++ ()
{
   if (pNode == nullptr)
   {
      return *this;
   }
   


   if (pNode->pRight != nullptr)
   {
      pNode = pNode -> pRight;
      while (pNode->pLeft)
      {
         pNode = pNode->pLeft;
      }
      return *this;
   }


   assert(pNode->pRight == nullptr);
   const BNode* pSave = pNode;
   

   pNode = pNode->pParent;

   if (pNode == nullptr)
   {
      return *this;
   }


   if (pSave == pNode->pLeft)
   {
      return *this;
   }
   
   while (pNode != nullptr && pSave == pNode->pRight)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }


   return *this;
}

/**************************************************
 * BST ITERATOR :: DECREMENT PREFIX
 * advance by one
 *************************************************/
template <typename T>
typename BST <T> :: iterator & BST <T> :: iterator :: operator -- ()
{
   if (pNode == nullptr)
   {
      return *this;
   }



   if (pNode->pLeft != nullptr)
   {
      pNode = pNode->pLeft;

      while (pNode->pRight)
      {
         pNode = pNode->pRight;
      }
      return *this;
   }


   assert(pNode->pLeft == nullptr);
   const BNode* pSave = pNode;


   pNode = pNode->pParent;

   if (pNode == nullptr)
   {
      return *this;
   }


   if (pSave == pNode->pRight)
   {
      return *this;
   }

   while (pNode != nullptr && pSave == pNode->pLeft)
   {
      pSave = pNode;
      pNode = pNode->pParent;
   }


   return *this;

}


} // namespace custom




