#include<unordered_map>


struct Node{
    int key, value;
    Node* prev;
    Node* next;
};

class NodeList{
public:
    NodeList(){
        size_=0;
        head_=new Node();
        tail_=head_;
    }

    void addToHead(int k, int val){
        head_->key=k;
        head_->value=val;
        map_[k]=head_;

        Node* newHead=new Node();
        newHead->next=head_;
        head_->prev=newHead;
        head_=newHead;

        size_++;
        
    }

    Node* get(int k){
        Node* node=map_[k];
        if(node) return node;
        return nullptr;
    }

    void moveToHead(int k, int val){
        Node* moveNode=get(k);
        if(moveNode){
            if(tail_==moveNode) pop();
            else removeNode(moveNode);
            addToHead(k, val);
        }
    }

    void removeNode(Node* node){
        node->next->prev=node->prev;
        node->prev->next=node->next;
        node->next=nullptr;
        node->prev=nullptr;
  
        map_.erase(node->key);
        size_--;
        
        if(node) delete node;
    }

    void pop(){
        Node* newTail=tail_->prev;

        tail_->prev->next=tail_->next;
        tail_->prev=nullptr;

        map_.erase(tail_->key);
        size_--;

        if(tail_) delete tail_;
        tail_=newTail;
    }

    int size(){ return size_; }
    Node* end(){ return tail_; }

private:
    Node* head_;
    Node* tail_;
    int size_;

    std::unordered_map<int, Node*> map_;
};

class LRUCache {
public:
    LRUCache(int capacity) {
        capacity_=capacity;
    }
    
    int get(int key) {
        Node* node=list_.get(key);
        if(node){
            int value=node->value;
            list_.moveToHead(key, value);
            return value;
        }
        return -1;
    }
    
    void put(int key, int value) {
        Node* node=list_.get(key);
        if(!node){
            if(list_.size()==capacity_){
                list_.pop();
            }
            list_.addToHead(key, value);
        }else list_.moveToHead(key, value);
    }

private:
    NodeList list_;
    int capacity_;
};