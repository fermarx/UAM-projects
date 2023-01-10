'''
Practica 3 Redes 2

servidor_descubrimiento.py: 
Descubre el servidor y envía las peticiones echas por el cliente.

'''

import socket

from appJar import gui
import cv2


SERVER_ADDRESS = ("vega.ii.uam.es",8000) # URL del servidor de descubrimiento
PROTOCOL = 'V0'

REGISTER_RESPONSE = 2048
USERS_BEGIN_SIZE = 16

PORT_UDP = 8001 # Puerto para transmision de video
PORT_CONTROL = 8002 # Puerto para control de llamadas

MY_USER = 'fermarx'
MY_PWD = "password"


def connect_DS():
    """Se conecta con el servidor dado

    Returns:
        socket s: socket abierto
    """

    # Creación del socket con la URL donde queremos conectarnos
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(SERVER_ADDRESS)
    return s


def register(nick, password):
    """Registra un usuario

    Args:
        nick (String):  nombre del cliente
        password (String): contraseña del cliente

    Returns:
        respuesta: respuesta que se enviará
    """

    # Llamamos al socket para que abra una conexión al servidor
    s = connect_DS()
    # Cogemos nuestra IP
    #ip = getIP()
    ip = '127.0.0.1'
    # se crea el mensaje que el cliente le envia al servidor
    cs = 'REGISTER ' + nick + ' ' + ip + ' ' + str(PORT_CONTROL) + ' ' + password + ' ' + PROTOCOL
    # Enviamos el mensaje
    s.send(cs.encode())
    # Recibimos la respuesta
    respuesta = s.recv(REGISTER_RESPONSE)
    return respuesta


def getIP():
    # Conectamos con una web
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))

    # Obtenemos nuestra IP privada
    my_ip = s.getsockname()[0]
    print("\nMi IP es: " + my_ip)
    s.close()
    
    return str(my_ip)


def query(nick):
    """Devuelve la informacion de un usuario pedido

    Args:
        nick (String):  nombre del usuario a buscar

    Returns:
        res: diccionario con la informacion del usuario pedido
    """

    # se crea el mensaje que el cliente le envia al servidor
    cs = 'QUERY ' + str(nick)

    # Llamamos al socket para que abra una conexión al servidor
    s = connect_DS()
    s.sendall(cs.encode())

    # Respuesta del servidor
    response = s.recv(REGISTER_RESPONSE)

    # Formateamos
    res = response.decode().split(' ')

    # Almacenamos el usuario en un diccionario y lo devolvemos
    if res[0] == "OK":
        dict ={}
        dict['nick'] = res[2]
        dict['ip'] = res[3]
        dict['port'] = res[4]
        dict['protocol'] = res[5]

        return dict

    return None


def list_users():
    """Devuelve la lista de usuarios en el servidor

    Returns:
        res: lista con la informacion de los usuarios
    """

    # Mensaje cliente-servidor
    cs = 'LIST_USERS'

    # Llamamos al socket para que abra una conexión al servidor
    s = connect_DS()
    s.sendall(cs.encode())

    # Respuesta del servidor
    response = s.recv(REGISTER_RESPONSE)
    
    res = response[USERS_BEGIN_SIZE:]

    # Formateamos
    users = res.decode().split("#")
    users_list=""
    numUsers=0
    for item in users:
        numUsers += 1

    users_list = "Hay " + str(numUsers) + " usuarios registrados\n"

    i = 0
    #Añadimos todos los usuarios a una lista
    for item in users:
        i += 1
        users_list += str(i) + " - " + item +"\n"
    
    return users_list


def quit_server():
    """
    Cierra la conexión con el servidor
    """

    # Mensaje cliente-servidor
    cs = "QUIT"
    
    # Llamamos al socket para que abra una conexión al servidor
    s = connect_DS()
    s.sendall(cs.encode())
    response = s.recv(REGISTER_RESPONSE)

    print(response.decode())
    
    # Cerramos la conexion
    s.close()


def calling(user):    
    """Llama a un usuario dado

    Args:
        user (String):  nombre del usuario a llamar

    Returns:
        ip: diccionario con la informacion de la ip del usuario a llamar
        response: respuesta que se enviará
    """
    
    # nick, ip, port, protocol
    dict = query(user)

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((dict['ip'], int(dict['port'])))
        cs = 'CALLING ' + MY_USER + ' ' + str(PORT_UDP)
        # Enviamos el mensaje
        s.send(cs.encode())
        response = s.recv(REGISTER_RESPONSE)
        print('Respuesta recibida: ' + response.decode())
        return dict['ip'], response


def call_end(user):
    """Termina la llamada a un usuario dado

    Args:
        user (String):  nombre del usuario con el que terminar la llamada
    """

    dict = query(user)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Nos conectamos con el servidor
        s.connect((dict['ip'], int(dict['port'])))
        # Creamos el mensaje a enviar
        message = 'CALL_END ' + MY_USER
        # Enviamos el mensaje
        s.send(message.encode())


def call_accept(user):
    """Acepta la llamada a un usuario dado

    Args:
        user (String): nombre del usuario para aceptar la llamada

    Returns:
        True : si acepta la llamada
    """

    dict = query(user)
    protocol = str(dict['protocol'])
    if protocol == PROTOCOL:
        return True
    
    return False


def call_on_hold(user):
    """Poner la llamada en pausa

    Args:
        user (String): nombre del usuario para poner la llamada en pausa
    """

    dict = query(user)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Nos conectamos con el servidor
        s.connect((dict['ip'], int(dict['port'])))
        # Creamos el mensaje a enviar
        message = 'CALL_HOLD ' + MY_USER
        # Enviamos el mensaje
        s.send(message.encode())


def call_resume(user):
    """Reanuda la llamada en pausa

    Args:
        user (String): nombre del usuario para reanudar la llamada en pausa
    """
    
    dict = query(user)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Nos conectamos con el servidor
        s.connect((dict['ip'], int(dict['port'])))
        # Creamos el mensaje a enviar
        message = 'CALL_RESUME ' + MY_USER
        # Enviamos el mensaje
        s.send(message.encode())
