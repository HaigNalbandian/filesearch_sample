#ifndef smartset_h_
#define smartset_h_

#include <vector>
#include <iostream>
#include <string>
#include <cstddef>
#include <set>

using namespace std;

template <class T>
class SmartSet : public set<T> {
public:
    SmartSet ();
    ~SmartSet ();
    SmartSet<T> setIntersection (const SmartSet<T> & other) const;
    SmartSet<T> setIntersection (const set<T> & other) const;
    SmartSet<T> setUnion (const SmartSet<T> & other) const;
    SmartSet<T> setUnion (const set<T> & other) const;
};

template <class T>
SmartSet<T>::SmartSet (){}

template <class T>
SmartSet<T>::~SmartSet (){}

template <class T>
SmartSet<T> SmartSet<T>::setIntersection (const SmartSet<T> & other) const
{
    SmartSet<T> intersection;
    for(typename std::set<T>::iterator it = this->begin(); it != this->end(); ++it)
    {
        if(other.find(*it) != other.end())
        {
            intersection.insert(*it);
        }
    }
    return intersection;
}

template <class T>
SmartSet<T> SmartSet<T>::setUnion (const SmartSet<T> & other) const   
{
    SmartSet<T> unionSet;
    for(typename std::set<T>::iterator it = this->begin(); it != this->end(); ++it)
    {
        unionSet.insert(*it);
    }

    for(typename SmartSet<T>::iterator it = other.begin(); it != other.end(); ++it) //add only the elements of set2 that set1 does not have
    {
        if(this->find(*it) == this->end())
        {
            unionSet.insert(*it);
        }
    }
    return unionSet;
}


//WHAT IF I GET A SET INSTEAD?
template <class T>
SmartSet<T> SmartSet<T>::setIntersection (const set<T> & other) const
{
    SmartSet<T> intersection;
    for(typename std::set<T>::iterator it = this->begin(); it != this->end(); ++it)
    {
        if(other.find(*it) != other.end())
        {
            intersection.insert(*it);
        }
    }
    return intersection;
}

template <class T>
SmartSet<T> SmartSet<T>::setUnion (const set<T> & other) const   
{
    SmartSet<T> unionSet;
    for(typename std::set<T>::iterator it = this->begin(); it != this->end(); ++it)
    {
        unionSet.insert(*it);
    }

    for(typename set<T>::iterator it = other.begin(); it != other.end(); ++it) //add only the elements of set2 that set1 does not have
    {
        if(this->find(*it) == this->end())
        {
            unionSet.insert(*it);
        }
    }
    return unionSet;
}
#endif
