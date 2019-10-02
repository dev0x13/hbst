#include <cstddef>
#include <limits>
#include <iostream>
#include <sstream>

template <typename ValueT>
struct BinaryTreeNode {
    const size_t hiddenKey;
    const ValueT value;
    BinaryTreeNode *left = nullptr;
    BinaryTreeNode *right = nullptr;

    BinaryTreeNode(const size_t hiddenKey_, const ValueT& value_) : hiddenKey(hiddenKey_), value(value_) {}

    ~BinaryTreeNode() {
        delete left;
        delete right;
    }
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

        insert(&root, newHiddenKey, newValue, 0, maxKeyValue);
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
        // TODO: implement
        /*
        NodeT** foundNode = search(&root, KeyHasherT::toSizeT(key), 0, maxKeyValue);

        if (foundNode) {

            delete *foundNode;
            *foundNode = nullptr;
        }
        */
    }

    void printTree() {
        printTree("", root, false);
    }

private:
    void insert(NodeT** r, const size_t newHiddenKey, const ValueT& newValue, size_t min, size_t max) {
        if (!(*r)) {
            *r = new NodeT(newHiddenKey, newValue);
            return;
        }

        if ((*r)->hiddenKey == newHiddenKey) {
            delete *r;
            *r = new NodeT(newHiddenKey, newValue);
            return;
        }

        const size_t hiddenRef = (min + max) / 2;

        if (newHiddenKey < hiddenRef) {
            insert(&(*r)->left, newHiddenKey, newValue, min, hiddenRef);
        } else {
            insert(&(*r)->right, newHiddenKey, newValue, hiddenRef, max);
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