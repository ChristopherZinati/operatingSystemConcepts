#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <string>

using namespace std;

class Process {
public:
    int pid;
    int arr_time;
    int burst_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;

    Process(int id, int arr, int burst) : pid(id), arr_time(arr), 
        completion_time(0), waiting_time(0), turnaround_time(0){}
};

class SchedulingAlgorithms{
public:
    //First Come First Serve
    void fcfs (vector<Process>& processes){
        sort(processes.begin(), processes.end(), [](Process& a, Process& b) { 
            return a.arr_time < b.arr_time;
            });
        int curr_time = 0;
        for (auto& process : processes){
            if (curr_time < process.arr_time){
                curr_time = process.arr_time;
            }
            process.completion_time = curr_time + process.burst_time;
            process.turnaround_time = process.completion_time-process.arr_time;
            process.waiting_time = process.turnaround_time-process.burst_time;
            curr_time += process.burst_time;          
        }
    }
    //Shortest Job Next
    void sjn (vector<Process>& processes){
        sort(processes.begin(), processes.end(), [](Process& a, Process& b) { 
            return a.arr_time < b.arr_time;
            });
        int curr_time = 0;
        vector<Process> ready;

        while(!processes.empty() || !ready.empty()){
            while(!processes.empty() && processes.front().arr_time < curr_time){
                ready.push_back(processes.front());
                processes.erase(processes.begin());
            }
            if (!ready.empty()){
                sort(ready.begin(), ready.end(), [](Process& a, Process& b) {
                    return a.burst_time < b.burst_time;
                });
                Process process = ready.front();
                ready.erase(ready.begin());

                process.completion_time = curr_time + process.burst_time;
                process.turnaround_time = process.completion_time - process.arr_time;
                process.waiting_time = process.turnaround_time - process.burst_time;
                curr_time += process.burst_time;
            }
            else {
               curr_time = processes.front().arr_time; 
            }
        }
    }
    //Round Robin 
    void roundr (vector<Process>& processes, int time_quant){
        queue<Process> ready;
        int curr_time = 0;
        vector<Process> remaining = processes;
        while(!remaining.empty() || !ready.empty()){
            while(!remaining.empty() && remaining.front().arr_time <= curr_time){
                ready.push(remaining.front());
                remaining.erase(remaining.begin());
            }
            if(!ready.empty()){
                Process process = ready.front();
                ready.pop();
                if(process.burst_time > time_quant){
                    curr_time += time_quant;
                    process.burst_time -= time_quant;
                    while(!remaining.empty() && remaining.front().arr_time <= curr_time){
                        ready.push(remaining.front());
                        remaining.erase(remaining.begin());
                    }
                    ready.push(process);
                } else {
                    curr_time += process.burst_time;
                    process.completion_time = curr_time;
                    process.turnaround_time = process.turnaround_time - process.arr_time;
                    process.waiting_time = process.turnaround_time - process.burst_time;
                }
            } else {
                curr_time = remaining.front().arr_time;
            }
        }
    }
    void gantt_chart(const vector<Process>& processes) {
        cout << "Gantt Chart:\n";
        for (const auto& process : processes) {
            cout << "| P" << process.pid << " ";
        }
        cout << "|\n";

        int current_time = 0;
        for (const auto& process : processes) {
            cout << current_time << setw(4 + to_string(process.pid).length()) << "";
            current_time += process.burst_time;
        }
        cout << current_time << "\n";
    }
};
int main() {
    int choice;
    cout << "Choose an option:\n";
    cout << "1. Enter custom processes\n";
    cout << "2. Use example processes\n";
    cin >> choice;

    vector<Process> processes;

    if (choice == 1) {
        int num_processes;
        cout << "Enter the number of processes: ";
        cin >> num_processes;

        for (int i = 0; i < num_processes; ++i) {
            int pid, arrival_time, burst_time;
            cout << "Enter Process ID, Arrival Time, and Burst Time for process " << (i + 1) << ": ";
            cin >> pid >> arrival_time >> burst_time;
            processes.emplace_back(pid, arrival_time, burst_time);
        }
    } else {
        processes = {
            Process(1, 0, 8),
            Process(2, 1, 4),
            Process(3, 2, 9),
            Process(4, 3, 5)
        };
    }

    int time_quantum;
    cout << "Enter time quantum for Round Robin scheduling: ";
    cin >> time_quantum;

    SchedulingAlgorithms scheduler;

    vector<Process> fcfs_processes = processes;
    scheduler.fcfs(fcfs_processes);
    cout << "\nFCFS Scheduling:\n";
    for (const auto& process : fcfs_processes) {
        cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.gantt_chart(fcfs_processes);


    vector<Process> sjn_processes = processes;
    scheduler.sjn(sjn_processes);
    cout << "\nSJN Scheduling:\n";
    for (const auto& process : sjn_processes) {
        cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.gantt_chart(sjn_processes);


    vector<Process> rr_processes = processes;
    scheduler.roundr(rr_processes, time_quantum);
    cout << "\nRR Scheduling:\n";
    for (const auto& process : rr_processes) {
        cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.gantt_chart(rr_processes);

    return 0;
}
