#ifndef UTILS_SRC_RBT_H
#define UTILS_SRC_RBT_H

#include <iostream>
#include <iomanip>

enum RBTColor {
    RED, BLACK
};

template<class T>
class RBTNode {
public:
    RBTNode(T value, enum RBTColor c, RBTNode *p, RBTNode *l, RBTNode *r) :
            key(value), color(c), parent(p), left(l), right(r) {}

    RBTNode *&GetParent() { return this->parent; }

    RBTNode *&GetLeft() { return this->left; }

    RBTNode *&GetRight() { return this->right; }

    T &GetKey() { return this->key; }

    enum RBTColor &GetColor() { return this->color; }

    bool IsRed() { return this->color == RED; }

    bool IsBlack() { return this->color == BLACK; }

    void SetBlack() { this->color = BLACK; }

    void SetRed() { this->color = RED; }

    void SetParent(RBTNode *p) { this->GetParent() = p; }

    void SetColor(enum RBTColor color) { this->color = color; }

private:
    T key;
    enum RBTColor color;
    RBTNode *parent;
    RBTNode *left;
    RBTNode *right;
};

template<class T>
class RBTree {
public:
    RBTree() : root(nullptr) {}

    ~RBTree() {
        Destory();
    }

    // 前序遍历"红黑树"
    void PreOrder() {
        if (root) {
            PreOrder(root);
        }
    }

    // 中序遍历"红黑树"
    void InOrder() {
        if (root) {
            if (root) {
                InOrder(root);
            }
        }
    }

    // 后序遍历"红黑树"
    void PostOrder() {
        if (root) {
            PostOrder(root);
        }
    }

    // (递归实现)查找"红黑树"中键值为key的节点
    RBTNode<T> *Search(T key) {
        return Search(root, key);
    }

    // (非递归实现)查找"红黑树"中键值为key的节点
    RBTNode<T> *IterativeSearch(T key) {
        return IterativeSearch(root, key);
    }

    // 找结点(x)的后继结点。即，查找"红黑树中数据值大于该结点"的"最小结点"。
    RBTNode<T> *Successor(RBTNode<T> *x) {
        if (x->GetRight() != nullptr) {
            return MinNode(x->GetRight());
        }

        /**
         * x没右右孩子
         * 1. 当前节点是个左孩子， 其后继节点是其父节点
         * 2. 当前节点是个右孩子， 则寻找其父节点其具有左孩子的
         *
         *        7
         *       /
         *     5
         *       \
         *        6
         */

        RBTNode<T> *y = x->GetParent();
        while ((y != nullptr) && (x == y->GetRight())) {
            x = y;
            y = y->GetParent();
        }
        return y;

    }

    // 找结点(x)的前驱结点。即，查找"红黑树中数据值小于该结点"的"最大结点"。
    RBTNode<T> *Predecessor(RBTNode<T> *x) {
        if (x->GetLeft() != nullptr) {
            return MaxNode(x->GetLeft());
        }
        /***
         * x 是右孩子: 其前驱是父节点
         * x 是左孩子： 其则寻找其父节点其具有右孩子的
         *
         *      5
         *       \
         *        7
         *     /
         *    6
         *
         */
        RBTNode<T> *y = x->GetParent();
        while (y != nullptr && x == y->GetLeft()) {
            x = y;
            y = y->GetParent();
        }
        return y;
    }


    void Insert(T key) {
        RBTNode<T> *z = new RBTNode<T>(key, BLACK, nullptr, nullptr, nullptr);
        if (nullptr == z) return;
        Insert(root, z);
    }

    void Remove(T key) {
        RBTNode<T> *node = Search(root, key);
        if (node) {
            Remove(root, node);
        }
    }

    void Destory() {
        Destory(root);
    }

    void Print() {
        if (root) {
            Print(root, root->GetKey(), 0);
        }
    }

private:

    void PreOrder(RBTNode<T> *tree);

    void InOrder(RBTNode<T> *tree);

    void PostOrder(RBTNode<T> *tree);


