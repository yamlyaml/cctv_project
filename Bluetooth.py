from bluetooth import *
import time
#Mac address list
#HC-06 : 98:DA:60:01:8D:84
#TEAMNULL : 98:DA:60:02:C0:B5

global client_socket
client_socket = BluetoothSocket( RFCOMM )

def Bluetooth_Connect():
    global client_socket
    client_socket.connect(("98:DA:60:01:8D:84",1))
    print ("Python : bluetooth connected!")
    return

def DataReceived():
    global client_socket
    receivedmsg = client_socket.recv(1024)
    print('Python : received message :',receivedmsg)
    val = int.from_bytes(receivedmsg,byteorder='big',signed=False)
    return val

def DataSend_ON1():
    global client_socket
    print('Python : sended message : BUZZER1 ON')
    client_socket.send(b'1')
    return

def DataSend_ON2():
    global client_socket
    print('Python : sended message : BUZZER2 ON')
    client_socket.send(b'2')
    return
    
def DataSend_ON3():
    global client_socket
    print('Python : sended message : BUZZER3 ON')
    client_socket.send(b'3')
    return




def DataSend_OFF():
    global client_socket
    print('Python : sended message : BUZZER OFF')
    client_socket.send(b'0')
    return

def Bluetooth_Disconnect():
    global client_socket
    client_socket.close();
    print ("Python : bluetooth disconnected!")
    return

