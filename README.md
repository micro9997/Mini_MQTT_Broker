# Mini_MQTT_Broker
Mini MQTT broker and client

Using,

C, System call, TCP/IP Socket, Multithreading and Single linked list

&nbsp;

### Introduction
MQTT is a lightweight protocol for machine-to-machine communication and is most suitable for IoT devices. It uses TCP/IP sockets for communicating through IP.

Here I have created server (broker) and client programs. This single client program can work as multiple clients simultaneously, so a broker can communicate with multiple clients simultaneously using multithreading. The broker stores the data (topics, values, and file descriptors of clients) using a single linked list. Then, when the published message is received from the clients, the broker will send the data only to the clients who have subscribed to the topics based on the data stored in the single linked list.

&nbsp;

*Demonstration video*

[https://youtu.be/smnAeb7OjQg](https://youtu.be/smnAeb7OjQg)

&nbsp;

### Testing

![image_1](https://github.com/micro9997/Mini_MQTT_Broker/blob/master/images/image_1.png)


&nbsp;
