#include <iostream>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
	nodes_(max_size + 1, KeyValuePair()),
	max_size_(max_size),
	size_(0) {
}
//Check for parent
bool hasParent(size_t i){
    return i>1;
}
size_t Parent(size_t i){
    return i/2;
}
//Compare children and parent
bool PriorityQueue::LessThan(size_t child, size_t parent){
    return nodes_[child].first < nodes_[parent].first;
}
//Left and right children indicies
size_t leftChild(size_t i){
    return 2*i;
}
size_t rightChild(size_t i){
    return 2*i+1;
}
bool PriorityQueue::InBounds(size_t i){
    return i < size_ + 1;
}

void PriorityQueue::insert(Key k) {
	insert(std::make_pair(k, std::make_pair(0, 0)));
}

void PriorityQueue::insert(KeyValuePair kv) {
	// TODO: complete this function
    //Push item into vector
    nodes_[++size_] = kv;
    //make the vector into heap
    heapifyUp(size_);
}

KeyValuePair PriorityQueue::min() {
	// TODO: complete this function
    return nodes_[1];
}

KeyValuePair PriorityQueue::removeMin() {
	// TODO: complete this function
    //Check if queue empty
    if(size_ == 0){
        throw std::underflow_error("Empty queue!");
    }
    KeyValuePair root = nodes_[1];
    nodes_[1] = nodes_[size_--];
    nodes_.erase(nodes_.end());
    heapifyDown(1);
    return root;
}

bool PriorityQueue::isEmpty() const {
	// TODO: complete this function
    if(size_ == 0){
        return true;
    }
    return false;
}

// size_t PriorityQueue::size() const {
// 	// TODO: complete this function
// }

nlohmann::json PriorityQueue::JSON() const {
	nlohmann::json result;
  for (size_t i = 1; i <= size_; i++) {
      nlohmann::json node;
      KeyValuePair kv = nodes_[i];
      node["key"] = kv.first;
      node["value"] = kv.second;
      if (i != ROOT) {
          node["parent"] = std::to_string(i / 2);
      }
      if (2 * i <= size_) {
          node["leftChild"] = std::to_string(2 * i);
      }
      if (2 * i + 1 <= size_) {
          node["rightChild"] = std::to_string(2 * i + 1);
      }
      result[std::to_string(i)] = node;
  }
  result["metadata"]["max_size"] = max_size_;
  result["metadata"]["maxHeapSize"] = max_size_;
  result["metadata"]["size"] = size_;
	return result;
}

void PriorityQueue::heapifyUp(size_t i) {
	// TODO: complete this function
    //Check for smaller parent
    while(hasParent(i) && LessThan(i, Parent(i))){
        std::swap(nodes_[Parent(i)], nodes_[i]);
        i = Parent(i);
    }
}

void PriorityQueue::heapifyDown(size_t i) {
	// TODO: complete this function
    while(InBounds(leftChild(i))){
        size_t child = leftChild(i);
        if(InBounds((rightChild(i))) && LessThan(rightChild(i), leftChild(i))){
            child=rightChild(i);
        }
        if(LessThan(child, i)){
            std::swap(nodes_[child], nodes_[i]);
        }
        else{
            break;
        }
        i=child;
    }
}

void PriorityQueue::removeNode(size_t i) {
	// TODO: complete this function
    nodes_[i] = nodes_[size_--];
    nodes_.erase(nodes_.end());
    if(nodes_[i].first < nodes_[Parent(i)].first){
        heapifyUp(i);
    }
    else{
        heapifyDown(i);
    }
}

Key PriorityQueue::getKey(size_t i) {
	// TODO: complete this function
    return nodes_[i].first;
}

