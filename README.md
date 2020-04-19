# Compression Detection

This project is an implementation of end-to-end compression detection of network traffic flows by intermediaries inspired by the research of Vahab Pournaghshband, Alexander Afanasyev, and Peter Reiher. The range of detection control through the reading expressed its sensitivity to the layer in which we implement our observation and expressed the possibility of encountering servers with different levels of compliance. To narrow the varieties of implementation, we decided to restrict our methods to detect TCP/IP compression events with the possibility of encountering either a cooperative or responsive server.

## Collaborative End Points  
Here, two factors come into play: the [client](client_cooperative.c) and [server](server_cooperative.c). The client is responsible for carrying out an initial TCP connection that sends our [configuration file](myconfig.json) to the server-side in char array representation. When that information is received, the server generates a structure that holds the configuration data for later use in communication and processing. The second step is taken by the client, where it sends 6000 packets as one chain, holds for 25 seconds, then sends another set with the same amount of packets. These packets, however, differ in the payload that they hold, for we can see that the first chain carries **low entropy data** and the latter carries **high entropy data**. 

Data Type         | Meaning                                                 | Compression Capacity
-------------     | -------------                                           | -------------
low entropy data  | Data with highly repetitive information           | High
high entropy data | Data with less frequent similarity of information | Low 


Noting from the table, we can see that the effects of compression are not autonomous for all data and therefore it is necessary to include the differences for better accuracy in detecting its presence. 

We compute the difference in time between the arrival of the first bit of the first packet up until the first bit of the last packet received from the low entropy train (∆tL), repeat this calculation for the high entropy set (∆tH), and check for compression by calculating their difference. Compression is present when the difference is less than a threshold (100 ms) calculated from the time precision and performance of our machine. This result is then sent to the client as a form of a buffer for further analysis.

## Responsive End Point 
Unlike cooperative servers, responsive hosts are free from any restrictions or orders enforced by the [client](standalone.c), making it difficult to manipulate for data retrieval. From what we know, any host machine in the network holds a wide range of ports that need to be open for connection in order to receive data. However, when a client attempts to communicate with an unavailable port, RST packets are sent back to inform the client of the failed attempt. 

An initial TCP connection is established to send a SYN packet to an arbitrary server. These packets are assigned to a closed port **x** in order to proceed with the three-way handshake, yet are dropped with an alert being sent back to the client. the client then sends the packet trains discussed earlier to a valid port and ends the connection with a final TCP connection to port **y** to retrieve our second RST packet. the difference of duration between the arrival of both alerting packets will be measured and compared to the threshold previously mentioned to detect compression. 

## Usage for client

```bash
make client_cooperative
./client_cooperative myconfig.json
```

## Usage for server

```bash
make server_cooperative
./server_cooperative
```

## Usage for standalone (run once)

```bash
make standalone
./standalone myconfig.json
```

## Usage for standalone (incrementing TTL)

```bash
bash standalone.sh
```

or  

```bash
./stadalone.sh
```

and you may need to run the command below if you choose the second option

```bash
chmod +x standalone.sh
```

## Design Choices

1. We chose to have a timeout of 5 minutes for all applications.  We felt that this was long enough for any of our programs to find the answer so if they didn't then something must have gone wrong.

2. We chose to not include the myconfig.json in the server side because the point of the first TCP connection was to send over the information so there would be no reason to have the first connection if the server already had the information.

3. For the client server application we decided to use regular socet connections to make our programs easier to understand and because we did not need to.  However, for the standalone we decided to use the more indepth example of raw sackets setting up each frame to learn more as well as have more control.

4. We split up our code into header files, multiple c files, and a makefile.  This was to make the code cleaner.

5. We used json format for the config file with all the options specified in the project directions. We then created a struct to put in this information.

6. For the standalone, we decided to use the pcap library in order to capture the RST packets and forked the process.  The child would be the packet capturer and the parent would be the sender of the packets.  Just to be safe, we added a slight delay of 5 seconds in the parent process before sending the packets so the child has time to get set up.

7. For the client and server application, we decided to send 1 extra UDP packet that would not be part of the timing test (this is why there is an extra packet in the packet capture).  We wanted to make our timing of compression accurate and because the server needs to start the connection before the client, we thought the best way to synchronize the programs would be to send the extra packet that would not be part of the test and would end up making the timingtest more accurate.

8. For the standalone we decided to make the program run once by running ./standalone with whatever configuration the user chooses then made a wrapper script incrementing the TTL for the last part of the project for detecting compression.  The easiest way to do this was to add an extra argument for the standalone program to take in a TTL if the user specified which would overwrite what was read in from the config file in the struct that we created.  If the user wishes they could specify a TTL for example ./standalone myconfig.json 100, but this option is really meant for the wrapper script that we created.

## Resources
https://lasr.cs.ucla.edu/vahab/resources/compression_detection.pdf
