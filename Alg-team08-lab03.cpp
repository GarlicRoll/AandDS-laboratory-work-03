using namespace std;

#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#include <algorithm>

// Класс «узел дерева»
class Node { char d; //тег узла
    public:
    Node * lft; // левый сын
    Node * mdl; //— средний сын (если нужно)
    Node * rgt; // правый сын
    Node() : lft(nullptr), rgt(nullptr) { } // конструктор узла
    ~Node(){ if(lft) delete lft; // деструктор (уничтожает поддерево)
    if (rgt) delete rgt; }
    friend class Tree; // дружественный класс «дерево»
} ;
// Класс «дерево в целом»
class Tree
{ 
    
    char num, maxnum; //счётчик тегов и максимальный тег
    int maxrow, offset; //максимальная глубина, смещение корня
    char ** SCREEN; // память для выдачи на экран
    void clrscr(); // очистка рабочей памяти
    Node* MakeNode(int depth); // создание поддерева
    void OutNodes(Node * v, int r, int c); // выдача поддерева
    Tree (const Tree &); // фиктивный конструктор копии
    Tree (Tree &&); //копия с переносом (С++11)
    Tree operator = (const Tree &) const = delete; // присваивание
    Tree operator = (Tree &&) const = delete; // то же, с переносом
    public:
    Node * root; // указатель на корень дерева
    Tree(char num, char maxnum, int maxrow);
    ~Tree();
    void MakeTree() // ввод — генерация дерева
    { root = MakeNode(0); }
    bool exist() { return root != nullptr; } // проверка «дерево не пусто»
    int DFS(); // обход дерева «в глубину»
    int BFS(); // обход «в ширину»
    // рекурсивные функции
    void InOrder(Node*); // В глубину симметрия
    int GetMaxDepth(Node*);
    void OutTree(); // выдача на экран
};

Tree :: Tree(char nm, char mnm, int mxr):
    num(nm), maxnum(mnm), maxrow(mxr), offset(40), root(nullptr),
    SCREEN(new char * [maxrow])
    { for(int i = 0; i < maxrow; ++i ) SCREEN[ i ] = new char[ 80 ]; }
Tree :: ~Tree( ) { for(int i = 0; i < maxrow; ++i) delete [ ]SCREEN[ i ];
    delete [ ]SCREEN; delete root; }

Node * Tree :: MakeNode(int depth)
{ Node * v = nullptr;
    int Y = (depth < rand( )%4+1) && (num <= 'z');
    //Вариант: cout << "Node (" << num << ',' << depth << ")1/0: "; cin >> Y;
    if (Y) { // создание узла, если Y = 1
    v = new Node;
    //v->d = num++; // разметка в прямом порядке (= «в глубину»)
    v->lft = MakeNode(depth+1);
    //v->d = num++; //вариант — во внутреннем
    v->rgt = MakeNode(depth+1);
    v->mdl = MakeNode(depth+1);
    v->d = num++; // вариант — в обратном
    }
    return v;
}

void Tree :: clrscr( )
{   for(int i = 0; i < maxrow; i++)
    memset(SCREEN[i], '.', 80);
}

void Tree :: OutTree( )
{   clrscr( );
    OutNodes(root, 1, offset);
    for (int i = 0; i < maxrow; i++)
    { SCREEN[ i ][ 79 ] = 0;
    cout << '\n' << SCREEN[ i ];
    }
    cout << '\n';
}

void Tree :: OutNodes(Node * v, int r, int c)
{   
    if (r && c && (c<80)) SCREEN[ r - 1 ][ c - 1 ] = v->d; // вывод метки
    if (r < maxrow) {
        if (v->lft) OutNodes(v->lft, r + 1, c - (offset >> r) + 2); //левый сын
        if (v->mdl) OutNodes(v->mdl, r + 1, c); //– средний сын (если нужно)
        if (v->rgt) OutNodes(v->rgt, r + 1, c + (offset >> r) - 2); //правый сын
    }
}

