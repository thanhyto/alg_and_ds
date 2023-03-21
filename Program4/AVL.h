#include <memory>
#include <string>
#include <vector>
#include <utility>
#include "json.hpp"

using namespace std; 

class AVL;

class AVLNode {
 public:
 	AVLNode(int key);
 	AVLNode(int key, std::weak_ptr<AVLNode> parent);
 	bool IsLeaf() const;
 	bool HasLeftChild() const;
 	bool HasRightChild() const;

 	int max(int leftH, int rightH); 


 protected:
	 int bf_;
	 int key_;
	 int height_; 
	 std::weak_ptr<AVLNode> parent_;
	 std::shared_ptr<AVLNode> left_;
	 std::shared_ptr<AVLNode> right_;
 
 friend class AVL; 

}; // class AVLNode

class AVL {
 public:
 	AVL();

 	void Insert(int key);
 	bool Find(int key) const;
 	std::string JSON() const;
 	size_t size() const;
 	bool empty() const;

    void recursiveInsert(int key);
 protected:
    void Insert(int key, shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent); 

 	int Height(shared_ptr<AVLNode> t);


	// Balance Functions
	shared_ptr<AVLNode> RotateRight(shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent);
	shared_ptr<AVLNode> RotateLeft(shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent); 
	void Balance(int key, shared_ptr<AVLNode> node, shared_ptr<AVLNode> parent); 

	//Members
 	std::shared_ptr<AVLNode> root_;
 	size_t size_;
}; // class AVL