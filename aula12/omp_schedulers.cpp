#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

void print_iterations(const std::string& description,
const std::vector< std::vector<int> >& vectors,
const int n)
{
    std::vector< std::string > strings(4, std::string());
    for (int i = 0; i != n; i++)
    {
        for (int j = 0; j != 4; j++)
        {
            const auto& vector = vectors[j];
            auto it = std::find(vector.begin(), vector.end(), i);
            if (it != vector.end())
            {
                strings[j] += "*";
            }
            else 
            { 
                strings[j] += " ";
            }
        }
    }
    std::cout << description << std::endl;
    for (auto& s : strings)
    {
        std::cout << s << "\n";
    }
    std::cout << std::endl;
}

template <typename T>
double schedule(T function, 
                const std::string& description, 
                const int n)
{
    std::vector< std::vector<int> > vectors(4, std::vector<int>());
    double start_time = omp_get_wtime();
    function(vectors, n);
    double end_time = omp_get_wtime();
    
    print_iterations(description, vectors, n);
    return end_time - start_time;
}

void scheduleDefault(std::vector< std::vector<int> >& vectors,
                     int n)
{
#pragma omp parallel num_threads(4) shared(vectors, n)
    {    
#pragma omp for 
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic(std::vector< std::vector<int> >& vectors,
                    int n)
{
#pragma omp parallel num_threads(4) shared(vectors, n)
    {    
#pragma omp for schedule(static)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic(std::vector< std::vector<int> >& vectors,
                     int n)
{
#pragma omp parallel num_threads(4) shared(vectors, n)
    {    
#pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided(std::vector< std::vector<int> >& vectors,
                    int n)
{
#pragma omp parallel num_threads(4) shared(vectors, n)
    {    
#pragma omp for schedule(guided)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleAuto(std::vector< std::vector<int> >& vectors,
                  int n)
{
#pragma omp parallel num_threads(4) shared(vectors, n)
    {    
#pragma omp for schedule(auto)
        for (int i = 0; i < n; i++)
        {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void measure_execution_time(void (*scheduler)(std::vector< std::vector<int> >&, int), 
                            const std::string& description, int n, 
                            std::vector<std::pair<std::string, double>>& summary) {
    double total_time = 0.0;
    for (int i = 0; i < 3; ++i) {
        double time_taken = schedule(scheduler, description, n);
        total_time += time_taken;
        std::cout << "Execution " << (i + 1) << " time: " << time_taken << " seconds\n";
    }
    double avg_time = total_time / 3;
    std::cout << "Average time for " << description << ": " << avg_time << " seconds\n\n";
    
    summary.push_back({description, avg_time});
}

void print_summary(const std::vector<std::pair<std::string, double>>& summary) {
    std::cout << "\n===== Execution Time Summary =====\n";
    for (const auto& entry : summary) {
        std::cout << entry.first << ": " << entry.second << " seconds\n";
    }
    std::cout << "==================================\n";
}

int main()
{
    const int n = 64;
    std::vector<std::pair<std::string, double>> summary;
    
    std::cout << "Measuring execution times...\n";
    
    measure_execution_time(scheduleDefault, "default", n, summary);
    measure_execution_time(scheduleStatic, "static", n, summary);
    measure_execution_time(scheduleDynamic, "dynamic", n, summary);
    measure_execution_time(scheduleGuided, "guided", n, summary);
    measure_execution_time(scheduleAuto, "auto", n, summary);

    // Print the summary of all average execution times
    print_summary(summary);

    return 0;
}
