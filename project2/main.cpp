#include <fstream>
#include <queue>
#include <set>

using namespace std;

struct CPU {
    int id;
    bool state;
    double frequency, active_time;

    CPU(const int &id, const bool &state, const double &frequency, const double &active_time) {
        this->id = id;
        this->state = state;
        this->frequency = frequency;
        this->active_time = active_time;
    }
};

struct OD {
    int id;
    bool state;
    double quantum, active_time;

    OD(const int &id, const bool &state, const double &quantum, const double &active_time) {
        this->id = id;
        this->state = state;
        this->quantum = quantum;
        this->active_time = active_time;
    }
};

struct Event {
    int type, device_id;
    double cpu_work, od_work, arrival_time, wait_time, departure_time;

    Event(const int &type, const int &device_id, const double &cpu_work, const double &od_work,
          const double &arrival_time, const double &wait_time, const double &departure_time) {
        this->type = type;
        this->device_id = device_id;
        this->cpu_work = cpu_work;
        this->od_work = od_work;
        this->arrival_time = arrival_time;
        this->wait_time = wait_time;
        this->departure_time = departure_time;
    }
};

struct compare_devices {
    template<typename T>
    bool operator()(const T &t1, const T &t2) {
        if (t1.state != t2.state) {
            return t1.state < t2.state;
        }
        return t1.id < t2.id;
    }
};

struct compare_events {
    bool operator()(const Event &e1, const Event &e2) {
        if (e1.departure_time != e2.departure_time) {
            return e1.departure_time >= e2.departure_time;
        } else if (e1.type != e2.type) {
            return e1.type <= e2.type;
        }
        return e1.device_id >= e2.device_id;
    }
};

struct compare_works {
    bool operator()(const Event &e1, const Event &e2) {
        return e1.cpu_work > e2.cpu_work;
    }
};

set<CPU, compare_devices> cpus;
set<OD, compare_devices> ods;
priority_queue<Event, vector<Event>, compare_events> events;
priority_queue<Event, vector<Event>, compare_works> first_pq;
queue<Event> second_pq;
queue<Event> final_pq;

CPU find_cpu(int device_id) {
    for (set<CPU, compare_devices>::iterator it = cpus.begin(); it != cpus.end(); it++) {
        if ((*it).id == device_id) {
            CPU c = *it;
            it = cpus.erase(it);
            return c;
        }
    }
    return CPU(0, 0, 0.0, 0.0);
}

