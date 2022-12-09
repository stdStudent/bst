#ifndef DDP_DDT_H
#define DDP_DDT_H

#include <iostream>
#include <string>

using namespace std;

template <typename T>
class ddt {
    struct TREE {
        TREE* left;
        TREE* right;

        string key;
        T      value;

        int height{};
    };

    TREE* tree;

    // Calculating a node's height based on parents
    int getMaxHeightFromChildren(const int numberOne, const int numberTwo)
    {
        return (numberOne > numberTwo) ? numberOne : numberTwo;
    }

    /********************************/
    /** Node's fields manipulation **/
    /********************************/
    int heightOfNode(TREE* const root)
    {
        return root ? root->height : 0;
    }

    int updateHeight(TREE* const root)
    {
        if (root) {
            root->height = 1 + getMaxHeightFromChildren(heightOfNode(root->left), heightOfNode(root->right));
            return root->height;
        }

        return 0;
    }

    int balanceFactorOfNode(TREE* const root)
    {
        return root ? heightOfNode(root->left) - heightOfNode(root->right) : 0;
    }

    /**********************/
    /** Balancing a tree **/
    /**********************/
    TREE* rotateRight(TREE* const root)
    {
        if (root) {
            TREE* const leftChild        = root->left;
            TREE* const rightOfLeftChild = leftChild->right;

            leftChild->right = root;
            root->left       = rightOfLeftChild;

            /* Get children's heights. */
            updateHeight(root);
            updateHeight(leftChild);

            return leftChild;
        }

        return root;
    }

    TREE* rotateLeft(TREE* const root)
    {
        if (root) {
            TREE* rightChild       = root->right;
            TREE* leftOfRightChild = rightChild->left;

            rightChild->left = root;
            root->right      = leftOfRightChild;

            /* Get children's heights. */
            updateHeight(root);
            updateHeight(rightChild);

            return rightChild;
        }

        return root;
    }

    /*************/
    /** Helpers **/
    /*************/
    void freeTree(TREE* const root)
    {
        if (root) {
            freeTree(root->left);
            freeTree(root->right);

            // TODO: make it more CPP-like.
            free(root->value);
            free(root->key);
            free(root);
        }
    }

    TREE* createAndInitTreeNode(const string& key, const T& value)
    {
        // TODO: make it more CPP-like.
        TREE* newNode = (TREE*)malloc(sizeof(TREE));

        newNode->key    = key;
        newNode->value  = value;
        newNode->height = 1;
        newNode->left   = newNode->right = nullptr;

        return newNode;
    }

    TREE* clearTree(TREE* const root, const string& key, const T& value)
    {
        freeTree(root);

        return createAndInitTreeNode(key, value);
    }

    /**************/
    /** Printing **/
    /**************/
    void printTreeNode(TREE* const root)
    {
        if (root)
            cout << "["
                 << " Key: "    << root->key   << ", "
                 << " Value: "  << root->value << ", "
                 << " Height: " << root->height //<< ", "
                 //<< " Balance Factor:" << balanceFactorOfAVLNode(root)
                 <<" ]\n";
    }

    void printTree_indented(TREE* const root, const char seperator, const char repeater)
    {
        if (root) {
            printTree_indented(root->left, seperator, repeater + 2);

            for(int i = 0; i < repeater; ++i)
                printf("%c", seperator);

            printTreeNode(root);

            printTree_indented(root->right, seperator, repeater + 2);
        }
    }

    void printTree(TREE* const root)
    {
        putchar('\n');
        printTree_indented(root, '\t', 0);
    }

    // Use simple hash function
    size_t hash(const string& s)
    {
        const char* cp = s.c_str();

        size_t hash = 0x811c9dc5;
        while (*cp) {
            hash ^= (unsigned char) *cp++;
            hash *= 0x01000193;
        }
        return hash;
    }

