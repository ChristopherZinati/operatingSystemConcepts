#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <string>

class Process {
public:
    int pid;
    int arrival_time;
    int burst_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;

    Process(int id, int arrival, int burst)
        : pid(id), arrival_time(arrival), burst_time(burst),
          completion_time(0), waiting_time(0), turnaround_time(0) {}
};

class SchedulingAlgorithms {
public:
    void fcfs_scheduling(std::vector<Process>& processes) {
        std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;
        for (auto& process : processes) {
            if (current_time < process.arrival_time) {
                current_time = process.arrival_time;
            }
            process.completion_time = current_time + process.burst_time;
            process.turnaround_time = process.completion_time - process.arrival_time;
            process.waiting_time = process.turnaround_time - process.burst_time;
            current_time += process.burst_time;
        }
    }

    void sjn_scheduling(std::vector<Process>& processes) {
        std::sort(processes.begin(), processes.end(), [](Process& a, Process& b) {
            return a.arrival_time < b.arrival_time;
        });

        int current_time = 0;
        std::vector<Process> ready_queue;

        while (!processes.empty() || !ready_queue.empty()) {
            while (!processes.empty() && processes.front().arrival_time <= current_time) {
                ready_queue.push_back(processes.front());
                processes.erase(processes.begin());
            }
            if (!ready_queue.empty()) {
                std::sort(ready_queue.begin(), ready_queue.end(), [](Process& a, Process& b) {
                    return a.burst_time < b.burst_time;
                });
                Process process = ready_queue.front();
                ready_queue.erase(ready_queue.begin());

                if (current_time < process.arrival_time) {
                    current_time = process.arrival_time;
                }

                process.completion_time = current_time + process.burst_time;
                process.turnaround_time = process.completion_time - process.arrival_time;
                process.waiting_time = process.turnaround_time - process.burst_time;
                current_time += process.burst_time;
            } else {
                current_time = processes.front().arrival_time;
            }
        }
    }

    void rr_scheduling(std::vector<Process>& processes, int time_quantum) {
        std::queue<Process*> ready_queue;
        int current_time = 0;
        int index = 0;

        while (index < processes.size() || !ready_queue.empty()) {
            while (index < processes.size() && processes[index].arrival_time <= current_time) {
                ready_queue.push(&processes[index]);
                index++;
            }
            if (!ready_queue.empty()) {
                Process* process = ready_queue.front();
                ready_queue.pop();

                if (process->burst_time > time_quantum) {
                    current_time += time_quantum;
                    process->burst_time -= time_quantum;
                    while (index < processes.size() && processes[index].arrival_time <= current_time) {
                        ready_queue.push(&processes[index]);
                        index++;
                    }
                    ready_queue.push(process);
                } else {
                    current_time += process->burst_time;
                    process->completion_time = current_time;
                    process->turnaround_time = process->completion_time - process->arrival_time;
                    process->waiting_time = process->turnaround_time - process->burst_time;
                }
            } else {
                current_time = processes[index].arrival_time;
            }
        }
    }

    void print_gantt_chart(const std::vector<Process>& processes) {
        std::cout << "Gantt Chart:\n";
        int current_time = 0;
        for (const auto& process : processes) {
            if (current_time < process.arrival_time) {
                current_time = process.arrival_time;
            }
            std::cout << "| P" << process.pid << " ";
            current_time += process.burst_time;
        }
        std::cout << "|\n";

        current_time = 0;
        for (const auto& process : processes) {
            if (current_time < process.arrival_time) {
                current_time = process.arrival_time;
            }
            std::cout << current_time << std::setw(4 + std::to_string(process.pid).length()) << "";
            current_time += process.burst_time;
        }
        std::cout << current_time << "\n";
    }
};
int main() {
    int choice;
    std::cout << "Choose an option:\n";
    std::cout << "1. Enter custom processes\n";
    std::cout << "2. Use example processes\n";
    std::cin >> choice;

    std::vector<Process> processes;

    if (choice == 1) {
        int num_processes;
        std::cout << "Enter the number of processes: ";
        std::cin >> num_processes;

        for (int i = 0; i < num_processes; ++i) {
            int pid, arrival_time, burst_time;
            std::cout << "Enter Process ID, Arrival Time, and Burst Time for process " << (i + 1) << ": ";
            std::cin >> pid >> arrival_time >> burst_time;
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
    std::cout << "Enter time quantum for Round Robin scheduling: ";
    std::cin >> time_quantum;

    SchedulingAlgorithms scheduler;

    // FCFS Scheduling
    std::vector<Process> fcfs_processes = processes;
    scheduler.fcfs_scheduling(fcfs_processes);
    std::cout << "\nFCFS Scheduling:\n";
    for (const auto& process : fcfs_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart(fcfs_processes);

    // SJN Scheduling
    std::vector<Process> sjn_processes = processes;
    scheduler.sjn_scheduling(sjn_processes);
    std::cout << "\nSJN Scheduling:\n";
    for (const auto& process : sjn_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart(sjn_processes);

    // RR Scheduling
    std::vector<Process> rr_processes = processes;
    scheduler.rr_scheduling(rr_processes, time_quantum);
    std::cout << "\nRR Scheduling:\n";
    for (const auto& process : rr_processes) {
        std::cout << "Process " << process.pid << ": Waiting Time = " << process.waiting_time
                  << ", Turnaround Time = " << process.turnaround_time << "\n";
    }
    scheduler.print_gantt_chart(rr_processes);

    return 0;
}
