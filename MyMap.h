// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#include <string>
#include "provided.h"

#include "support.h"

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap(){
        root=nullptr;
        num=0;
    }
    
    ~MyMap(){
        clear();
    }
    
    void clear(){
        if(root==nullptr)
            return;
        node* t=root;   // save ptr to root
        root=t->l;  // go to left
        clear();    // clear left subtree
        root=t->r;  // go to right
        clear();    // clear right subtree
        delete t;   // delete root
    }
    
    int size() const{
        return num;
    }
    
    void associate(const KeyType& key, const ValueType& value){
        if(root==nullptr){
            root=new node(key,value);
            num++;
            return;
        }
        node* n=root;
        
        while(n!=nullptr){
            if(key==n->key){
                n->val=value;   // if find value, set the value to parameter
                return;
            }
            else if(key > n->key){  // going right
                if(n->r==nullptr){  // if no node, then add to the right
                    n->r=new node(key,value);
                    num++;
                    return;
                }
                n=n->r; // go to right
            }
            else{   // going left
                if(n->l==nullptr){  // if no node, then add to left
                    n->l=new node(key,value);
                    num++;
                    return;
                }
                n=n->l; // go to left
            }
        }
    }

	  // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const{
        node* n=root;
        while(n!=nullptr){
            if(key==n->key) // found value, return pointer
                return &(n->val);
            if(key>n->key)  // going right
                n=n->r;
            else
                n=n->l; // going left
        }
        return nullptr;
    }

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
    class node{
    public:
        KeyType key;
        ValueType val;
        node* l;
        node* r;
        node(const KeyType& keyx, const ValueType& valuex){
            key=keyx;
            val=valuex;
            l=r=nullptr;
        }
    };
    node* root;
    int num;
};