    TREE* insertTreeNode(TREE* root, const string& key, const T& value)
    {
        size_t key_hash = hash(key);
        size_t root_key_hash;
        if (root != NULL)
            root_key_hash = hash(root->key);
        else
            root_key_hash = 0;

        if (root) {
            if (root_key_hash != key_hash) { // do NOT insert duplicates
                if (key_hash < root_key_hash)
                    root->left = insertTreeNode(root->left, key, value);
                else if (key_hash > root_key_hash)
                    root->right = insertTreeNode(root->right, key, value);

                updateHeight(root);

                const int rootBalanceFactor = balanceFactorOfNode(root);
                if (rootBalanceFactor > 1) {
                    size_t root_left_key_hash = hash(root->left->key);
                    if(key_hash > root_left_key_hash)
                        root->left = rotateLeft(root->left);

                    root = rotateRight(root);
                } else if (rootBalanceFactor < -1) {
                    size_t root_right_key_hash = hash(root->right->key);
                    if (key_hash < root_right_key_hash)
                        root->right = rotateRight(root->right);

                    root = rotateLeft(root);
                }
            }

            return root;
        }

        return createAndInitTreeNode(key, value);
    }


    TREE* either(TREE* firstChoice, TREE* secondChoice)
    {
        return firstChoice ? firstChoice : secondChoice;
    }

    TREE* removeNode(TREE* root, const string& key)
    {
        size_t key_hash = hash(key);
        size_t root_key_hash;
        if (root != NULL)
            root_key_hash = hash(root->key);
        else
            root_key_hash = 0;

        if (root) {
            if (key_hash < root_key_hash)
                root->left = removeNode(root->left, key);
            else if (key_hash > root_key_hash)
                root->right = removeNode(root->right, key);
            else {
                /* Check for leaf node: both left and right child nodes are empty. */
                if (!(root->left || root->right)) {
                    free(root);
                    return NULL;
                } else if (root->left == NULL ^ root->right == NULL) { // If either are empty
                    TREE* temp = either(root->right, root->left); // No code duplication
                    free(root);
                    root = temp;
                } else {
                    TREE* temp = root->left;
                    while(temp->right) // Find the maximum key in the left subtree
                        temp = temp->right;

                    root->key = temp->key;
                    root->left = removeNode(root->left, temp->key);
                }
            }

            updateHeight(root);

            int balanceFactorOfRoot = balanceFactorOfNode(root);
            if (1 < balanceFactorOfRoot) {
                /* If there is an redundant node in the right subtree
                 * then the right rotation will NOT balance the number of nodes
                 * since this redundant node is located on the right side of the left node.
                 */
                if (balanceFactorOfNode(root->left) < 0)
                    root->left = rotateLeft(root->left);

                root = rotateRight(root);
            } else if (balanceFactorOfRoot < -1) {
                /* If the left subtree of the right node contains lots of leaves
                 * then it is worth moving the redundant node to the right subtree.
                 */
                if(balanceFactorOfNode(root->right) > 0)
                    root->right = rotateRight(root->right);

                root = rotateLeft(root);
            }

        }

        return root;
    }

    T getValueByKey(TREE* root, const string& key)
    {
        if (root == NULL)
            return "  __NO_SUCH_KEY_FOUND__  ";

        size_t key_hash = hash(key);
        size_t root_value_hash = hash(root->key);

        if (key_hash == root_value_hash) {
            return root->value;
        }
        else if (key_hash < root_value_hash)
            return getValueByKey(root->left, key);
        else  /* key_hash > root_value_hash */
            return getValueByKey(root->right, key);
    }

public:
    ddt(const string& key, const T& value) {
        tree = createAndInitTreeNode(key, value);
    }

    // TODO: create proper destructor.

    void insert(const string& key, const T& value) {
        tree = insertTreeNode(tree, key, value);
    }

    void remove(const string& key) {
        tree = removeNode(tree, key);
    }

    // FIXME:
    void clear() {
        freeTree(tree);
    }

    // FIXME:
    void reinit(const string& key, const T& value) {
        clearTree(tree, key, value);
    }

    string find(const string& key) {
        return getValueByKey(tree, key);
    }

    void print() {
        printTree(tree);
    }
};

#endif //DDP_DDT_H
