# Compression Detection

This project is an implementation of end-to-end compression detection of network traffic flows by intermediaried inspired by the research of Vahab Pournaghshband, Alexander Afanasyev, and Peter Reiher. The range of detection control through the reading expressed its sensitivity to the layer in which we implement our observation and expressed the possibility of encountering servers with different levels of compliance.To narrow the varieties of implementation, we decided to restrict our methods to detect TCP/IP compression events with the possibilty of encountering either a cooperative or responsive server.

## Collaborative End Points  
Here, two factors come into play: the [client](client_cooperative.c) and [server](server_cooperative.c). The client is responsible for carrying out an initial TCP conenction that sends our [configuration file](myconfig.json) to the server side in char array representation. When that information is recieved, the server generates a structure that holds the configuration data for later use in comunication and processing. The second step is taken by the client, where its sends 6000 packets as one chain, holds for 25 seconds, then sends another set with the same amount of packets. These packets, however, differ in the payload that they hold, for we can see that the first chain carries **low entropy data** and the latter carries **high entropy data**. 

Data Type         | Meaning                                                 | Compression Capacity
-------------     | -------------                                           | -------------
low entropy data  | It is data with highly repetitive information           | High
high entropy data | It is data with less frequent similarity of information | Low 


Noting from the table, we can see that effects of compression are not autonomous for all data and therefore it is necessary to include the differences for better accuracy in detecting its presence. 

Finally, we compute the difference in time between the arrival of the first bit of the first packet up until the first bit of the last packet recieved from the low entropy train (∆tL), repeat this calculation for the high entropy set (∆tH), and check for compression by calculating their differences. Compression is present when the differnce is less than a threshold (100 ms) calulated from time precision and performance of our machine. This result is then sent to the client as a form of a buffer for further analysis.

## Responsive End Point 
Unlike cooperative servers, responsive hosts are free from any restrictions or orders enforced by the client, making it difficult to manipulate it for data retrieval. From what we know, any host machine in the network holds a wide range of ports that need to be open for connection in order to recieve data. However, when a client attempts to communicate with an unavailable port, RST packets are sent back to inform the client of the failed attempt. 

An initial TCP connection is established to send a SYN packet to an arbitrary server. These packets are assigned to a closed port **x** in order to precede with the three way handshake, yet are droped with an alert being sent back to the client. the client then sends the packet trains discussed earlier to a valid port, and ends the connection with a final TCP connection to port **y** to retrieve our second RST packet. the difference of duration between the arrival of both alerting packets will be measured and compared to the threshold previously mentioned to detect compression. 
## resources
https://lasr.cs.ucla.edu/vahab/resources/compression_detection.pdf
