import requests as req
import json
from Cryptodome.Cipher import AES, PKCS1_OAEP
from Cryptodome.Random import *
from Cryptodome.Util.Padding import pad, unpad
from Cryptodome.PublicKey import *
from Cryptodome.Hash import *
from Crypto.Signature import pkcs1_15
from usuarios_identidades import *
import copy
import os
import sys
import re


def encriptarFichero(fichero, idDestino, token):
    """Se cifra el fichero del argumento (tanto si está firmado como si no)
       con la clave pública del destinatario siguiendo el esquema híbrido de 
       las presentaciones de teoría 

    Args:
        fichero (String):  nombre del fichero a cifrar
        idDestino (String): id del destinatario para obtener clave pública
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    print("-> Cifrando fichero " + fichero +
          " para el destinatario ID#" + idDestino + "...", end="")
    try:
        f = open(fichero, 'rb')
        output = f.read()
        f.close()
    except (FileExistsError, FileNotFoundError):
        print("Error: no se puede leer el archivo")
        return -1

    # Usamos clave simétrica de 256 bits (32 Bytes) para cifrar el mensaje
    claveSimetrica = SHA256.new(get_random_bytes(32)).digest()

    # Se usará AES con modo de encadenamiento CBC que necesita un vector de inicialización IV de 16 Bytes
    iv = get_random_bytes(16)

    # Ciframos el documento con la clave simétrica. Necesitamos hacer padding del IV
    # docs: https://pycryptodome.readthedocs.io/en/latest/src/util/util.html
    cifrado = AES.new(claveSimetrica, AES.MODE_CBC, iv=iv)
    mensajeCifrado = cifrado.encrypt(pad(output, 16))
    print("OK")

    # Ciframos la clave simétrica mediante RSA con la clave pública del destinatario de 2048 bits
    # docs: https://www.dlitz.net/software/pycrypto/api/2.6/Crypto.Cipher.PKCS1_OAEP-module.html
    clavePublicaDest = obtenerClavePublica(idDestino, token)
    if clavePublicaDest is None:
        return -1

    clavePublicaDest = RSA.importKey(clavePublicaDest)
    cipher = PKCS1_OAEP.new(clavePublicaDest)
    claveSimetricaCifrada = cipher.encrypt(claveSimetrica)

    # Creamos el nuevo fichero cifrado
    with open(fichero, 'wb') as f:
        f.write(iv + claveSimetricaCifrada + mensajeCifrado)


def firmarFichero(fichero):
    """Firmar digitalmente un fichero con la clave privada propia

    Args:
        fichero (String):  nombre del fichero a firmar
    """
    print("-> Firmando fichero " + fichero + "...", end="")
    try:
        f = open(fichero, 'rb')
        output = f.read()
        f.close()
    except (FileExistsError, FileNotFoundError):
        print("Error: no se puede leer el archivo")
        return -1

    # Firmamos el hash del documento con nuestra clave privada
    # docs: https://pycryptodome.readthedocs.io/en/latest/src/signature/pkcs1_v1_5.html
    try:
        clavePrivada = RSA.importKey(open("clavePrivada.bin", "rb").read())
    except (FileNotFoundError):
        print("\nError: Debes crear primero una nueva identidad con --create_id")
        return -1

    # Función hash: SHA256
    hashFichero = SHA256.new(output)
    firma = pkcs1_15.new(clavePrivada).sign(hashFichero)

    with open("firmaDigital.bin", 'wb') as f:
        f.write(firma)

    print("OK")
    print("-> Firma digital del fichero " +
          fichero + " guardada en 'firmaDigital.bin'")


def encriptarFirmarFichero(fichero, idDestino, token):
    """Firmar digitalmente un fichero y cifrarlo junto con la firma

    Args:
        fichero (String):  nombre del fichero a firmar y cifrar
        idDestino (String): id del destinatario
        token (String): token almacenada en pc

    Returns:
        fichero_cifrado (String): El nombre del fichero firmado y cifrado
    """
    if (firmarFichero(fichero) == -1):
        return -1
    try:
        f = open("firmaDigital.bin", 'rb')
        firmaDigital = f.read()
        f.close()
        f = open(fichero, 'rb')
        contenido = f.read()
        f.close()
    except (FileExistsError, FileNotFoundError):
        print("Error: no se puede leer el archivo")

    p = re.split("[.]", fichero)
    fichero_cifrado = "fichero_cifrado." + p[1]

    with open(fichero_cifrado, 'wb') as f:
        f.write(firmaDigital + contenido)

    encriptarFichero(fichero_cifrado, idDestino, token)

    if os.path.exists("firmaDigital.bin"):
        print("-> Borrando firma digital 'firmaDigital.bin'...", end="")
        os.remove("firmaDigital.bin")
        print("OK")

    return fichero_cifrado


def desencriptarFichero(fichero, idOrigen, token):
    """Descifra un fichero y comprueba la firma digital

    Args:
        fichero (String):  nombre del fichero a firmar y cifrar
        idOrigen (String): id del usuario de origen, para comprobar la firma digital
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    print("-> Descifrando fichero...", end="")

    # Primeros 16 bytes son el vector IV
    iv = fichero[:16]

    # Desciframos la clave simétrica usando nuestra clave privada
    try:
        clavePrivada = RSA.importKey(open("clavePrivada.bin", "rb").read())
    except (FileNotFoundError):
        print("\nError: Debes crear primero una nueva identidad con --create_id")
        return -1

    cipher = PKCS1_OAEP.new(clavePrivada)

    try:
        claveSimetrica = cipher.decrypt(fichero[16:272])
    except ValueError:
        print(
            "\nError: Clave no válida, usted no es el receptor de destino de este fichero.")
        return -1

    # Desciframos el resto del mensaje donde los primeros 256 bytes son la firma digital
    cifrado = AES.new(claveSimetrica, AES.MODE_CBC, iv=iv)
    mensajeDescifrado = unpad(cifrado.decrypt(fichero[272:]), 16)

    firmaDigital = mensajeDescifrado[:256]
    mensaje = mensajeDescifrado[256:]
    print("OK")

    # Comprobamos firma
    clavePublicaOrigen = obtenerClavePublica(idOrigen, token)
    clavePublicaOrigen = RSA.importKey(clavePublicaOrigen)
    hashFichero = SHA256.new(mensaje)

    print("-> Verificando firma...", end="")
    try:
        pkcs1_15.new(clavePublicaOrigen).verify(hashFichero, firmaDigital)
    except (ValueError, TypeError):
        print("\nError: firma digital no coincidente. El mensaje puede haber sido interceptado y modificado o enviado por otro emisor")
        return -1

    print("OK")
    return mensaje
