import requests as req
import json
from cifrado_firma import *


def subirFichero(fichero, idDestino, token):
    """Envia un fichero a otro usuario. Por defecto, el archivo se subirá a SecureBox 
    firmado y cifrado con las claves adecuadas para que pueda ser recuperado y 
    verificado por el destinatario.

    Args:
        fichero (String):  nombre del fichero a cifrar
        idDestino (String): id del destinatario para obtener clave pública
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    fichero_cifrado = encriptarFirmarFichero(fichero, idDestino, token)
    if (fichero_cifrado == -1):
        return -1
    print("-> Solicitado envio del fichero " +
          fichero + " a SecureBox...", end="")

    url = 'https://vega.ii.uam.es:8080/api/files/upload'
    headers = {"Authorization": "Bearer " + token}

    # En la primera implementación, el cifrado se realizaba sobre el archivo original, de mofo
    # que ese archivo se corrompía y no podía ser visualizado a no ser que se descargara de nuevo.
    # Aquí lo que hacemos es crear una copia del fichero original, para poder cifrarlo correctamente
    # y mantener el nombre de la descarga. + info en la memoria.
    p = re.split("[.]", fichero)
    copia = "copia." + p[1]
    try:
        f = open(fichero, 'rb')
        output = f.read()
        f.close()

        f = open(fichero_cifrado, 'rb')
        output_cifrado = f.read()
        f.close()
        os.remove(fichero_cifrado)
    except (FileExistsError, FileNotFoundError):
        print("Error: no se puede leer el archivo")
        return -1

    with open(copia, "wb") as f:
        f.write(output)
    with open(fichero, "wb") as f:
        f.write(output_cifrado)

    r = req.post(url=url, headers=headers, files={
                 'ufile': open(fichero, 'rb')})
    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("OK")
        print("Subida realizada correctamente, ID del fichero: " +
              str(ret['file_id']))
        # Aquí recuperamos el contenido del fichero original para poder seguir visualizándolo en el directorio local
        # y borramos la copia que hemos creado
        try:
            f = open(copia, 'rb')
            output_original = f.read()
            f.close()
            os.remove(copia)
        except (FileExistsError, FileNotFoundError):
            print("Error: no se puede leer el archivo")
            return -1
        with open(fichero, "wb") as f:
            f.write(output_original)


def descargarFichero(idFichero, idOrigen, token):
    """Recupera un fichero del sistema. Tras ser descargado, es verificada 
    la firma y, después, descifrado el contenido.

    Args:
        idFichero (String): Este ID se genera en la llamada a upload, y debe ser comunicado al receptor
        idOrigen (String): id del destinatario para obtener clave pública y comprobar la firma
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    print("Descargando fichero " + idFichero + " de SecureBox...", end="")

    url = 'https://vega.ii.uam.es:8080/api/files/download'
    args = {"file_id": idFichero}
    headers = {"Authorization": "Bearer " +
               token, "content-type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))

    if r.status_code != 200:
        ret = json.loads(r.content.decode('utf-8'))
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) +
              "\nAPI error code: " + str(ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("OK")
        ficheroRecibido = r.content

        print("-> "+str(len(ficheroRecibido)) +
              " bytes descargados correctamente")

        # Se descifra y comprueba la firma
        mensaje = desencriptarFichero(ficheroRecibido, idOrigen, token)
        if(mensaje == -1):
            return -1

        # Tras analizar la respuesta, hemos visto que el nombre del archivo viene en su cabecera,
        # muy útil para identificar la extensión y mantener el nombre original del archivo
        name = str(
            (r.headers['Content-Disposition'].split("filename=\"", 1)[1])[:-1])

        # Se descargará el archivo en el directorio Descargas o Downloads del usuario
        try:
            path = os.path.expanduser("~")+"/Descargas/" + name
            with open(path, "wb") as f:
                f.write(mensaje)
        except (FileExistsError, FileNotFoundError):
            try:
                path = os.path.expanduser("~")+"/Downloads/" + name
                with open(path, "wb") as f:
                    f.write(mensaje)
            except (FileExistsError, FileNotFoundError):
                print("No se encuentra el directorio de descargas, por favor introduzca " +
                      "el directorio para guardar los archivos descargados en la línea 121 de ficheros.py")
                return -1

        print("Fichero descargado y verificado correctamente")


def listarFicheros(token):
    """Lista todos los ficheros pertenecientes al usuario

    Args:
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    print("Lista de ficheros propios subidos a SecureBox:")

    url = 'https://vega.ii.uam.es:8080/api/files/list'
    headers = {"Authorization": "Bearer " +
               token, "content-type": "application/json"}

    r = req.post(url=url, headers=headers)
    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        for fileid in ret['files_list']:
            print("- "+str(fileid))
        print(str(ret['num_files']) + " ficheros encontrados")


def borrarFichero(idFichero, token):
    """Borra un fichero del sistema.

    Args:
        idFichero (String): ID del fichero a borrar
        token (String): token almacenada en pc

    Returns:
        int -1: En caso de error
    """
    print("Borrando fichero " + idFichero)

    url = 'https://vega.ii.uam.es:8080/api/files/delete'
    args = {"file_id": idFichero}
    headers = {"Authorization": "Bearer " +
               token, "content-type": "application/json"}

    r = req.post(url=url, headers=headers, data=json.dumps(args))
    ret = json.loads(r.content.decode('utf-8'))

    if r.status_code != 200:
        print("Error en SecureBox:\nHttp error code: " + str(ret['http_error_code']) + "\nAPI error code: " + str(
            ret['error_code']) + "\nDescription: " + str(ret['description']))
        return -1
    else:
        print("Fichero con ID " +
              str(ret['file_id']) + " borrado correctamente")
