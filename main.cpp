#include <iostream>
#include <list>
#include <vector>
#include <ostream>
#include <fstream>
#include <unordered_set>
#include <queue>
using namespace std;

template <class V, class E>
class Vertex;

template <class V, class E>
class Edge
{
public:
    E m_Weigth;              // Peso de la arista
    Vertex<V, E> *m_pVertex; // Vertice
public:
    Edge(E d, Vertex<V, E> *p = 0)
    {
        m_Weigth = d;
        m_pVertex = p;
    }
    Edge(){};
};

template <class V, class E>
class Vertex
{
public:
    V m_Dato;                   // Nombre Vertice
    list<Edge<V, E>> m_Aristas; // Lista de Aristas que salen
public:
    Vertex(V d)
    {
        m_Dato = d;
        m_Aristas.clear();
    }
    Vertex(){};
};

template <class V, class E>
class Grafo
{
public:
    list<Vertex<V, E>> m_grafo; // Lista de Vetices
public:
    Grafo(){};
    Vertex<V, E> *find_Vertex(V v)
    {
        Vertex<V, E> *p = new Vertex<V, E>(v);
        if (m_grafo.empty())
            return 0;
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if ((*it).m_Dato == p->m_Dato)
                return &(*it);
        }
        return p;
    }

    void Insert_Vertex(V dato)
    {
        m_grafo.push_back(Vertex<V, E>(dato));
    }

    void Insert_Arista(V v1, V v2, E Ar)
    {
        Vertex<V, E> *p = find_Vertex(v1);
        Vertex<V, E> *q = find_Vertex(v2);
        if (p && q)
        {
            if (p == q)
            {
                cout << "No vale mano\n";
                return;
            }
            p->m_Aristas.push_back(Edge<V, E>(Ar, q));
        }
    }

    int index_vert(V nombre)
    {
        int index = 0;
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if (nombre == (*it).m_Dato)
            {
                return index;
            }
            index++;
        }
        return 0;
    }

    vector<vector<int>> obtenerMatriz()
    {
        int size = m_grafo.size();
        int fil = 0;
        int col, peso;
        vector<vector<int>> matrix(size, vector<int>(size, 0));
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            typename ::list<Edge<V, E>>::iterator it_2 = (*it).m_Aristas.begin();
            for (; it_2 != (*it).m_Aristas.end(); it_2++)
            {
                col = index_vert((*it_2).m_pVertex->m_Dato);
                peso = (*it_2).m_Weigth;
                matrix[fil][col] = peso;
            }
            fil++;
        }
        return matrix;
    }

    void verAdyacencias()
    {
        for (auto a : m_grafo)
        {
            cout << "[" << a.m_Dato << "]";
            for (auto ver : a.m_Aristas)
            {
                cout << "->[" << ver.m_pVertex->m_Dato << "]";
            }
            cout<<endl;
        }
    }

    void busqueda_Anchura(string file)
    {
        ofstream archivo;
        vector<V> vertex_join;
        archivo.open(file.c_str(), ios::out);
        archivo << "digraph {\n";
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if ((*it).m_Aristas.empty())
            {
                archivo << (*it).m_Dato << "\n";
                vertex_join.push_back((*it).m_Dato);
            }
            else
            {
                typename ::list<Edge<V, E>>::iterator it_2 = (*it).m_Aristas.begin();
                for (; it_2 != (*it).m_Aristas.end(); it_2++)
                {
                    if(vertex_join.size() == m_grafo.size()){
                        break;
                    }
                    bool encontrado = false;
                    for(int i = 0; i <vertex_join.size(); i++){
                        if(vertex_join[i] == (*it_2).m_pVertex->m_Dato){
                            encontrado = true;
                            break;
                        }
                    }
                    if (encontrado){
                        archivo << (*it).m_Dato << "\n";
                    }else{
                        archivo << (*it).m_Dato << "->" << (*it_2).m_pVertex->m_Dato << "[ label=\"" << (*it_2).m_Weigth << "\" ];\n";
                        vertex_join.push_back((*it).m_Dato);
                    }                    
                }
            }
        }
        archivo << "}\n";
        archivo.close();
    }

    void profundidad(V vertice){
        unordered_set<V> visitados;
        profundidad(vertice,visitados);
    }

    void profundidad(V vertice, unordered_set<V> &visitados) {
        Vertex<V, E> * v_actual = find_Vertex(vertice);
        cout << "Visitando " << vertice << '\n';
        if(!v_actual) return;
        visitados.insert(vertice);
        for(auto it = (*v_actual).m_Aristas.begin(); it != (*v_actual).m_Aristas.end(); it++) {
            if(visitados.find(((*it).m_pVertex) -> m_Dato) == visitados.end()) {
                profundidad(((*it).m_pVertex) -> m_Dato, visitados);
            }
        }
	}

    void showDot(string file)
    {
        ofstream archivo;
        archivo.open(file.c_str(), ios::out);
        archivo << "digraph {\n";
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if ((*it).m_Aristas.empty())
            {
                archivo << (*it).m_Dato << "\n";
            }
            typename ::list<Edge<V, E>>::iterator it_2 = (*it).m_Aristas.begin();
            for (; it_2 != (*it).m_Aristas.end(); it_2++)
            {
                archivo << (*it).m_Dato << "->" << (*it_2).m_pVertex->m_Dato << "[ label=\"" << (*it_2).m_Weigth << "\" ];\n";
            }
        }
        archivo << "}\n";
        archivo.close();
    }

    bool esVacio(){
        int index = 0;
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            index++;
        }
        if(index > 0){
            return false;
        }
        else{
            return true;
        }
    }

    bool existeNodo(V v){
        Vertex<V, E> *p = new Vertex<V, E>(v);
        if (m_grafo.empty())
            return false;
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if ((*it).m_Dato == p->m_Dato)
                return true;
        }
        return false;
    }

    bool sonAdyacentes(V dato1,V dato2){
        Vertex<V, E> *p = find_Vertex(dato1);
        Vertex<V, E> *q = find_Vertex(dato2);
        typename ::list<Vertex<V, E>>::iterator it = m_grafo.begin();
        for (; it != m_grafo.end(); it++)
        {
            if ((*it).m_Aristas.empty() && ((*it).m_Dato == dato1 || (*it).m_Dato == dato2))
            {
                return false;
            }
            typename ::list<Edge<V, E>>::iterator it_2 = (*it).m_Aristas.begin();
            for (; it_2 != (*it).m_Aristas.end(); it_2++)
            {
                if((*it).m_Dato == dato1 && (*it_2).m_pVertex->m_Dato == dato2){
                    return true;
                }
            }
        }
        return false;
    }

    void solve(V vertice){
        queue<Vertex<V,E>* > vertices;
        unordered_set< V* > visitados;
        Vertex<V,E>* ver = find_Vertex(vertice);
        vertices.push( ver );
        while( !vertices.empty() ){
            for( auto it2=ver->m_Aristas.begin() ; it2!=ver->m_Aristas.end(); ++it2 ){
                if(visitados.find( &(( it2->m_pVertex)->m_Dato) ) == visitados.end() ){
                    if( it2->m_Weigth == 't' ){
                        cout << "YES\n";
                        return;
                    }
                    vertices.push(it2->m_pVertex);
                }
            }
            vertices.pop();
            visitados.insert(&(ver->m_Dato));
            if(!vertices.empty())
                ver = vertices.front();
        }
        cout<< "NO\n";
    }

};

int main(){
    int f,c; cin >> f >> c;
    vector<string> tablero(f);
    for(int i=0;i<f;i++){
        cin >> tablero[i];
    }
    Grafo<pair<int,int>,char> red;
    for(int i=0; i < f; ++i){
        for(int j = 0; j < c; ++j){
            if( tablero[i][j] == 'X') continue;
            else {
                red.Insert_Vertex(make_pair(i,j));
                if(i) red.Insert_Arista( make_pair(i,j), make_pair(i-1,j), tablero[i][j]);
                if(j) red.Insert_Arista( make_pair(i,j), make_pair(i,j-1), tablero[i][j]);
            }            
        }
    }
    
    int x,y; cin >> x >> y;
    pair<int,int> query = make_pair(x-1,y-1);
    red.solve(query);
    return 1;
}
