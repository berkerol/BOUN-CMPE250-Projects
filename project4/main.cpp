#include <fstream>
#include <set>

using namespace std;

struct Edge {
    long long first, second, weight;

    Edge(const long long &first, const long long &second, const long long &weight) {
        this->first = first;
        this->second = second;
        this->weight = weight;
    }
};

struct compare_edges {
    bool operator()(const Edge &edge1, const Edge &edge2) {
        return edge1.weight > edge2.weight;
    }
};

struct DS {
    long long root;
    bool marked;

    DS() {
        this->root = -1;
        this->marked = 0;
    }
};

long long find(long long index, DS *sets) {
    long long root = sets[index].root;
    while (root >= 0) {
        index = root;
        root = sets[root].root;
    }
    return index;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Run the code with the following command: ./project4 [input_file] [output_file]");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    long long total_vertices, total_cuts, sum = 0;
    fscanf(file, "%lld %lld", &total_vertices, &total_cuts);
    multiset<Edge, compare_edges> edges;
    for (long long i = 0; i < total_vertices - 1; ++i) {
        long long first, second, weight;
        fscanf(file, "%lld %lld %lld", &first, &second, &weight);
        edges.insert(Edge(first, second, weight));
    }
    DS sets[total_vertices];
    for (long long i = 0; i < total_cuts; ++i) {
        long long cut;
        fscanf(file, "%lld", &cut);
        sets[cut].marked = 1;
    }
    fclose(file);
    for (set<Edge, compare_edges>::iterator it = edges.begin(); it != edges.end(); it++) {
        long long first = find((*it).first, sets), second = find((*it).second, sets);
        if (sets[first].root < sets[second].root) {
            if (sets[second].marked) {
                if (sets[first].marked) {
                    sum += (*it).weight;
                }
                sets[first].marked = 1;
            }
            sets[first].root--;
            sets[second].root = first;
        } else {
            if (sets[first].marked) {
                if (sets[second].marked) {
                    sum += (*it).weight;
                }
                sets[second].marked = 1;
            }
            sets[second].root--;
            sets[first].root = second;
        }
    }
    file = fopen(argv[2], "w");
    fprintf(file, "%lld", sum);
    fclose(file);
    return 0;
}