OD find_od(int device_id) {
    for (set<OD, compare_devices>::iterator it = ods.begin(); it != ods.end(); it++) {
        if ((*it).id == device_id) {
            OD od = *it;
            it = ods.erase(it);
            return od;
        }
    }
    return OD(0, 0, 0.0, 0.0);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Run the code with the following command: ./project2 [input_file] [output_file]");
        return 1;
    }
    int total_cpus, total_ods, total_tasks, first_pq_max_length = 0, second_pq_max_length = 0;
    int greatest_active_CPU_id = 0, greatest_active_od_id = 0;
    double greatest_active_CPU_time = 0.0, greatest_active_od_time = 0.0;
    double wait_time_sum = 0.0, wait_time_longest = 0.0, total_time_sum = 0.0;
    FILE *file = fopen(argv[1], "r");
    fscanf(file, "%d", &total_cpus);
    for (int i = 1; i <= total_cpus; ++i) {
        double frequency;
        fscanf(file, "%lf", &frequency);
        cpus.insert(CPU(i, 0, frequency, 0.0));
    }
    fscanf(file, "%d", &total_ods);
    for (int i = 1; i <= total_ods; ++i) {
        double quantum;
        fscanf(file, "%lf", &quantum);
        ods.insert(OD(i, 0, quantum, 0.0));
    }
    fscanf(file, "%d", &total_tasks);
    for (int i = 1; i <= total_tasks; ++i) {
        double arrival_time, cpu_work, od_work;
        fscanf(file, "%lf %lf %lf", &arrival_time, &cpu_work, &od_work);
        events.push(Event(0, 0, cpu_work, od_work, arrival_time, 0.0, arrival_time));
    }
    fclose(file);
    while (!events.empty()) {
        Event e = events.top();
        events.pop();
        double time = e.departure_time;
        switch (e.type) {
            case 0: {
                if (!(*cpus.begin()).state) {
                    CPU c = *cpus.begin();
                    cpus.erase(cpus.begin());
                    double process_time = e.cpu_work / c.frequency;
                    c.state = 1;
                    c.active_time += process_time;
                    e.type = 1;
                    e.device_id = c.id;
                    e.departure_time += process_time;
                    cpus.insert(c);
                    events.push(e);
                } else {
                    first_pq.push(e);
                    if (first_pq.size() > first_pq_max_length) {
                        first_pq_max_length = first_pq.size();
                    }
                }
                break;
            }
            case 1: {
                CPU c = find_cpu(e.device_id);
                if (!(*ods.begin()).state) {
                    OD od = *ods.begin();
                    ods.erase(ods.begin());
                    od.state = 1;
                    if (od.quantum > e.od_work) {
                        od.active_time += e.od_work;
                        e.departure_time += e.od_work;
                        e.od_work = 0.0;
                    } else {
                        od.active_time += od.quantum;
                        e.departure_time += od.quantum;
                        e.od_work = e.od_work - od.quantum;
                    }
                    e.type = 2;
                    e.device_id = od.id;
                    ods.insert(od);
                    events.push(e);
                } else {
                    second_pq.push(e);
                    if (second_pq.size() > second_pq_max_length) {
                        second_pq_max_length = second_pq.size();
                    }
                }
                if (first_pq.empty()) {
                    c.state = 0;
                } else {
                    Event e2 = first_pq.top();
                    first_pq.pop();
                    double process_time = e2.cpu_work / c.frequency;
                    c.active_time += process_time;
                    e2.type = 1;
                    e2.device_id = c.id;
                    e2.wait_time += time - e2.departure_time;
                    e2.departure_time = time + process_time;
                    events.push(e2);
                }
                cpus.insert(c);
                break;
            }
            case 2: {
                OD od = find_od(e.device_id);
                if (e.od_work == 0) {
                    od.state = 0;
                    ods.insert(od);
                    final_pq.push(e);
                } else {
                    if (second_pq.empty()) {
                        if (od.quantum > e.od_work) {
                            od.active_time += e.od_work;
                            e.departure_time += e.od_work;
                            e.od_work = 0.0;
                        } else {
                            od.active_time += od.quantum;
                            e.departure_time += od.quantum;
                            e.od_work = e.od_work - od.quantum;
                        }
                        ods.insert(od);
                        events.push(e);
                    } else {
                        od.state = 0;
                        ods.insert(od);
                        second_pq.push(e);
                    }
                }
                if (!second_pq.empty()) {
                    Event e2 = second_pq.front();
                    second_pq.pop();
                    od = *ods.begin();
                    ods.erase(ods.begin());
                    od.state = 1;
                    if (od.quantum > e2.od_work) {
                        od.active_time += e2.od_work;
                        e2.wait_time += time - e2.departure_time;
                        e2.departure_time = time + e2.od_work;
                        e2.od_work = 0.0;
                    } else {
                        od.active_time += od.quantum;
                        e2.wait_time += time - e2.departure_time;
                        e2.departure_time = time + od.quantum;
                        e2.od_work = e2.od_work - od.quantum;
                    }
                    e2.type = 2;
                    e2.device_id = od.id;
                    ods.insert(od);
                    events.push(e2);
                }
                break;
            }
            default: {
            }
        }
    }
    file = fopen(argv[2], "w");
    fprintf(file, "%d\n", first_pq_max_length);
    fprintf(file, "%d\n", second_pq_max_length);
    for (set<CPU, compare_devices>::iterator it = cpus.begin(); it != cpus.end(); it++) {
        CPU c = *it;
        if (c.active_time > greatest_active_CPU_time) {
            greatest_active_CPU_time = c.active_time;
            greatest_active_CPU_id = c.id;
        }
    }
    fprintf(file, "%d\n", greatest_active_CPU_id);
    for (set<OD, compare_devices>::iterator it = ods.begin(); it != ods.end(); it++) {
        OD od = *it;
        if (od.active_time > greatest_active_od_time) {
            greatest_active_od_time = od.active_time;
            greatest_active_od_id = od.id;
        }
    }
    fprintf(file, "%d\n", greatest_active_od_id);
    while (!final_pq.empty()) {
        Event e = final_pq.front();
        final_pq.pop();
        wait_time_sum += e.wait_time;
        if (e.wait_time > wait_time_longest) {
            wait_time_longest = e.wait_time;
        }
        total_time_sum += e.departure_time - e.arrival_time;
    }
    fprintf(file, "%lf\n", wait_time_sum / total_tasks);
    fprintf(file, "%lf\n", wait_time_longest);
    fprintf(file, "%lf\n", total_time_sum / total_tasks);
    fclose(file);
    return 0;
}