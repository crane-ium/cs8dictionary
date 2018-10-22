#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
#include <cassert>
#include <math.h>

using namespace std;

template <class T>
struct tree_node{
    T _item;
    tree_node<T>* _left;
    tree_node<T>* _right;
    unsigned int _height;
    //CTOR
    tree_node(T item=T(), tree_node* left=NULL, tree_node* right=NULL):
        _item(item), _left(left), _right(right), _height(0){}
    //MOD MEMBER FUNCTIONS
    void tree_insert(tree_node<T>* &root, const T &item); //inserts by comparing the node's items
    int height(); //gets height of the current node by propogating thru the children also
    void update_height(tree_node<T> *&node); //updates the height of the chosen node
    bool delete_node(tree_node<T> *&root, const T& item); //deletes a specific node based on item
    tree_node<T>* get_largest_node(tree_node<T>* root, tree_node<T> *&parent);
    tree_node<T>* get_smallest_node(tree_node<T>* root, tree_node<T> *&parent);
    bool find_node(const T& item, tree_node<T>* &root);
    bool find_node(const T& item, tree_node<T> *&root, tree_node<T> **&node);
    void clear(tree_node<T>* &root); //deletes the root and all children
    //Rotation Functions:
    tree_node<T>* rotate_right(tree_node<T>* &root);
    tree_node<T>* rotate_left(tree_node<T>* &root);
    tree_node<T>* rotate(tree_node<T>* &root);
    //CONST MEMBER FUNCS
    void tree_print_inorder(tree_node<T>* root, int level, ostream& outs=cout) const;
    void tree_print(tree_node<T>* root, int level, ostream& outs=cout,  const bool &show_h=false) const;
    void tree_print_list(tree_node<T>* root, int level, ostream& outs=cout) const;
    int balance_factor() const;
    //FRIEND FUNCS
    template<class U>
    friend void tree_copy(tree_node<U>* copy_root, tree_node<U> *&main_root);
    friend ostream& operator <<(ostream& outs, tree_node<T>*& node){
        node->tree_print(node, 0, outs);
        return outs;
    }
};

template<class T>
void tree_node<T>::tree_insert(tree_node<T>* &root, const T& item){
    //just starting with a basic in-order cout
    if(!root){
        root = new tree_node(item);
    }else if(item < root->_item){
        tree_insert(root->_left, item);
    }else if(item > root->_item){
        tree_insert(root->_right, item);
    }
    if(item == root->_item){
        root->_item = item;
        return;
    }
    assert(item != root->_item); //quick check
    root->update_height(root);
}

template<class T>
void tree_node<T>::tree_print_inorder(tree_node<T> *root, int level, ostream &outs) const{
    if(!root)
        return;
    tree_print_inorder(root->_left, level+1, outs);
    outs << setw(level) << "";
    outs << root->_item << ": " << root->_height << endl;
    tree_print_inorder(root->_right, level+1, outs);
}

template<class T>
void tree_node<T>::tree_print_list(tree_node<T>* root, int level, ostream& outs) const{
    if(!root)
        return;
    tree_print_list(root->_left, level+1, outs);
    outs << "[" << root->_item << "]  ";
    tree_print_list(root->_right, level+1, outs);
}

template<class T>
void tree_node<T>::tree_print(tree_node<T> *root, int level, ostream &outs, const bool &show_h) const{
    if(!root)
        return;
    tree_print(root->_right, level+1, outs, show_h);
    for(int i = 0; i < level; i++)
        outs << "---- ";
    outs << "["<< root->_item << "] ";
    if(show_h)
        outs << "H: " << root->_height;
    outs << endl;
    tree_print(root->_left, level+1, outs, show_h);
}

template<class T>
int tree_node<T>::height(){
    //set the height of the node
    if(_right && _left)
        this->_height = (_right->height() > _left->height())?
                (_right->height() + 1):(_left->height() + 1);
    else if(_right)
        _height = _right->height() + 1;
    else if(_left)
        _height = _left->height() + 1;
    else
        _height = 0;
    return _height;
}

template<class T>
void tree_node<T>::update_height(tree_node<T>* &node){
    this->_height = height();
    if(balance_factor() == 2){
        //right heavy
        if(abs(_right->balance_factor()) != 2){
            this->rotate_left(node);
            node->update_height(node); //This should not make an infinite loop.
        }
    }else if(balance_factor() == -2){
        //left heavy
        if(abs(_left->balance_factor()) != 2){
//            cout << "Node " << this->_item << " is the base\n";
            this->rotate_right(node);
            node->update_height(node); //This should not make an infinite loop.
        }
    }else
        return;
}

