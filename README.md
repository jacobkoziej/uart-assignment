# uart-assignment

> The objective of this activity is to learn how to program two wireless
> devices such as bluetooth to communicate through UART RS-232 serial
> communication.  The main focus is to create two programs that exchange data
> through a defined protocol to ensure data will reach its destination, avoid
> data overlap and time overrun, and data are stored in the proper sequential
> manner.


## Materials

* 2PCS Arduino Uno Rev3
* Breadboard
* Flex sensor
* Pushbutton
* 10 kΩ resistor
* 8.8 kΩ resistor
* Jumpers


## Theory

Serial communication is prevalent in both the computer industry in general and
the embedded microcontroller industry.  UART is asynchronous communication and
the simplest serial protocol that transmits one bit at a time and one byte per
cycle.  UART utilizes a full duplex communication using the receive (RX) and
transmit (TX) wires to send and receive data simultaneously.  UART uses
a defined protocol to send and receive a byte.  This protocol defines the
transmission time of the bit in addition to adding a starting bit to trigger
the transmission cycle and an ending bit to indicate the end of the
transmission cycle.  The protocol also has an additional feature such as parity
to ensure data transmitted and received are identical and there is no data lost
during transmission.

When sending data from different devices there are a few issues needed to be
addressed to ensure that the data exchanged are correctly received and didn't
get lost during transmission.  Another crucial problem arises is the
synchronisation of transmitted data in the absence of the clocking signal.  To
overcome these challenges we will have to define our own protocol to send and
receive packets of data.  This protocol will be established based on the
application needed.  The following is an example of exchange data between two
MCUs using master and slave method:

### Master

1. Declare the buffer and variables
2. Begin serial communication
3. Transmit
4. If no reply resend packet
5. Check for data availability
6. Check first byte/name
7. Check for last byte/end
8. Repeat from step 3 to 7

### Slave

1. Declare the buffer
3. Begin serial communication
3. Check for data availability
4. Check first byte / name
5. Check for last byte
6. Transmit Repeat form step 3 to 6


## Assignment

### Task one

Create a serial interface between two Arduinos where the master will send five
bytes to the slave and receive six bytes asynchronously such as handshaking
protocol.  Transmit and receive must be sequential and as follow:

1. Master transmit data and wait to receive data from salve
2. Master will resend packet if it didn't receive data from salve within
   definite interval time
3. Slave will send data upon receiving data from master
4. Data must have starting and ending bytes
5. Task must go on forever
6. Add one more constraint of your own

### Task two

Add a servo motor to the **slave** MCU and a flex sensor to the **master** MCU.
Map the value of the flex sensor to control the angle of the servo motor
(0-180).  Use the serial communication protocol you created in task one to send
the value of the flex sensor.  Add a toggle pushbutton to the master to allow
users to enable and disable serial communication.


---

Credits to Ali Harb for creating this assignment.