    RBTNode<T> *Search(RBTNode<T> *x, T key);

    // 递归从树x搜索键值为key的节点
    RBTNode<T> *IterativeSearch(RBTNode<T> *x, T key);

    RBTNode<T> *MinNode(RBTNode<T> *tree);

    RBTNode<T> *MaxNode(RBTNode<T> *tree);

    // root指针可能会被改变
    void LeftRotate(RBTNode<T> *&root, RBTNode<T> *x);

    void RightRotate(RBTNode<T> *&root, RBTNode<T> *y);

    void Insert(RBTNode<T> *&root, RBTNode<T> *node);

    void InsertFixup(RBTNode<T> *&root, RBTNode<T> *node);

    void Remove(RBTNode<T> *&root, RBTNode<T> *node);

    void RemoveFixup(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent);

    void Destory(RBTNode<T> *&tree);

    void Print(RBTNode<T> *&tree, T key, int direction);

    RBTNode<T> *root;
};

template<class T>
void RBTree<T>::PreOrder(RBTNode<T> *tree) {
    if (tree) {
        std::cout << tree->GetKey() << " ";
        PreOrder(tree->GetLeft());
        PreOrder(tree->GetRight());
    }
}

template<class T>
void RBTree<T>::InOrder(RBTNode<T> *tree) {
    if (tree) {
        InOrder(tree->GetLeft());
        std::cout << tree->GetKey() << " ";
        InOrder(tree->GetRight());
    }
}

template<class T>
void RBTree<T>::PostOrder(RBTNode<T> *tree) {
    if (tree) {
        PostOrder(tree->GetLeft());
        PostOrder(tree->GetRight());
        std::cout << tree->GetKey() << " ";
    }
}

template<class T>
RBTNode<T> *RBTree<T>::Search(RBTNode<T> *x, T key) {
    if (x == nullptr || x->GetKey() == key) return x;
    if (key < x->GetKey()) {
        return Search(x->GetLeft(), key);
    } else {
        return Search(x->GetRight(), key);
    }
}

// 递归从树x搜索键值为key的节点
template<class T>
RBTNode<T> *RBTree<T>::IterativeSearch(RBTNode<T> *x, T key) {
    while ((x != nullptr) && (x->GetKey() != key)) {
        if (key < x->GetKey()) {
            x = x->GetLeft();
        } else {
            x = x->GetRight();
        }
    }
    return x;
}

template<class T>
RBTNode<T> *RBTree<T>::MinNode(RBTNode<T> *tree) {
    if (tree == nullptr) return nullptr;

    while (tree->GetLeft()) {
        tree = tree->GetLeft();
    }
    return tree;
}

template<class T>
RBTNode<T> *RBTree<T>::MaxNode(RBTNode<T> *tree) {
    if (tree == nullptr) return nullptr;
    while (tree->GetRight()) {
        tree = tree->GetRight();
    }
    return tree;
}

// root指针可能会被改变
/*
 *         px                       px
 *       /      left               /
 *      x     ======>            y
 *    /  \                     /  \
 *  lx   y                   x    ry
 *      / \                 / \
 *     ly  ry              lx   ly
 * */
template<class T>
void RBTree<T>::LeftRotate(RBTNode<T> *&root, RBTNode<T> *x) {
    RBTNode<T> *y = x->GetRight();

    x->GetRight() = y->GetLeft();
    if (y->GetLeft() != nullptr) {
        y->GetLeft()->GetParent() = x;
    }
    y->GetParent() = x->GetParent();

    if (x->GetParent() == nullptr) {
        root = y;
    } else {
        if (x->GetParent()->GetLeft() == x) {
            x->GetParent()->GetLeft() = y;
        } else {
            x->GetParent()->GetRight() = y;
        }
    }
    y->GetLeft() = x;
    x->GetParent() = y;
}

/*
 *         px                         px
 *        /                          /
 *      y          right           x
 *     /  \      ========>        /  \
 *    x    ry                  lx    y
 *   / \                            / \
 * lx   rx                          rx  ry
 * */
