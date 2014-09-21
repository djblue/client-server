# 1. What does a client do if it tries using a locked file?
# 2. How does the incarnation number work?
#  3. I get that you must release files previously locked on new incarnation.

# Failure of the client’s machine is simulated by incrementing the incarnation
# number. Since there may be multiple clients all running on the same machine m,
# the incarnation number must be stored on stable storage (i.e., a file). The
# file used to obtain and update the current incarnation number must be locked
# and unlocked every time it is accessed.


./server 3000&

node test/1-args.js

echo 'c1.txt ---------------------------------'
cat c1.txt
echo 'c2.txt ---------------------------------'
cat c2.txt
echo 'c3.txt ---------------------------------'
cat c3.txt

# kill async server process
trap 'kill $(jobs -p)' EXIT
