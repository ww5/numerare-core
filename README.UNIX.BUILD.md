# update repository
sudo apt update

# install git 
sudo apt install git

# dependencies
sudo apt install build-essential cmake libboost-all-dev miniupnpc libunbound-dev graphviz doxygen libunwind8-dev pkg-config libssl-dev libcurl4-openssl-dev libgtest-dev libreadline-dev libminiupnpc-dev libzmq3-dev

# build
make clean

make 
