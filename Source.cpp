#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>

using namespace std;

const int NUMBER_OF_BITS = 4;

struct Node
{
    Node * left = nullptr;  // 0
    Node * right = nullptr; // 1
    int level = 0;
    int key = 0;
};

class XFastTrie
{
public:
    int m_numberOfLevels; // w = log(M)
    unordered_map<int, Node *> m_hashTables[NUMBER_OF_BITS+1];
    Node * m_root;
    Node * m_leftMost;
    Node * m_rightMost;

    XFastTrie();
    Node * successor(int number);
    Node * predecessor(int number);
    void insert(int number);
    void remove(int number);
    void print();
};

void XFastTrie::print()
{
    cout << " -----------------------------------------------" << endl;
    for (int level=0 ; level <= m_numberOfLevels; level ++)
    {
        cout << "level: " << level << " [";
        for (const auto& elem : m_hashTables[level])
        {
            assert(elem.second);
            cout << elem.first << ", ";
        }
        cout << "]" << endl;
    }
    cout << " -----------------------------------------------" << endl;
}

XFastTrie::XFastTrie()
{
    m_root = nullptr;
    m_leftMost = nullptr;
    m_rightMost = nullptr;
    m_numberOfLevels = NUMBER_OF_BITS;
}

Node * XFastTrie::successor(int number)
{
    int low = 0; // level zero os the root the number is always in the root
    int high = m_numberOfLevels + 1; // level M is the leaf level, high not inclusive
    int mid = 0;
    int prefix = 0;
    Node * tmp = m_root;

    if (m_root == nullptr)
    {
        return nullptr;
    }

    // binary search across the layers
    while (high - low > 1)
    {
        mid = (low + high) / 2;
        prefix = number >> (m_numberOfLevels - mid);
        if (m_hashTables[mid].find(prefix) == m_hashTables[mid].end())
        {
            // not found 
            high = mid;
        }
        else
        {
            // found
            low = mid;
            tmp = m_hashTables[mid][prefix];
        }
    }

    if (tmp->level == m_numberOfLevels)
    {
        // leaf node, found the number, return
        return tmp;
    }

    if (tmp->right == nullptr)
    {
        return nullptr;
    }

    if (tmp->right->level == m_numberOfLevels)
    {
        return tmp->right;
    }
    else
    {
        // assert missing left pointer
        assert(tmp->left == nullptr || tmp->left->level == m_numberOfLevels);
        if (tmp->left == nullptr)
        {
            return m_leftMost;
        }

        return tmp->left->right;
    }
}