template <class T> //uhh i think i had a brainfart when i made this. It still works well tho
bool tree_node<T>::delete_node(tree_node<T>* &root, const T& item){
    tree_node<T>** node = NULL;
    if(!find_node(item, root, node))
        return false;
    else{
        tree_node<T>* largest = NULL;
        tree_node<T>* smallest = NULL;
        tree_node<T>* parent = NULL;
        tree_node<T>* small_parent = NULL;
        largest = get_largest_node((*node)->_left, parent);
        smallest = get_smallest_node((*node)->_right, small_parent);
        if(largest){
            //Then there is a node to the left
            if(parent){
                //Then there was a parent of the left node that is not this
                parent->_right = NULL;
            }else
                (*node)->_left = NULL; //else the largest node was the left child
            (*node)->_item = largest->_item;
            delete largest;
        }else if(smallest){
            if(small_parent){
                //Then there was a parent of the left node that is not this
                small_parent->_right = NULL;
            }else
                (*node)->_right = NULL; //else the largest node was the left child
            (*node)->_item = smallest->_item;
            delete smallest;
        }else{
            //There's no node to the right so just set node to null and delete
            tree_node<T>* temp = (*node);
            (*node) = NULL;
            delete temp;
            return true;
        }
        root->update_height(root);
    }
    return true;
}
template<class T>
bool tree_node<T>::find_node(const T &item, tree_node<T> *&root){
    if(this->_item == item){
        root = this;
        return true;
    }else if(this->_left && item <this->_item){
        this->_left->find_node(item, root);
    }else if(this->_right && item > this->_item){
        this->_right->find_node(item,root);
    }
    return false;
}
template<class T>
bool tree_node<T>::find_node(const T& item, tree_node<T>* &root, tree_node<T>** &node){
    if(!root)
        return false;
    if(root->_item == item){
        node = &root; //returns the node back to the base call
        return true;
    }
    if(root->_item < item){
        return find_node(item, root->_right, node);
    }else{
        return find_node(item, root->_left, node);
    }
}

template<class T>
tree_node<T>* tree_node<T>::get_largest_node(tree_node<T> *root, tree_node<T>* &parent){
    //returns pointer to node with the largest node in that tree.
    if(root == NULL)
        return NULL; //this would be bad, btw
    if(root->_right != NULL){
        parent = root;
        return get_largest_node(root->_right, parent);
    }else{
        return root;
    }
}

template<class T>
tree_node<T>* tree_node<T>::get_smallest_node(tree_node<T> *root, tree_node<T>* &parent){
    //returns pointer to node with the largest node in that tree.
    if(root == NULL)
        return NULL; //this would be bad, btw
    if(root->_left != NULL){
        parent = root;
        return get_largest_node(root->_left, parent);
    }else{
        return root;
    }
}

template <class T>
void tree_node<T>::clear(tree_node<T> *&root){
    //deletes in post-order
    if(!root)
        return;
    clear(root->_left);
    clear(root->_right);
    delete root;
//    cout << "Deleted " << root->_item << endl;
    root = NULL;

}

template<class T>
int tree_node<T>::balance_factor() const{
    //Checks whether to use the _height or -1 if _left or _right DNE
    return ((_right)?(_right->_height):(-1)) - ((_left)?(_left->_height):(-1));
}

template<class T>
void tree_copy(tree_node<T>* copy_root, tree_node<T> *& main_root){
    if(!copy_root)
        return;
    //Preorder traversal while inserting into main_root
    tree_node<T>* temp = new tree_node<T>(copy_root->_item);
    main_root = temp;
    main_root->_height = copy_root->_height;
    tree_copy(copy_root->_left, main_root->_left);
    tree_copy(copy_root->_right, main_root->_right);
}

//-----ROTATION FUNCS----
template<class T>
tree_node<T>* tree_node<T>::rotate_right(tree_node<T>* &root){
    //We need to check the two possible rotations:
    //Left-Right rotation
    if(root->_left->balance_factor() > 0){
//        cout << "Rotate right: left-right\n";
        root = root->_left->_right;
        if(root->_left)
            this->_left->_right = root->_left;
        else
            this->_left->_right = NULL;
        root->_left = this->_left;
        this->_left = NULL;
        if(root->_right){
            this->_left = root->_right;
        }
        root->_right = this;
    }else if(root->_left->balance_factor() < 0){
        //Left-Left rotation
//        cout << "Rotate right: left-left\n";
        root = root->_left;
        this->_left = root->_right;
        root->_right = this;
    }
    return NULL;
}

template<class T>
tree_node<T>* tree_node<T>::rotate_left(tree_node<T>* &root){
    //Right-Right rotation
    if(root->_right->balance_factor() > 0){
//        cout << "Rotate left: right-right\n";
        root = root->_right;
        this->_right = root->_left;
        root->_left = this;
    }else if(root->_right->balance_factor() < 0){
        //Right-Left rotation
//        cout << "Rotate left: right-left\n";
        root = root->_right->_left;
        if(root->_right)
            this->_right->_left = root->_right;
        else
            this->_right->_left = NULL;
        root->_right = this->_right;
        this->_right = NULL;
        if(root->_left){
            this->_right = root->_left;
        }
        root->_left = this;
    }
    return NULL;
}

template<class T>
tree_node<T>* tree_node<T>::rotate(tree_node<T>* &root){
    //I DON'T SEE THIS AS NECESSARY WITH MY SETUP
}

#endif // TREENODE_H
