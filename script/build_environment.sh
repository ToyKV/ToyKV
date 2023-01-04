#!bin/bash
sudo apt-get -y update

# Install packages.
sudo apt-get -y install \
      build-essential \
      clang-12 \
      clang-format-12 \
      clang-tidy-12 \
      cmake \
      doxygen \
      git \
      g++-12 \
      pkg-config \
      zlib1g-dev \
      python-pip

# Install xmake
sudo add-apt-repository ppa:xmake-io/xmake
sudo apt update
sudo apt install xmake

# Install cpplint
pip install --user cpplint
