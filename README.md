# Mini_MQTT_Broker
Mini MQTT broker and client

Using,

C, System call, TCP/IP Socket, Multithreading and Single linked list

&nbsp;

### Introduction
MQTT is a lightweight protocol for machine-to-machine communication and is most suitable for IoT device communication. It uses TCP/IP sockets for communicating through IP.

Here I'm creating server (broker) and client programs. This single client program can work as multiple clients at the same time, so a broker can communicate with multiple clients at a time using multithreading. The broker stores the data (topics, values, and file descriptors of clients) using single linked list. Then, if a published message is received, the broker sends data to subscribed clients only based on the data stored in a single linked list.

&nbsp;

*Demonstration video*

[https://youtu.be/smnAeb7OjQg](https://youtu.be/smnAeb7OjQg)

&nbsp;

### Testing

![image_1](https://github.com/micro9997/Mini_MQTT_Broker/blob/master/images/image_1.png)


&nbsp;
