#include <iostream>
#include <string>
#include <cmath>

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

template <typename T>
class HashTable
{
private:
    AVLTree<T> *t; // vetor de arvores AVL
    static const int m = 151;

    size_t hash(const std::string &key) const
    {
        size_t hashValue = 0;
        size_t n = key.length();
        for (size_t i = 0; i < n; ++i)
        {
            hashValue += key[i] * static_cast<size_t>(pow(128, n - i - 1));
            hashValue %= m;
        }
        return hashValue;
    }

public:
    HashTable() { t = new AVLTree<T>[m]; }

    ~HashTable() { delete[] t; }

    void insert(const T &key)
    {
        size_t index = hash(key);
        t[index].insert(key); // insere na arvore AVL do bucket
    }

    bool search(const T &key) const
    {
        size_t index = hash(key);
        return t[index].search(key);
    }

    size_t getHeight(const T &key) const
    {
        size_t index = hash(key);
        return t[index].getHeight(); // altura da AVL onde a chave deve estar
    }
};

// limpa a palavra antes de entrar na hashtable
std::string cleanWord(const std::string &word)
{
    std::string result;
    for (char c : word)
    {
        if (isalnum(c))
            result += c;
    }
    return result;
}

// processa as linhas para adicionar cada palavra na hashtable
void processLine(const std::string &line, HashTable<std::string> &table)
{
    std::string word;
    for (char c : line)
    {
        if (c == ' ' || c == '\t')
        {
            if (!word.empty())
            {
                table.insert(cleanWord(word));
                word.clear();
            }
        }
        else
        {
            word += c;
        }
    }

    if (!word.empty())
    {
        table.insert(cleanWord(word));
    }
}

// leitura do texto e da chave que deseja buscar
std::string enterText(HashTable<std::string> &table)
{
    std::string line;
    std::string key;

    while (std::getline(std::cin, line))
    {
        if (line.substr(0, 3) == "###")
        {
            key = cleanWord(line.substr(4));
            break;
        }
        else
        {
            processLine(line, table);
        }
    }

    return key;
}

int main()
{
    HashTable<std::string> table;

    std::string key = enterText(table);

    if (table.search(key))
        std::cout << table.getHeight(key) << std::endl;
    else
        std::cout << "Nao encontrado\n";

    return 0;
}
