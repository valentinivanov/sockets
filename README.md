*** Install prerequisites (on Ubuntu 16.04) ***
1. Install c++ compiler and make.
sudo apt-get install g++
sudo apt-get install build-essential
2. Install cmake
sudo apt-get cmake
3. Install pthreads library
sudo apt-get install libpthread-stubs0-dev 

*** Building the project ***
1. Enter project build dir
For example: cd ./projects/attotask/build
Your actual path to attotask dir may be different.
2. Run cmake
cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug ..
3. Run make
make -j 8

Optionally you may use VSCode with C++ extension and CMake extension installed. 
Just open project folder ./projects/attotask and run linux:cmake task and then linux:make task.

*** Running the project ***
There are 3 executables in the build directory after successfull build:
udp_producer, udp_consumer, tcp_consumer.

udp_producer produces stream of datagrams, 
udp_consumer listens to the multicast socket, stores messages and forwards filtered messages to the tcp_consumer via TCP connection.
tcp_consumer receives forwarded messages via TCP connection and prints their attributes to the console.

Run them each in its own terminal window in the following order:
1. tcp_consumer
2. udp_consumer
3. udp_producer

*** Code guide ***
There are 3 CPP files in the project: 
udp_producer.cpp, udp_consumer.cpp and tcp_consumer.cpp. 
They contain entry points for each of project programs.

Application classes are stored in HPP files and shared between programs.

There is threading wrapper over pthread library: worker_thread. It allows to run tasks defined at compile time.

There are 2 types of runnable tasks:
udp_consumer_task performing reading from the socket, storage of the message and notifying tcp_consumer_forward_task via the queue.
tcp_consumer_forward_task forwards messages filtered by the udp_consumer_task to the tcp_consumer.
Both tasks work with sockets in non-blocking manner and don't lock shared data structures.

There are wrappers over POSIX sockets: udp_socket, tcp_socket and 
more high-level classes for sending and receiving messages: 
udp_multicast_receiver, udp_multicast_sender, tcp_client, tcp_server.

There are 2 lock-free data structures: 
atomic_hashset representing big static hash table and
atomic_queue representing queue over circular buffer.
