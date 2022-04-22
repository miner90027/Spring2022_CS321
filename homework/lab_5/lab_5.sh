#!/bin/bash

# lab_5.sh
# Aleks McCormick
# 2022/21/04
# Spring 2022 CS321
# Docker build script to install Anaconda & Jupyter notebook

# pull simple ubuntu Docker image
sudo docker pull ubuntu
docker run -it ubuntu

apt-get update && apt-get -y upgrade

# download anaconda installer script inside docker image
wget https://repo.anaconda.com/archive/Anaconda3-2021.11-Linux-x86_64.sh -O ~/anaconda.sh

# change file permissions
chmod 777 anaconda.sh

# Run the anaconda installer in silent mode
bash ~/anaconda.sh -b -p -f $HOME/anaconda

# create Conda environment
conda create -n $HOME/anaconda

# Activate conda environment
conda activate $HOME/anaconda

# install jupyter
conda install -c anaconda jupyter
