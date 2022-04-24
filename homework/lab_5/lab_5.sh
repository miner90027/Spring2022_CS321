#!/bin/bash

# lab_5.sh
# Aleks McCormick
# 2022/21/04
# Spring 2022 CS321
# Docker build script to install Anaconda & Jupyter notebook

# pull simple ubuntu Docker image
docker pull ubuntu

# creat the docker container
docker create -it --name lab_5 ubuntu

# copy the anaconda installer script into the container
docker cp Anaconda3-2021.11-Linux-x86_64.sh lab_5:/anaconda.sh

# start and attach to the container
docker start lab_5

# sudo docker start -ai lab_5
# verify everything up to date
docker exec -u 0 lab_5 /bin/bash -c "apt-get update && apt-get -y upgrade"

# change file permissions
docker exec -u 0 lab_5 /bin/bash -c "chmod 777 anaconda.sh"

# Run the anaconda installer in silent mode
docker exec -u 0 lab_5 /bin/bash -c "./anaconda.sh -b -p"

# append .bashrc as needed to run conda
docker exec -u 0 lab_5 /bin/bash -c 'echo "export PATH=/root/anaconda3/bin:$PATH" >> ~/.bashrc'

docker exec -it -u 0 lab_5 /bin/bash -c 'eval "$(/root/anaconda3/conda shell.bash hook)"'

#docker restart lab_5

#docker exec -u 0 lab_5 /root/anaconda3/bin/conda "init"
docker exec -u 0 lab_5 /bin/bash -c "conda init"

docker restart lab_5

# create Conda environment
docker exec -u 0 lab_5 /bin/bash -c "conda create -q --name note"

# Activate conda environment
docker exec -u 0 lab_5 /bin/bash -c "codna activate note"

# install jupyter
docker exec -u 0 lab_5 /bin/bash -c "conda install -c -y note jupyter"
