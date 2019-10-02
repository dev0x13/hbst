#include <cstddef>
#include <limits>
#include <iostream>
#include <sstream>

template <typename ValueT>
struct BinaryTreeNode {
    size_t hiddenKey;
    ValueT value;
    BinaryTreeNode *left = nullptr;
    BinaryTreeNode *right = nullptr;
    BinaryTreeNode *parent = nullptr;

    BinaryTreeNode(const size_t hiddenKey_, const ValueT& value_, BinaryTreeNode* parent_) :
        hiddenKey(hiddenKey_), value(value_), parent(parent_) {}
};

template <typename KeyT, typename ValueT, typename KeyHasherT>
class HBST {
public:
    using NodeT = BinaryTreeNode<ValueT>;

    explicit HBST(size_t maxKeyValue_ = std::numeric_limits<size_t>::max()) : maxKeyValue(maxKeyValue_) {}

    void insertValue(const KeyT& newKey, const ValueT& newValue) {
        const size_t newHiddenKey = KeyHasherT::toSizeT(newKey);

        if (newHiddenKey > maxKeyValue) {
            std::stringstream ss;
            ss << "New hidden key if greater than max key value: " << newHiddenKey << " > " << maxKeyValue;
            throw std::runtime_error(ss.str());
        }

        insert(&root, nullptr, newHiddenKey, newValue, 0, maxKeyValue);
    }

    bool searchValue(const KeyT& key, ValueT& foundValue) {
        NodeT** foundNode = search(&root, KeyHasherT::toSizeT(key), 0, maxKeyValue);

        if (!foundNode || !(*foundNode)) {
            return false;
        } else {
            foundValue = (*foundNode)->value;
            return true;
        }
    }

    void deleteValue(const KeyT& key) {
        del(&root, KeyHasherT::toSizeT(key), 0, maxKeyValue);
    }

    void printTree() {
        printTree("", root, false);
    }

private:
    void insert(NodeT** r, NodeT *parent, const size_t newHiddenKey, const ValueT& newValue, size_t min, size_t max) {
        if (!(*r)) {
            *r = new NodeT(newHiddenKey, newValue, parent);
            return;
        }

        if ((*r)->hiddenKey == newHiddenKey) {
            (*r)->value = newValue;
            return;
        }

        const size_t hiddenRef = (min + max) / 2;

        if (newHiddenKey < hiddenRef) {
            insert(&(*r)->left, *r, newHiddenKey, newValue, min, hiddenRef);
        } else {
            insert(&(*r)->right, *r, newHiddenKey, newValue, hiddenRef, max);
        }
    }

    NodeT** search(NodeT** r, const size_t hiddenKey, size_t min, size_t max) {
        if (!(*r) || min > max) {
            return nullptr;
        }

        if ((*r)->hiddenKey == hiddenKey) {
            return r;
        }

        const size_t hiddenRef = (min + max) / 2;

        if (hiddenKey < hiddenRef) {
            return search(&(*r)->left, hiddenKey, min, hiddenRef);
        } else {
            return search(&(*r)->right, hiddenKey, hiddenRef, max);
        }
    }

    void replaceNodeInParent(NodeT* r, NodeT* newNode) {
        if (r) {
            if (!r->parent) {
                root = newNode;
            } else if (r == r->parent->left) {
                r->parent->left = newNode;
            } else {
                r->parent->right = newNode;
            }

            delete r;

            if (newNode) {
                newNode->parent = r->parent;
            }
        }
    }

    void del(NodeT** r, const size_t hiddenKey, size_t min, size_t max) {
        if (!r || !(*r)) {
            return;
        }

        const size_t hiddenRef = (min + max) / 2;

        if ((*r)->hiddenKey == hiddenKey) {
            if ((*r)->left && (*r)->right) {
                NodeT **successor = &(*r)->right;

                size_t hiddenRef2 = (hiddenRef + max) / 2;

                while ((*successor)->left && (*successor)->hiddenKey < hiddenRef2) {
                    successor = &(*successor)->left;
                    hiddenRef2 = (min + hiddenRef2) / 2;
                }

                (*r)->hiddenKey = (*successor)->hiddenKey;
                (*r)->value = (*successor)->value;
                del(successor, (*successor)->hiddenKey, hiddenRef, max);
            } else if ((*r)->left) {
                replaceNodeInParent(*r, (*r)->left);
            } else if ((*r)->right) {
                replaceNodeInParent(*r, (*r)->right);
            } else {
                replaceNodeInParent(*r, nullptr);
            }

            return;
        }

        if (hiddenKey < hiddenRef) {
            del(&(*r)->left, hiddenKey, min, hiddenRef);
        } else {
            del(&(*r)->right, hiddenKey, hiddenRef, max);
        }
    }

    void printTree(const std::string& prefix, const NodeT* node, bool isLeft) {
        if (node) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──" );
            std::cout << node->value << "[" << node->hiddenKey << "]" << std::endl;
            printTree(prefix + (isLeft ? "│   " : "    "), node->left, true);
            printTree(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    NodeT *root = nullptr;
    const size_t maxKeyValue;
};