#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <fstream>
#include <chrono>
using namespace std;

const int NUMBER_OF_BITS = 32;

class TreeNode{
    public:
    int key;
    TreeNode* left;
    TreeNode* right;
    TreeNode* p;
    public:
    TreeNode();
    TreeNode(int val);
    
};
TreeNode::TreeNode()
{
    key=0;
    left=right=p=NULL;
}
TreeNode::TreeNode(int val)
{
    key=val;
    left=right=p=NULL;
}
void inorder_tree_walk(TreeNode* x)
{
    if(x!=NULL)
    {
        inorder_tree_walk(x->left);
        cout<<x->key<<" ";
        inorder_tree_walk(x->right);
    }else 
    return;
}
void preorder_tree_walk(TreeNode*x)
{
    if(x)
    {
        cout<<x->key<<" ";
        preorder_tree_walk(x->left);
        preorder_tree_walk(x->right);
    }
}
TreeNode* tree_search(TreeNode* x, int k)
{
    if(x==NULL||k==x->key)
    return x;
    if(k<x->key)
    return tree_search(x->left,k);
    else return tree_search(x->right,k);
}
TreeNode* tree_minmum(TreeNode* x)
{
    while(x->left!=NULL)
    x=x->left;
    return x;
}
TreeNode* tree_maxmum(TreeNode* x)
{
    while(x->right!=NULL)
    x=x->right;
    return x;
}
TreeNode* tree_successor(TreeNode* x)
{
    if(x->right) 
    return tree_minmum(x->right);
    TreeNode* y=x->p;
    while(y&& x==y->right)
    {
        x=y;
        y=y->p;
    }
    return y;
} 
TreeNode* tree_predecessor(TreeNode* x)
{
    if(x->left) 
    return tree_maxmum(x->left);
    TreeNode* y=x->p;
    while(y&& x==y->left)
    {
        x=y;
        y=y->p;
    }
    return y;
} 

void tree_insert(TreeNode* T, TreeNode* z) 
{
    TreeNode * y,*x=T;
    while(x)
    {
        y=x;
        if(z->key<x->key)
        x=x->left;
        else x=x->right;
     } 
     z->p=y;
     if(!y)
     T=z;
     else if(z->key<y->key)
     y->left=z;
     else y->right=z;

} 

void transplant(TreeNode* T, TreeNode* u,TreeNode* v)
{
    if(u->p==NULL)
    T=v;
    else if(u==u->p->left) 
    u->p->left=v;
    else u->p->right=v;
    if(v)
    v->p=u->p;
 } 
 void tree_delete(TreeNode* T, TreeNode* z)
 {
    if(!z->left)
    transplant(T,z,z->right);
    else if(!z->right)
    transplant(T,z,z->left);
    else {
        TreeNode* y=tree_minmum(z->right);
        if(y->p!=z)
        {
            transplant(T,y,y->right);
            y->right=z->right;
            y->right->p=y;
         }
         transplant(T,z,y);
         y->left=z->left;
         y->left->p=y;
     }
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


int main(int argc, char *argv[]) {
    chrono::time_point<chrono::steady_clock> start, stop; 
    chrono::duration<double> difference_in_time;
    double difference_in_seconds;  // Holds the final run time
      /* Local data */
    int n=0;
      // Input Data:  DO NOT CHANGE 

    cout<<"# N\tX-fast_Insert\tBST_Insert\tX-fast_successor\t BST_successor\n";
    for(int j=0;j<20;j++)
    {
        n+=2000;
    int* data = NULL;
    data = new int[n];
    for(int i=0; i<n ; i++)  data[i] = rand()%100000;
    //cout<<"Start inserting element into X-fast Trie ...\n";
    cout<<n<<"\t";
    start = chrono::steady_clock::now();
    XFastTrie trie;
    for(int i=0;i<n;i++)
        trie.insert(data[i]);
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds = double(difference_in_time.count());
    cout<<difference_in_seconds<<"\t";

    //cout<<"Finished.\n";
    //cout<<"Time:  "<<difference_in_seconds_heap_sort<<"\n";

    start = chrono::steady_clock::now();

    //cout<<"Start inserting element into BST ...\n";
    TreeNode *T=new TreeNode(data[0]);
    for(int i=1;i<n;i++)
    {
        TreeNode* z=new TreeNode(data[i]);
        tree_insert(T,z);
        delete z;
    }
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds = double(difference_in_time.count());
    cout<<difference_in_seconds<<"\t";
    //cout<<"Finished.\n";
    //cout<<"Time:  "<<difference_in_seconds_heap_sort<<"\n";
    

    //cout<<"Finding successor in X-fast trie ...\n";
    start = chrono::steady_clock::now();
    for(int i=0;i<n;i++)
        Node *Xs=trie.successor(data[i]);
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds = double(difference_in_time.count());
    cout<<difference_in_seconds<<"\t\t";
    //cout<<"Finished.\n";
    //cout<<"Time:  "<<difference_in_seconds_heap_sort<<"\n";

    //cout<<"Finding successor in BST trie ...\n";
    start = chrono::steady_clock::now();
    for(int i=0;i<n;i++)
    {
        TreeNode* z=new TreeNode(data[i]);
        tree_successor(z);
        delete z;
    }   
    stop = chrono::steady_clock::now();
    difference_in_time = stop - start;
    difference_in_seconds = double(difference_in_time.count());
    cout<<difference_in_seconds<<"\n";
    //cout<<"Finished.\n";
    //cout<<"Time:  "<<difference_in_seconds_heap_sort<<"\n";

    delete[] data;
    }
}