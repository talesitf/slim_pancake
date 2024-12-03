#!/bin/bash
#SBATCH --job-name=pizera
#SBATCH --output=output_vector_search.txt
#SBATCH --mem=512
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --time=00:05:00

./omp_vs_schedulers