template<class T>
void RBTree<T>::RightRotate(RBTNode<T> *&root, RBTNode<T> *y) {
    RBTNode<T> *x = y->GetLeft();

    y->GetLeft() = x->GetRight();
    if (x->GetRight() != nullptr) {
        x->GetRight()->GetParent() = y;
    }
    x->GetParent() = y->GetParent();

    if (y->GetParent() == nullptr) {
        root = x;
    } else {
        if (y == y->GetParent()->GetRight()) {
            y->GetParent()->GetRight() = x;
        } else {
            y->GetParent()->GetLeft() = x;
        }
    }
    x->GetRight() = y;
    y->GetParent() = x;
}


template<class T>
void RBTree<T>::Insert(RBTNode<T> *&root, RBTNode<T> *node) {
    RBTNode<T> *y = nullptr;
    RBTNode<T> *x = root;

    //二叉树插入
    while (x) {
        y = x;
        if (node->GetKey() < x->GetKey()) {
            x = x->GetLeft();
        } else {
            x = x->GetRight();
        }
    }

    //node作为y的子节点
    node->GetParent() = y;
    if (y) {
        if (node->GetKey() < y->GetKey()) {
            y->GetLeft() = node;
        } else {
            y->GetRight() = node;
        }
    } else {
        root = node;
    }

    //节点node设置为红色
    node->SetColor(RED);

    InsertFixup(root, node);
}

template<class T>
void RBTree<T>::InsertFixup(RBTNode<T> *&root, RBTNode<T> *node) {
    RBTNode<T> *parent, *gparent;

    // 如果父节点存在，且父节点是红色
    while ((parent = node->GetParent()) && parent->IsRed()) {
        gparent = parent->GetParent();
        if (parent == gparent->GetLeft()) {
            //uncle is red

            RBTNode<T> *uncle = gparent->GetRight();
            if (uncle && uncle->IsRed()) {
                uncle->SetBlack();
                parent->SetBlack();
                gparent->SetRed();
                node = gparent;
                continue;
            }

            //uncle is black
            //当前节点是父节点右孩子
            if (parent->GetRight() == node) {
                LeftRotate(root, parent);
                RBTNode<T> *tmp = parent;
                parent = node;
                node = tmp;

            } else {
                parent->SetBlack();
                gparent->SetRed();
                RightRotate(root, gparent);
            }
        } else {//父节点是祖父节点右孩子
            RBTNode<T> *uncle = gparent->GetLeft();
            if (uncle && uncle->IsRed()) {
                uncle->SetBlack();
                parent->SetBlack();
                gparent->SetRed();
                node = gparent;
                continue;
            }

            //uncle is black
            //当前节点是父节点左孩子
            if (parent->GetLeft() == node) {
                RightRotate(root, parent);
                RBTNode<T> *tmp = parent;
                parent = node;
                node = tmp;
            } else {
                parent->SetBlack();
                gparent->SetRed();
                LeftRotate(root, gparent);
            }
        }
    }
    //根节点设为黑色
    root->SetBlack();
}

template<class T>
void RBTree<T>::Remove(RBTNode<T> *&root, RBTNode<T> *node) {
    RBTNode<T> *child, *parent;
    RBTColor color;

    if ((node->GetLeft() && node->GetRight())) {
        //被删除的节点的后继节点取代被删除的节点
        RBTNode<T> *replace = node->GetRight();
        while (replace->GetLeft()) {
            replace = replace->GetLeft();
        }
        if (node->GetParent()) {
            if (node->GetParent()->GetLeft() == node) {
                node->GetParent()->GetLeft() = replace;
            } else {
                node->GetParent()->GetRight() = replace;
            }
        } else {
            //node is root node and update it
            root = replace;
        }

        //后继节点没有左孩子，所以不需要care左孩子
        child = replace->GetRight();
        parent = replace->GetParent();
        color = replace->GetColor();

        //被删除的节点是后继节点的父节点
        if (parent == node) {
            parent = replace;
        } else {
            if (child) {
                child->SetParent(replace);
            }
            parent->GetLeft() = child;
            node->GetRight()->SetParent(replace);
        }

        replace->GetParent() = node->GetParent();
        replace->GetColor() = node->GetColor();
        replace->GetLeft() = node->GetLeft();
        node->GetLeft()->GetParent() = replace;

        if (color == BLACK) {
            RemoveFixup(root, child, parent);
        }
        delete node;
        return;
    }
    if (node->GetLeft()) {
        child = node->GetLeft();
    } else {
        child = node->GetRight();
    }
    parent = node->GetParent();
    color = node->GetColor();
    if (child) {
        child->SetParent(parent);
    }
    if (parent) {
        if (parent->GetLeft() == node) {
            parent->GetLeft() = child;
        } else {
            parent->GetRight() = child;
        }
    } else {
        root = child;
    }
    if (color == BLACK) {
        RemoveFixup(root, child, parent);
    }
    delete node;
}