void XFastTrie::insert(int number)
{
    if (m_root == nullptr)
    {
        m_root = new Node;
        m_root->level = 0;
        m_root->key = 0; // not really zero, just empty
    }

    // (1) Find successor and predecessor.
    Node * succ = successor(number);
    Node * pred = nullptr;

    if (succ == nullptr)
    {
        // no successor means the number being added is larger than all numbers in trie.
        pred = m_rightMost;
    }
    else 
    {
        pred = succ->left;
    }

    // (2) Wire number into linked list.
    Node * x = new Node;
    x->level = m_numberOfLevels; // x is a leaf
    x->key = number;
    if (succ == nullptr)
    {
        m_rightMost = x; // x is now the biggest number in trie
    }
    else
    {
        succ->left = x;
        x->right = succ;
    }
    if (pred == nullptr)
    {
        m_leftMost = x; // x is now the smallest number in trie
    }
    else
    {
        pred->right = x;
        x->left = pred;
    }
    m_hashTables[m_numberOfLevels][number] = x;

    // (3) Start at root and walk down to x, p and s
    int currentLevel = 0;
    Node * currentNode = m_root;

    Node * pred_branch_node = nullptr;
    Node * succ_branch_node = nullptr;

    while (currentLevel < m_numberOfLevels)
    {
        // retrieve bit
        int prefix = number >> (m_numberOfLevels - currentLevel);
        const int nextBit = (number >> (m_numberOfLevels - currentLevel - 1)) & 1;

        if (pred)
        {
            int pred_prefix = pred->key >> (m_numberOfLevels - currentLevel);
            if (pred_prefix == prefix)
            {
                pred_branch_node = currentNode;
            }
        }
        if (succ)
        {
            int succ_prefix = succ->key >> (m_numberOfLevels - currentLevel);
            if (succ_prefix == prefix)
            {
                succ_branch_node = currentNode;
            }
        }

        if (nextBit)
        {
            // go right
            if (currentNode->right == nullptr || currentNode->right->level == m_numberOfLevels) // leaf pointer
            {
                if (currentLevel != m_numberOfLevels - 1)
                {
                    const int nextPrefix = number >> (m_numberOfLevels - currentLevel - 1);
                    currentNode->right = new Node;
                    currentNode->right->level = currentLevel + 1;
                    currentNode->right->key = nextPrefix;
                    currentNode->right->left = pred;
                    currentNode->right->right = succ;
                    m_hashTables[currentLevel + 1][nextPrefix] = currentNode->right;
                }
                else
                {
                    currentNode->right = x;
                }
            }
            currentNode = currentNode->right;
        }
        else
        {
            //go left
            if (currentNode->left == nullptr || currentNode->left->level == m_numberOfLevels) // leaf pointer
            {
                if (currentLevel != m_numberOfLevels - 1)
                {
                    const int nextPrefix = number >> (m_numberOfLevels - currentLevel - 1);
                    currentNode->left = new Node;
                    currentNode->left->level = currentLevel + 1;
                    currentNode->left->key = nextPrefix;
                    currentNode->left->left = pred;
                    currentNode->left->right = succ;
                    m_hashTables[currentLevel + 1][nextPrefix] = currentNode->left;
                }
                else
                {
                    currentNode->left = x;
                }
            }
            currentNode = currentNode->left;
        }
        currentLevel++;
    }

    // Walk from pred_branch_node to pred and update right pointers
    if (pred_branch_node)
    {
        currentNode = pred_branch_node;
        while (currentNode != pred)
        {
            const int nextBit = (pred->key >> (m_numberOfLevels - currentNode->level - 1)) & 1;
            if (nextBit)
            {
                // go right
                currentNode = currentNode->right;
            }
            else
            {
                // go left
                if(currentNode->right == nullptr || currentNode->right->level == m_numberOfLevels)
                {
                    currentNode->right = x;
                }
                
                currentNode = currentNode->left;
            }
        }
    }

    // Walk from succ_branch_node to succ and update left pointers
    if (succ_branch_node)
    {
        currentNode = succ_branch_node;
        while (currentNode != succ)
        {
            const int nextBit = (succ->key >> (m_numberOfLevels - currentNode->level - 1)) & 1;
            if (nextBit)
            {
                // go right
                if(currentNode->left == nullptr || currentNode->left->level == m_numberOfLevels)
                {
                    currentNode->left = x;
                }
                
                currentNode = currentNode->right;
            }
            else
            {
                // go left
                currentNode = currentNode->left;
            }
        }
    }
}

void test_successor(XFastTrie & trie)
{
    for (int i = 0; i < 16; i++)
    {
        Node * n = trie.successor(i);
        if (n)
        {
            cout << "successor of " << i << " is " << n->key << endl;
        }
        else
        {
            cout << "successor of " << i << " is " << "nul" << endl;
        }
    }
}

int main() {
    XFastTrie trie;
    trie.insert(15);
    //trie.print();
    test_successor(trie);
    trie.insert(9);
    //trie.print();
    test_successor(trie);
    trie.insert(10);
    //trie.print();
    test_successor(trie);
    trie.insert(2);
    //trie.print();
    test_successor(trie);
    trie.insert(1);
    //trie.print();
    test_successor(trie);
    trie.insert(0);
    //trie.print();
    test_successor(trie);
    
    return 0;
}