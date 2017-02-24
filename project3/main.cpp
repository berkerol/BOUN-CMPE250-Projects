#include <fstream>
#include <vector>

using namespace std;

struct Vertex {
    double process_time, other_time;
    int in_degree;
    bool erased;
    vector<int> adj;

    Vertex(const double &process_time) {
        this->process_time = process_time;
        this->other_time = 0.0;
        this->in_degree = 0;
        this->erased = 0;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Run the code with the following command: ./project3 [input_file] [output_file]");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    unsigned int total_vertices, total_edges;
    fscanf(file, "%d %d", &total_vertices, &total_edges);
    vector<Vertex> vertices;
    for (int i = 0; i < total_vertices; ++i) {
        double time;
        fscanf(file, "%lf", &time);
        Vertex v = Vertex(time);
        vertices.push_back(v);
    }
    for (int i = 0; i < total_edges; ++i) {
        int from, to;
        fscanf(file, "%d %d", &from, &to);
        vertices[from].adj.push_back(to);
        vertices[to].in_degree++;
    }
    fclose(file);
    double result = -1.0;
    bool changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < total_vertices; i++) {
            if (!vertices[i].erased && vertices[i].in_degree == 0) {
                if (vertices[i].adj.empty()) {
                    if (vertices[i].process_time > result) {
                        result = vertices[i].process_time;
                    }
                } else {
                    for (auto &j : vertices[i].adj) {
                        if (vertices[j].in_degree == 1) {
                            if (vertices[i].process_time > vertices[j].other_time) {
                                vertices[j].process_time += vertices[i].process_time;
                            } else {
                                vertices[j].process_time += vertices[j].other_time;
                            }
                            vertices[j].other_time = 0.0;
                        } else {
                            if (vertices[i].process_time > vertices[j].other_time) {
                                vertices[j].other_time = vertices[i].process_time;
                            }
                        }
                        vertices[j].in_degree--;
                    }
                }
                changed = 1;
                vertices[i].erased = 1;
            }
        }
    }
    file = fopen(argv[2], "w");
    fprintf(file, "%lf", result);
    fclose(file);
    return 0;
}