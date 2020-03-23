1. we have an initial TCP connection followed by 6000 UDP packets and terminated with an end TCP connection. During the retrieval of these packets on the server side are we expected to accept in a loop of 6000? what if one of the UDP packets got lost, doesnt it mean our program will buffer? will we need an accept function call for our beginning and end TCP packets?

2. error with DONT_FRAGMENT

3. are all the UDP packets sent of the same socket connection? Do we have to reassign our sockets consecutively?

4. does our high entropy data include digits outside 0 and 1?

5. is checksum deployed on the server or client side?
