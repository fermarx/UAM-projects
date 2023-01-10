from Cryptodome.PublicKey import RSA
import requests as req
import json


def registrarUsuario(nombre, email, token):
    """Crea una nueva identidad (par de claves púlica y privada)
       y la registra en SecureBox, para que pueda ser encontrada
       por otros usuarios.

    Args:
        nombre (String): nombre completo
        email (String): email
        token (String): token almacenada en el PC

    Returns:
        int -1: en caso de error
    """
    print("Generando par de claves RSA de 2048 bits...", end="")
    clavePrivada = RSA.generate(2048)
    clavePrivadaPEM = clavePrivada.exportKey('PEM')

    f = open('clavePrivada.bin', 'wb')
    f.write(clavePrivadaPEM)
    f.close()

    clavePublica = (clavePrivada.publickey()).exportKey('PEM')

    f = open('clavePublica.bin', 'wb')
    f.write(clavePublica)
    f.close()

    clavePublica = clavePublica.decode('utf-8')

    url = 'https://vega.ii.uam.es:8080/api/users/register'
    args = {"nombre": nombre, "email": email, "publicKey": clavePublica}
    headers = {"Authorization": "Bearer " +
               token, "Content-Type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))

    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("OK")
        print("Identidad con ID#"+str(ret['userID'])+" creada correctamente")


def buscarUsuario(cadena, token):
    """Busca un usuario en el repositorio de identidades 
       de SecureBox introduciendo el nombre o el email 
       como argumento

    Args:
        cadena (String): nombre o email del usuario a buscar
        token (String): token almacenada en el PC

    Returns:
        int -1: en caso de error
    """
    print("Buscando usuario " + cadena + " en el servidor...", end="")

    url = 'https://vega.ii.uam.es:8080/api/users/search'
    args = {"data_search": cadena}
    headers = {"Authorization": "Bearer " +
               token, "Content-Type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))

    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("OK")
        if (len(ret) == 0):
            print("No existen usuarios que coincidan con ese nombre o email")
        else:
            print(str(len(ret)) + " usuarios encontrados:")

            contador = 1
            for usuario in ret:
                print("["+str(contador)+"] "+str(usuario['nombre'])+", "
                      + str(usuario['email'])+", ID: "
                      + str(usuario['userID']))
                contador += 1


def borrarUsuario(id, token):
    """Borra la identidad con ID id registrada en el sistema. 
       Sólo se pueden borrar aquellas identidades creadas por 
       el usuario que realiza la llamada.

    Args:
        id (String): id del usuario a borrar
        token (String): token almacenada en el PC

    Returns:
        int -1: en caso de error
    """
    print("Solicitando borrado de la identidad #" + id + "...", end="")

    url = 'https://vega.ii.uam.es:8080/api/users/delete'
    args = {"userID": id}
    headers = {"Authorization": "Bearer " +
               token, "Content-Type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))

    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("OK")
        print("Identidad con ID#" + ret['userID'] + " borrada correctamente")


def obtenerClavePublica(id, token):
    """Obtener la clave pública de un usuario

    Args:
        id (String): id del usuario
        token (String): token almacenada en el PC

    Returns:
        int -1: en caso de error]
    """
    print("-> Recuperando clave pública de ID " + id + "...", end="")

    url = 'https://vega.ii.uam.es:8080/api/users/getPublicKey'
    args = {"userID": id}
    headers = {"Authorization": "Bearer " +
               token, "Content-Type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))

    ret = json.loads(r.content.decode('utf-8'))
    clave = ret['publicKey']

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        if ret is not None:
            print("OK")
        else:
            print("Error: Clave pública no encontrada.")

        clave = ret['publicKey']
        return clave
