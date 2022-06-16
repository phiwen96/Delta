# Delta
C++ library

sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt install gcc-12
sudo apt install g++-12
sudo apt install libssl-dev
sudo apt install libaio-dev
sudo apt install clang-12   

sudo apt install clang-13
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-13 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-13 100

# Potential errors on mac 
	error: invalid parameter name: 'module' is a keyword
	fix: rename all occurences of it to something else

# developer notes
	- t's easy to make many small mistakes like using a new GPU feature and forgetting to request it at logical device creation time.