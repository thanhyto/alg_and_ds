#include <cassert>
#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include "json.hpp"

#include "AVL.h"


using namespace std; 
using namespace nlohmann;

using std::shared_ptr;
using std::make_shared;
using std::weak_ptr;


AVLNode::AVLNode(int key) :
	key_(key),
	parent_(std::weak_ptr<AVLNode>()),
	left_(nullptr),
	right_(nullptr) {}

AVLNode::AVLNode(int key, std::weak_ptr<AVLNode> parent) :
	key_(key),
	parent_(parent),
	left_(nullptr),
	right_(nullptr) {}

bool AVLNode::IsLeaf() const {
	return left_ == nullptr && right_ == nullptr;
}

bool AVLNode::HasLeftChild() const {
	return left_ != nullptr;
}

bool AVLNode::HasRightChild() const {
	return right_ != nullptr;
}



AVL::AVL() : root_(nullptr), size_(0) {}

void AVL::Insert(int key) {
	if (root_ == nullptr) {
		root_ = std::make_shared<AVLNode>(key);
		size_++;
		return;
	}
	std::shared_ptr<AVLNode> currentNode = root_, lastNode = nullptr;
	while (currentNode != nullptr) {
		lastNode = currentNode;
		currentNode = (key < currentNode->key_) ?
			currentNode->left_ : currentNode->right_;
	}
	if (key < lastNode->key_) {
		lastNode->left_ = std::make_shared<AVLNode>(key, lastNode);
	} else {
		lastNode->right_ = std::make_shared<AVLNode>(key, lastNode);
	}
	size_++;
}

int AVL::Height(shared_ptr<AVLNode> t)
{
	if (t)
		return t->height_;
	else
		return -1; 
}

int AVLNode::max(int leftH, int rightH)
{
	return (leftH > rightH) ? leftH: rightH;
}


size_t AVL::size() const {
	return size_;
}

bool AVL::empty() const {
	return size_ == 0;
}


std::string AVL::JSON() const {
	nlohmann::json result;
	std::queue< std::shared_ptr<AVLNode> > nodes;
	if (root_ != nullptr) {
		result["root"] = root_->key_;
		nodes.push(root_);
		while (!nodes.empty()) {
			auto v = nodes.front();
			nodes.pop();
			std::string key = std::to_string(v->key_);
			result[key]["balance factor"] = v->bf_;
			result[key]["height"] = v->height_;
			if (v->left_ != nullptr) {
				result[key]["left"] = v->left_->key_;
				nodes.push(v->left_);
			}
			if (v->right_ != nullptr) {
				result[key]["right"] = v->right_->key_;
				nodes.push(v->right_);
			}
			if (v->parent_.lock() != nullptr) {
				result[key]["parent"] = v->parent_.lock()->key_;
			} else {
				result[key]["root"] = true;
			}
		}
	}
	if (root_) 
	{
		result["height"] = root_->height_; 
		result["size"] = size_;
	}

	else // tree empty 
	{
		result["height"] = -1; 
		result["size"] = 0;
	}

	return result.dump(2) + "\n";
}




void AVL::recursiveInsert(int key)
{
	if (root_ == nullptr)
    {
      root_ = std::make_shared<AVLNode>(key);
      root_->height_ = 0;
      root_->bf_ = 0;
      size_++;
    }
    else
      Insert(key, root_, nullptr); 
}

void AVL::Insert(int key, shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent)
{
    if (node == nullptr)
    {

       if (key < parent->key_)
       {
    	  parent->left_ = std::make_shared<AVLNode>(key, parent); 
       }
       else
    	{
          parent->right_ = std::make_shared<AVLNode>(key, parent);
        }
     
        size_++;
    	return; 
    }
    else if (key < node->key_)//Insert on Left side 
    {
    	Insert(key, node->left_, node);
    	Balance(key, node, parent); 
    }
    else if (key > node->key_)// Insert on right side
    {	
    	Insert(key, node->right_, node);
    	Balance(key, node, parent);    	
    }
    else
    {
    	std::cerr << "AVLNode::Duplicate Error: duplicate node\n";
		exit(EXIT_FAILURE);
    }

    node->height_ = 1 + max(Height(node->left_), Height(node->right_)); 
    node->bf_ = Height(node->right_) - Height(node->left_);
    
}


void AVL::Balance(int key, shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent)
{
	
	if (Height(node->right_) - Height(node->left_) == -2) //LL LR case
    {
		if (node->left_ && key < node->left_->key_)	//LL
		{
			node = RotateRight(node->left_, node);
			if (parent == nullptr)//its the root 
				root_ = node;
			else
			{
				node->parent_ = parent;
        		if (parent->HasRightChild() && parent->right_ == node->right_)  
        		  parent->right_ = node;
        		else
        		  parent->left_ = node;
			}
		}
		else	//LR
		{	
			if (node->left_->right_)
			{
			    assert(node->left_ != nullptr);	
				node->left_ = RotateLeft(node->left_->right_, node->left_);  
				node = RotateRight(node->left_, node); 
				if (parent == nullptr)//root has no parent
		    	    root_ = node;
			    else
			    {
			    	node->parent_ = parent;
	        		if (parent->HasRightChild() && parent->right_ == node->right_)  
	        		  parent->right_ = node;
	        		else
	        		  parent->left_ = node;
			    }
			}
		}
    }
	if (Height(node->right_) - Height(node->left_) == 2) //RR RL case
    {
		if (node->right_ && key > node->right_->key_) //RR case 
		{
			node = RotateLeft(node->right_,node); 
			if (parent == nullptr)// root has no parent
				root_ = node; 
			else
			{
				node->parent_ = parent;
        		if (parent->HasRightChild() && parent->right_ == node->left_)  
        		  parent->right_ = node;
        		else
        		  parent->left_ = node;
			}
		}
		else //RL
		{
			if (node->right_->left_)
			{
				assert(node->right_ != nullptr);
				node->right_ = RotateRight(node->right_->left_, node->right_);
				node = RotateLeft(node->right_, node);
				if (parent == nullptr)//root has no parent 
	            	root_ = node;
	            else
	            {
	            	node->parent_ = parent;
	               
	        		if (parent->HasRightChild() && parent->right_ == node->left_)  
	        		  parent->right_ = node;
	        		else
	        		  parent->left_ = node;
	            }
	        }
		}
    }
}
//Right Rotation
shared_ptr<AVLNode> AVL::RotateRight(shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent)
{ 
    parent->left_ = node->right_;

    if (node->right_)
    	node->right_->parent_ = parent;

    parent->parent_ = node;
    node->right_ = parent;

    node->parent_.reset();

    parent->height_ = max(Height(parent->right_), Height(parent->left_) ) + 1;
    node->height_ = max(Height(node->left_), parent->height_) + 1;

    node->bf_ = Height(node->right_) - Height(node->left_);
    parent->bf_ = Height(parent->right_) - Height(parent->left_);

    return node;    
}
//Left Rotation
shared_ptr<AVLNode> AVL::RotateLeft(shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent)
{ 

	parent->right_ = node->left_;	
	if (node->left_)
	{
		node->left_->parent_ = parent; 
	}

    parent->parent_ = node;				
    node->left_ = parent;				

    node->parent_.reset();	 

    parent->height_ = max(Height(parent->right_), Height(parent->left_) ) + 1;
    node->height_ = max(Height(node->right_), Height(parent->left_)) + 1;

    node->bf_ = Height(node->right_) - Height(node->left_);
    parent->bf_ = Height(parent->right_) - Height(parent->left_);

    return node;    
}