template <class Item> class STACK
{   
    Item * S; int t;
    public:
    STACK(int maxt) : S(new Item[ maxt ]), t( 0 ) { }
    int empty( ) const { return t == 0; }
    void push(Item item) { S[t++] = item; }
    Item pop( ) {return ( t ? S[ --t ] : 0 ); }
};

template <class Item> class QUEUE
{   Item * Q; int h, t, N;
    public:
    QUEUE(int maxQ): h(0), t(0), N(maxQ), Q(new Item[maxQ + 1]) { }
    int empty( ) const { return (h % N) == t; }
    void push(Item item) { Q[ t++ ] = item; t %= N; }
    Item pop( ) { h %= N; return Q[ h++ ]; }
};

// Внутренний рексурсивный обход
void Tree :: InOrder(Node *root)
{
    if (root == NULL)
    {   
        return;
    }
    // Сначала левый сын, потом правый, потом остальные
    InOrder(root->lft);
    cout << root->d << '_'; // выдать тег, счёт узлов
    InOrder(root->rgt);
    InOrder(root->mdl); // Остальные сыновья
}

// Максимальная глубина дерева
int Tree :: GetMaxDepth(Node *root)
{
    if (root == NULL)
        return 0;
    int right, middle, left;
    right = GetMaxDepth(root->rgt);
    middle = GetMaxDepth(root->mdl);
    left = GetMaxDepth(root->lft);
    return max(max(right, middle), max(left, middle)) + 1; 
}

// Обход внутренний (но только для бинарного дерева)
int Tree :: DFS( )
{   
    const int MaxS = 20; // максимальный размер стека
    int count = 0;
    STACK <Node *> S(MaxS); //создание стека указателей на узлы
    Node * v = root;
    while (!S.empty( ) || v != NULL) // Пока стек не пуст…
    {
        if (v != NULL)
        {   
            S.push(v);
            v = v->lft;
        }
        else
        {
            v = S.pop();
            cout << v->d << '_'; count++; // выдать тег, счёт узлов
            v = v->mdl;
        }
    }                                       
    return count;
    /*
    const int MaxS = 20; // максимальный размер стека
    int count = 0;
    STACK <Node *> S(MaxS); //создание стека указателей на узлы
    S.push(root); // STACK <- root
    while (!S.empty( )) // Пока стек не пуст…
    {
    Node * v = S.pop( ); // поднять узел из стека
    cout << v->d << '_'; count++; // выдать тег, счёт узлов
    if (v->rgt) S.push(v->rgt); // STACK <- (правый сын)
    if (v->mdl) S.push(v->mdl); // STACK <- (средний сын)
    if (v->lft) S.push(v->lft); // STACK <- (левый сын)
    }
    return count;
    */
}

int Tree :: BFS( )
{
    const int MaxQ = 20; //максимальный размер очереди
    int count = 0;
    QUEUE <Node *> Q(MaxQ); //создание очереди указателей на узлы
    Q.push(root); // QUEUE <- root поместить в очередь корень дерева
    while (!Q.empty( )) //пока очередь не пуста
    { Node * v = Q.pop( );// взять из очереди,
    cout << v->d << '_'; count++; // выдать тег, счёт узлов
    if (v->lft) Q.push(v->lft); // QUEUE <- (левый сын)
    if (v->mdl) Q.push(v->mdl); 
    if (v->rgt) Q.push(v->rgt); // QUEUE <- (правый сын)
    }
    return count;
}

int main()
{   
    int n = 0;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Tree Tr('a', 'z', 8);
    srand(time(nullptr));
    setlocale(LC_ALL, "Russian");
    Tr.MakeTree( );
    if(Tr.exist( )) {
    Tr.OutTree( );
    cout << '\n' << "Обход в глубину: ";
    Tr.InOrder(Tr.root);
    n = Tr.GetMaxDepth(Tr.root->lft);
    //n = Tr.DFS();
    cout << " Максимальная глубина = " << n;
    }
    else cout << "Дерево пусто!";
    cout << '\n' << "=== Конец ==="; cin.get( );
}