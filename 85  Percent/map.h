/***********************************************************************
 * Header:
 *    MAP
 * Summary:
 *    Our custom implementation of a std::map 
 *        ____     _______        __
 *      .' __ '.  |  _____|   _  / /
 *      | (__) |  | |____    (_)/ /
 *      .`____'.  '_.____''.   / / _
 *     | (____) | | \____) |  / / (_)
 *     `.______.'  \______.' /_/
 *
 *    This will contain the class definition of:
 *        map                 : A class that represents a map
 * Author
 *    <your name here>
 ************************************************************************/

#pragma once

#include "pair.h"     // for pair
#include "bst.h"      // no nested class necessary for this assignment

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

class TestMap; // forward declaration for unit tests

namespace custom
{

/*****************************************************************
 * MAP
 * Create a Map, similar to a Binary Search Tree
 *****************************************************************/
class map
{
   friend class ::TestMap; // give unit tests access to the privates
   friend void swap(map& lhs, map& rhs);
public:
   using Pairs = custom::pair<int, int>;
   
   // 
   // Construct
   //
   map()                 
   {
   }
   map(const map &  rhs) 
   { 
   }
   map(map && rhs) 
   { 
   }
   template <class Iterator>
   map(Iterator first, Iterator last)
   {
   }
   map(const std::initializer_list <Pairs>& il)
   {
   }
   ~map()                                           
   {                          
   }

   //
   // Assign
   //
   map & operator = (const map & rhs)
   {
      return *this;
   }
   map & operator = (map && rhs)
   {
      return *this;
   }
   map & operator = (const std::initializer_list <Pairs> & il)
   {
      return *this;
   }
     
   // 
   // Access
   //
   int & operator [] (const int & k);
   int & at (const int& k);
   bool find(const int & k);
   
   //
   // Insert
   //
   bool insert(Pairs && rhs);
   bool insert(const Pairs & rhs);
   
   template <class Iterator>
   void insert(Iterator first, Iterator last)
   {
   }
   void insert(const std::initializer_list <Pairs>& il)
   {
   }
   
   //
   // Remove
   //
   void clear() noexcept 
   { 
   }
   size_t erase(const int & k)
   {
      return 99;
   }

   // 
   // Status
   //
   bool empty() const noexcept 
   { 
      return true;
   }
   size_t size() const noexcept 
   { 
      return 88; 
   }

   
private:
   
   BST<Pairs> bst;
};

/*****************************************************
 * MAP :: SUBSCRIPT
 * Retrieve an element from the map
 ****************************************************/
int & map :: operator [] (const int & key)
{
   return *(new int(99));
}


/*****************************************************
 * MAP :: AT
 * Retrieve an element from the map
 ****************************************************/
int & map::at(const int & key)
{
   return *(new int(99));
}

/*****************************************************
 * SWAP
 * Swap two maps
 ****************************************************/
void swap(map & lhs, map & rhs)
{
}


/***********************************************
 * MAP : FIND
 * Returns an iterator to the searched for element.
 ***********************************************/
bool map::find(const int & k)
{
   return true;
}


/***********************************************
 * MAP : INSERT
 * Inserts an element into the map.
 ***********************************************/
bool map::insert(Pairs && rhs)
{
   return true;
}

bool map::insert(const Pairs & rhs)
{
   return true;
}


}; //  namespace custom

