#include <iostream>
#include <vector>

class IGraph {
public:
    IGraph() = default;;

    [[maybe_unused]] explicit IGraph([[maybe_unused]] IGraph *_oth) {};
    virtual void AddEdge(int from, int to) = 0;
    [[nodiscard]] virtual int VerticesCount() const = 0;
    virtual void GetNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual void GetPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
    virtual void show () const = 0;
    [[nodiscard]] virtual int get (int indI, int indJ) const = 0;
    virtual ~IGraph() = default;
};

class ListGraph : public IGraph {
private:
    std::vector<std::vector<int>> vec;
public:
    ListGraph()= default;
    explicit ListGraph(IGraph *ptr) {
        int count = ptr->VerticesCount();
        vec.resize(count);
        for (int i = 0;i < vec.size();++i) {
            for (int j = 0;j < vec[i].size();++j) {
                vec[i].push_back(ptr->get(i, j));
            }
        }
    }
    ListGraph(IGraph *ptr, int) {
        int count = ptr->VerticesCount();
        vec.resize(count);
        for (int i = 0;i < vec.size();++i) {
            for (int j = 0;j < vec.size();++j) {
                if (ptr->get(i, j) == 1) {
                    vec[i].push_back(j);
                }
            }
        }
    }
    explicit ListGraph(int count) {
        vec.resize(count);
    }
    void AddEdge (int from, int to) override {
        vec[from].push_back(to);
    }
    [[nodiscard]] int VerticesCount() const override {
        return static_cast<int>(vec.size());
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override {
        if (vec[vertex].empty()) {
            std::cout << "There are no adjacent vertices for this vertex" << std::endl;
            return;
        }
        int buff = vertex;
        for (int i = 0;;++i) {
            for (int j : vec[vertex]) {
                for (int k = 0;;++k) {
                    if (vertices.empty()) {
                        vertices.push_back(j);
                        break;
                    } else if (j == vertices[k] || j == buff) {
                        break;
                    } else if (k == vertices.size() - 1) {
                        vertices.push_back(j);
                        break;
                    }
                }
            }
            if (i != vertices.size()) {
                vertex = vertices[i];
            } else {
                break;
            }

        }
    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override {
        for (int i = 0;i < vec.size();++i) { // находим все смежные вершины с vertex и добавляем
            for (int j = 0;j < vec[i].size();++j) {
                if (vec[i][j] == vertex) {
                    vertices.push_back(i);
                    break;
                }
            }
        }

        for (int i = 0;i < vertices.size();++i) { // находим остальные вершины
            for (int j = 0;j < vec.size();++j) {
                if (vertices[i] == j || j == vertex) { // отсекаем повторяющиеся вершины
                    continue;
                }
                for (int k = 0;k < vec[j].size();++k) { // находим все смежные вершины с вершинами из vertices
                    if (vec[j][k] == vertices[i]) { // если вершина j смежная с вершиной vertices[i] идем дальше
                        for (int l = 0;;++l) { // проверяем есть ли  уже такая вершина в vertices
                            if(vertices[l] == j) {
                                break;
                            } else if (l == vertices.size() - 1) {
                                vertices.push_back(j);
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    void show () const override {
        for (int i = 0; i < vec.size();++i) {
            std::cout << i << ": ";
            for (int j : vec[i]) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
    [[nodiscard]] int get (int indI, int indJ) const override {
        if (indI >= vec.size() || indJ >= vec[indI].size()) {
            return -1;
        }
        return vec[indI][indJ];
    }
};

class MatrixGraph : public IGraph {
private:
    std::vector<std::vector<int>> vec;
public:
    MatrixGraph() = default;
    explicit MatrixGraph(IGraph *ptr) {
        vec.resize(ptr->VerticesCount());
        for (int i = 0;i < vec.size();++i) {
            vec[i].resize(ptr->VerticesCount());
            for (int j = 0;j < vec[i].size();++j) {
                vec[i].resize(ptr->VerticesCount());
                vec[i][j] = ptr->get(i, j);
            }
        }
    }
    MatrixGraph(IGraph *ptr, int) {
        vec.resize(ptr->VerticesCount());
        for (int i = 0;i < vec.size();++i) {
            vec[i].resize(ptr->VerticesCount());
            for (int j = 0;j < vec[i].size();++j) {
                for (int k = 0;k < vec[i].size();++k) {
                    if (ptr->get(i, j) == -1) {
                        break;
                    } else if (k == ptr->get(i, j)) {
                        vec[i][k] = 1;
                        ++j;
                    } else {
                        vec[i][k] = 0;
                    }
                }
            }
        }
    }
    explicit MatrixGraph(int count) {
        vec.resize(count);
        for (auto & i : vec) {
            i.resize(count);
        }
    }
    void AddEdge (int from, int to) override {
        vec[from][to] = 1;
    }
    [[nodiscard]] int VerticesCount() const override {
        return static_cast<int>(vec.size());
    }
    void GetNextVertices(int vertex, std::vector<int> &vertices) const override {
        int buff = vertex;
        for (int i = 0;;++i) {
            for (int j = 0;j < vec.size();++j) {
                if (vec[vertex][j] == 1) {
                    for (int k = 0;;++k) {
                        if (vertices.empty()) {
                            vertices.push_back(j);
                            break;
                        } else {
                            if (vertices[k] == j || j == buff) {
                                break;
                            } else if (k == vertices.size() - 1) {
                                vertices.push_back(j);
                                break;
                            }
                        }
                    }
                }
            }
            if (vertices.empty()) {
                std::cout << "There are no adjacent vertices for this vertex" << std::endl;
                return;
            } else if (i == vertices.size()) {
                return;
            }
            vertex = vertices[i];
        }

    }
    void GetPrevVertices(int vertex, std::vector<int> &vertices) const override {
        int buff = vertex;
        for (int i = 0;;++i) {
            for (int j = 0;j < vec.size();++j) {
                if (vec[j][vertex] == 1) {
                    for (int k = 0;;++k) {
                        if (vertices.empty()) {
                            vertices.push_back(j);
                            break;
                        } else {
                            if (vertices[k] == j || j == buff) {
                                break;
                            } else if (k == vertices.size() - 1) {
                                vertices.push_back(j);
                                break;
                            }
                        }

                    }

                }
            }
            if (vertices.empty()) {
                std::cout << "There are no adjacent vertices for this vertex" << std::endl;
                return;
            } else if (i == vertices.size()) {
                return;
            }
            vertex = vertices[i];
        }
    }
    void show () const override {
        for (int i = 0; i < vec.size();++i) {
            std::cout << i << ": ";
            for (int j : vec[i]) {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }
    [[nodiscard]] int get (int indI, int indJ) const override {
        return vec[indI][indJ];
    }
};

int personInteraction () {
    int n;
    std::cout << "Enter the number of vertices of the list graph:";
    std::cin >> n;
    return n;
}
void graphInit (IGraph *graph) {
    while (true) {
        int n,m;
        std::cout << "Enter the starting vertex or enter -1 to exit: ";
        std::cin >> n;
        if (n >= graph->VerticesCount() || n < 0 && n != -1) {
            std::cout << "Invalid value.Try again" << std::endl;
            continue;
        } else if (n == -1) break;
        std::cout << "Enter the final vertex of the graph: ";
        std::cin >> m;
        if (m >= graph->VerticesCount() || m < 0) {
            std::cout << "Invalid value.Try again" << std::endl;
            continue;
        }
        graph->AddEdge(n,m);
    }
}
void showNextVert (IGraph *graph) {
    while (true) {
        int vertex;
        std::vector<int> vertices;
        std::cout << "Enter the vertex for which you want to find the vertices that can be reached by the edge or enter -1 to exit: ";
        std::cin >> vertex;
        if (vertex < 0 && vertex != -1 || vertex >= graph->VerticesCount()) {
            std::cout << "Invalid value. Try again" << std::endl;
            continue;
        } else if (vertex == -1) {
            break;
        }
        graph->GetNextVertices(vertex, vertices);
        for (int vertice : vertices) {
            std::cout << vertice << " ";
        }
        std::cout << std::endl;
    }
}
void showPrevVert (IGraph *graph) {
    while (true) {
        int vertex;
        std::vector<int> vertices;
        std::cout << "Enter the vertex for which you want to find the vertices from which you can get to this or enter -1 to exit: ";
        std::cin >> vertex;
        if (vertex < 0 && vertex != -1 || vertex >= graph->VerticesCount()) {
            std::cout << "Invalid value. Try again" << std::endl;
            continue;
        } else if (vertex == -1) {
            break;
        }
        graph->GetPrevVertices(vertex, vertices);
        for (int vertice : vertices) {
            std::cout << vertice << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    IGraph *LGraph1 = new ListGraph (personInteraction());
    graphInit (LGraph1);
    std::cout << "Adjacency list of the LGraph:" << std::endl;
    LGraph1->show();
    showNextVert(LGraph1);
    showPrevVert(LGraph1);

    IGraph *MGraph1 = new MatrixGraph(personInteraction());
    graphInit (MGraph1);
    std::cout << "Adjacency matrix of the MGraph:" << std::endl;
    MGraph1->show();
    showNextVert(MGraph1);
    showPrevVert(MGraph1);

    IGraph *LGraph2 = new ListGraph(LGraph1);
    LGraph2->show();
    showNextVert(LGraph2);
    showPrevVert(LGraph2);
    std::cout << std::endl;
    IGraph *MGraph2 = new MatrixGraph(MGraph1);
    MGraph2->show();
    showNextVert(MGraph2);
    showPrevVert(MGraph2);
    std::cout << std::endl;

    IGraph *LGraph3 = new ListGraph(MGraph1, 0);
    LGraph3->show();
    showNextVert(LGraph3);
    showPrevVert(LGraph3);
    std::cout << std::endl;
    IGraph *MGraph3 = new MatrixGraph(LGraph1, 0);
    MGraph3->show();
    showNextVert(MGraph3);
    showPrevVert(MGraph3);
    delete MGraph3;
    delete LGraph3;
    delete MGraph2;
    delete LGraph2;
    delete MGraph1;
    delete LGraph1;
    return 0;
}