#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
#include <time.h>

using namespace std;

const int NUMBER_OF_BITS = sizeof(uint64_t) * 8;

uint64_t rand64()
{
    return ((uint64_t)rand() << 32) | rand();
}

struct Node
{
    Node * left = nullptr;  // 0
    Node * right = nullptr; // 1
    int level = 0;
    uint64_t key = 0;
};

class XFastTrie
{
public:
    int m_numberOfLevels; // w = log(M)
    unordered_map<uint64_t, Node *> m_hashTables[NUMBER_OF_BITS+1];
    Node * m_root;
    Node * m_leftMost;
    Node * m_rightMost;

    XFastTrie();
    Node * successor(uint64_t number);
    Node * predecessor(uint64_t number);
    Node * lookup(uint64_t number);
    void insert(uint64_t number);
    void remove(uint64_t number);
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

Node * XFastTrie::lookup(uint64_t number)
{
    std::unordered_map<uint64_t, Node*>::const_iterator it = m_hashTables[m_numberOfLevels].find(number);
    if (it == m_hashTables[m_numberOfLevels].end())
    {
        return nullptr;
    }
    return it->second;
}

XFastTrie::XFastTrie()
{
    m_root = nullptr;
    m_leftMost = nullptr;
    m_rightMost = nullptr;
    m_numberOfLevels = NUMBER_OF_BITS;
}

Node * XFastTrie::successor(uint64_t number)
{
    int low = 0; // level zero os the root the number is always in the root
    int high = m_numberOfLevels + 1; // level M is the leaf level, high not inclusive
    int mid = 0;
    uint64_t prefix = 0;
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

void XFastTrie::insert(uint64_t number)
{
    // Don't add duplicate numbers
    if (lookup(number) != nullptr)
    {
        return;
    }

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
        uint64_t prefix = number >> (m_numberOfLevels - currentLevel);
        const int nextBit = (number >> (m_numberOfLevels - currentLevel - 1)) & 1;

        if (pred)
        {
            uint64_t pred_prefix = pred->key >> (m_numberOfLevels - currentLevel);
            if (pred_prefix == prefix)
            {
                pred_branch_node = currentNode;
            }
        }
        if (succ)
        {
            uint64_t succ_prefix = succ->key >> (m_numberOfLevels - currentLevel);
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
                    const uint64_t nextPrefix = number >> (m_numberOfLevels - currentLevel - 1);
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
                    const uint64_t nextPrefix = number >> (m_numberOfLevels - currentLevel - 1);
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

void test_successor()
{
    XFastTrie trie;

    int array[100];

    for (int i = 0; i<100; i++)
    {
        array[i] = rand();
        trie.insert(array[i]);
    }

    sort(array, array + 100);

    for (int i = 0; i<100; i++)
    {
        cout << array[i] << endl;
    }

    for (int i = 0; i < 100; i += 5)
    {
        int r = rand();
        uint64_t s = trie.successor(r)->key;

        cout << "successor of " << r << " is " << s << endl;
    }
}

void test_successor_64bit()
{
    XFastTrie trie;

    uint64_t array[100];

    for (int i = 0; i<100; i++)
    {
        array[i] = rand64();
        trie.insert(array[i]);
    }

    sort(array, array + 100);

    for (int i = 0; i<100; i++)
    {
        cout << array[i] << endl;
    }

    for (int i = 0; i < 100; i += 5)
    {
        uint64_t r = rand64();
        uint64_t s = trie.successor(r)->key;

        cout << "successor of " << r << " is " << s << endl;
    }
}

void test_lookup()
{
    XFastTrie trie;

    int array[100];

    for (int i = 0; i<100; i++)
    {
        array[i] = rand();
        trie.insert(array[i]);
    }

    sort(array, array + 100);

    for (int i = 0; i<100; i++)
    {
        Node * n = trie.lookup(array[i]);
        assert(n != nullptr);
    }
}

void test_64bit_xfast(int N)
{
    XFastTrie trie;

    clock_t tStart = clock();
    for (int i = 0; i<N; i++)
    {
        trie.insert(rand64());
    }
    printf("Inserting %i numbers into X-fast trie takes: %.2fs\n", N, (double)(clock() - tStart) / CLOCKS_PER_SEC);
    
    tStart = clock();
    for (int i = 0; i < N; i++)
    {
        uint64_t r = rand64();
        Node* s = trie.successor(r);
    }
    printf("%i successor operations on X-fast trie takes: %.2fs\n", N, (double)(clock() - tStart) / CLOCKS_PER_SEC);
}

void test_64bit_bst(int N)
{
    map<uint64_t, Node*> bst;

    clock_t tStart = clock();
    for (int i = 0; i<N; i++)
    {
        bst[rand64()] = nullptr;
    }
    printf("Inserting %i numbers into BST takes: %.2fs\n", N, (double)(clock() - tStart) / CLOCKS_PER_SEC);

    tStart = clock();
    for (int i = 0; i < N; i++)
    {
        uint64_t r = rand64();
        auto s = bst.lower_bound(r);
    }
    printf("%i successor operations on BST takes: %.2fs\n", N, (double)(clock() - tStart) / CLOCKS_PER_SEC);
}

int main() {
    
    test_64bit_bst(1000000);
    return 0;
}