#include <fstream>
#include <vector>
#include <set>

using namespace std;

struct compare_edges {
    bool operator()(const pair<long long, long long> &p1, const pair<long long, long long> &p2) {
        if (p1.second != p2.second) {
            return p1.second < p2.second;
        }
        return p1.first < p2.first;
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Run the code with the following command: ./project5 [input_file] [output_file]");
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    long long total_cities, total_roads, source, destination;
    fscanf(file, "%lld %lld", &total_cities, &total_roads);
    long long heuristics[total_cities];
    for (long long i = 0; i < total_cities; ++i) {
        fscanf(file, "%lld", &heuristics[i]);
    }
    vector<pair<long long, long long> > cities[total_cities];
    for (long long i = 0; i < total_roads; ++i) {
        long long first, second, length;
        fscanf(file, "%lld %lld %lld", &first, &second, &length);
        cities[first].push_back(make_pair(second, length));
        cities[second].push_back(make_pair(first, length));
    }
    fscanf(file, "%lld %lld", &source, &destination);
    fclose(file);
    long long costs[total_cities];
    for (long long i = 0; i < total_cities; ++i) {
        costs[i] = 0;
    }
    set<pair<long long, long long>, compare_edges> frontier;
    frontier.insert(make_pair(source, heuristics[source]));
    while (!frontier.empty()) {
        long long location = frontier.begin()->first;
        frontier.erase(frontier.begin());
        if (location == destination) {
            break;
        }
        for (auto edge : cities[location]) {
            long long new_cost = costs[location] + edge.second;
            if (costs[edge.first] == 0 || new_cost < costs[edge.first]) {
                if (new_cost < costs[edge.first]) {
                    frontier.erase(make_pair(edge.first, costs[edge.first] + heuristics[edge.first]));
                }
                costs[edge.first] = new_cost;
                frontier.insert(make_pair(edge.first, costs[edge.first] + heuristics[edge.first]));
            }
        }
    }
    file = fopen(argv[2], "w");
    fprintf(file, "%lld", costs[destination]);
    fclose(file);
    return 0;
}