template<class T>
void RBTree<T>::RemoveFixup(RBTNode<T> *&root, RBTNode<T> *node, RBTNode<T> *parent) {
    RBTNode<T> *other;

    while ((!node || node->IsBlack()) && node != root) {
        if (node == parent->GetLeft()) {
            other = parent->GetRight();

            if (other->IsRed()) {
                other->SetBlack();
                parent->SetRed();
                LeftRotate(root, parent);
                other = parent->GetRight();
            }

            //x的兄弟w是黑色，且w的俩个孩子也都是黑色的
            if ((!other->GetLeft() || other->GetLeft()->IsBlack()) &&
                (!other->GetRight() || other->GetRight()->IsBlack())) {
                other->SetRed();
                node = parent;
                parent = node->GetParent();
            } else {
                //x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。
                if (!other->GetRight() || other->GetRight()->IsBlack()) {
                    other->GetLeft()->SetBlack();
                    other->SetRed();
                    RightRotate(root, other);
                    other = parent->GetRight();
                } else {
                    //x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色
                    other->SetColor(parent->GetColor());
                    parent->SetBlack();
                    other->GetRight()->SetBlack();
                    LeftRotate(root, parent);
                }
                node = root;
                break;
            }
        } else {
            other = parent->GetLeft();
            if (other->IsRed()) {
                other->SetBlack();
                parent->SetRed();
                RightRotate(root, parent);
                other = parent->GetLeft();
            }

            if ((!other->GetLeft() || other->GetLeft()->IsBlack()) &&
                (!other->GetRight() || other->GetRight()->IsBlack())) {
                other->SetRed();
                node = parent;
                parent = node->GetParent();
            } else {

                if (!other->GetLeft() || other->GetLeft()->IsBlack()) {
                    other->GetRight()->SetBlack();
                    other->SetRed();
                    RightRotate(root, other);
                    other = parent->GetLeft();
                } else {

                    other->SetColor(parent->GetColor());
                    parent->SetBlack();
                    other->GetLeft()->SetBlack();
                    RightRotate(root, parent);
                }
                node = root;
                break;
            }
        }
    }
    if (node) {
        node->SetBlack();
    }
}


template<class T>
void RBTree<T>::Destory(RBTNode<T> *&tree) {
    if (nullptr == tree) return;
    if (tree->GetLeft()) Destory(tree->GetLeft());
    if (tree->GetRight()) Destory(tree->GetRight());

    delete tree;
    tree = nullptr;
}

template<class T>
void RBTree<T>::Print(RBTNode<T> *&tree, T key, int direction) {
    if (tree) {
        if (direction == 0) { //root
            std::cout << std::setw(2) << tree->GetKey() << "(B) is root" << std::endl;
        } else {
            std::cout << std::setw(2) << tree->GetKey() << (tree->IsRed() ? "(R)" : "(B)") << " is " << std::setw(2)
                      << key
                      << "'s " << std::setw(12) << (direction == 1 ? "right child" : "left child") << std::endl;
        }
        Print(tree->GetLeft(), tree->GetKey(), -1);
        Print(tree->GetRight(), tree->GetKey(), 1);
    }
}


#endif