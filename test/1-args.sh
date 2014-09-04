./server 3000&
./client test 1 127.0.0.1 3000 ./input/script1

# kill async server process
trap 'kill $(jobs -p)' EXIT
