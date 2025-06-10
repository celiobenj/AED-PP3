#include <iostream>

template <typename Key>
class AVLTree
{
private:
    struct AVLNode
    {
        Key key;
        AVLNode *left;
        AVLNode *right;
        int bal;

        AVLNode(const Key &k) : key(k), left(nullptr), right(nullptr), bal(0) {}
    };
    AVLNode *pRoot;

    void insert(const Key &k, AVLNode *&p, bool &h)
    {
        if (p == nullptr)
        {
            p = new AVLNode(k);
            h = true;
        }
        else if (k < p->key) // inserir na esquerda
        {
            insert(k, p->left, h);
            if (h)
            {
                if (p->bal == 1)
                {
                    p->bal = 0;
                    h = false;
                }
                else if (p->bal == 0)
                {
                    p->bal = -1;
                }
                else
                {
                    AVLNode *p1 = p->left;
                    if (p1->bal == -1) // Rotacao Simples Direita (RSD)
                    { 
                        p->left = p1->right;
                        p1->right = p;
                        p->bal = 0;
                        p = p1;
                    }
                    else // Rotacao Dupla Direita (RDD)
                    { 
                        AVLNode *p2 = p1->right;
                        p1->right = p2->left;
                        p2->left = p1;
                        p->left = p2->right;
                        p2->right = p;
                        p->bal = (p2->bal == -1) ? 1 : 0;
                        p1->bal = (p2->bal == 1) ? -1 : 0;
                        p = p2;
                    }
                    p->bal = 0;
                    h = false;
                }
            }
        }
        else if (k > p->key) // inserir na direita
        {
            insert(k, p->right, h);
            if (h)
            {
                if (p->bal == -1)
                {
                    p->bal = 0;
                    h = false;
                }
                else if (p->bal == 0)
                {
                    p->bal = 1;
                }
                else
                {
                    AVLNode *p1 = p->right;
                    if (p1->bal == 1) // Rotacao Simples Esquerda (RSE)
                    {
                        p->right = p1->left;
                        p1->left = p;
                        p->bal = 0;
                        p = p1;
                    }
                    else // Rotacao Dupla Esquerda (RDE)
                    {
                        AVLNode *p2 = p1->left;
                        p1->left = p2->right;
                        p2->right = p1;
                        p->right = p2->left;
                        p2->left = p;
                        p->bal = (p2->bal == 1) ? -1 : 0;
                        p1->bal = (p2->bal == -1) ? 1 : 0;
                        p = p2;
                    }
                    p->bal = 0;
                    h = false;
                }
            }
        }
        else
        {
            h = false;
        }
    }
    
    bool search(const Key &k, AVLNode *p) const
    {
        if (p == nullptr)
            return false;

        if (k < p->key)
            return search(k, p->left);

        if (k > p->key)
            return search(k, p->right);

        return true;
    }
    
    int getHeight(AVLNode *p) const
    {
        if (p == nullptr)
            return 0; 
        
        int leftHeight = getHeight(p->left);
        int rightHeight = getHeight(p->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    void clear(AVLNode *&p)
    {
        if (p != nullptr)
        {
            clear(p->left);
            clear(p->right);
            delete p;
            p = nullptr;
        }
    }

public:
    AVLTree() : pRoot(nullptr) {}

    ~AVLTree() { clear(pRoot); }

    void insert(const Key &k)
    {
        bool h = false; // h (height increased) inicializa como falso
        insert(k, pRoot, h);
    }

    bool search(const Key &k) const
    {
        return search(k, pRoot);
    }

    // retorna a altura
    int getHeight() const
    {
        return getHeight(pRoot);
    }
};
