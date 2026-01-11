#!/bin/bash
#SBATCH --job-name=TCLB:gdl260_m2
#SBATCH --partition=gpu
#SBATCH --account=pawsey1178-gpu
#SBATCH --nodes=1
#SBATCH --gres=gpu:3
#SBATCH --time=24:00:00

ulimit -l unlimited
echo "running on:"
hostname

date

set -e

module load r/4.4.1
module load rocm/6.2.0
module load python/3.11.6
module load craype-accel-amd-gfx90a

cd /scratch/pawsey1178/nparaskevas/TCLB
srun -N 1 -n 3 -c 8 --gres=gpu:3 /scratch/pawsey1178/nparaskevas/TCLB/CLB/d3q27_pf_velocity/main cases/gdl260_vsc_m2.xml
