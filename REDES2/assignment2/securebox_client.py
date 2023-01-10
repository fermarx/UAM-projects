import argparse
import sys
from usuarios_identidades import *
from ficheros import *
from cifrado_firma import *


def getToken():
    """Get the token of the actual user saved in 'token.txt' file

    Returns:
        String: token of the user
    """
    token = None
    try:
        f = open('token.txt', 'r')
        token = f.read()
        f.close()

        if len(token) == 0:
            token = None

    except (FileNotFoundError):
        print("Por favor, antes de continuar, guarde su token asociado en un\n" +
              "fichero llamado 'token.txt'. Si no dispone del token necesario\n" +
              "puede obtenerlo en https://vega.ii.uam.es:8080")
    return token


def argSel(args):
    """Llama a la función pertinente según el argumento recibido

    Args:
        args (String Array): Argumentos de entrada
    """
    token = getToken()

    if token is None:
        print("Error: no existe token asociado a usuario")

    elif args.create_id:
        if len(args.create_id) == 2:
            registrarUsuario(args.create_id[0], args.create_id[1], token)

        elif len(args.create_id) == 3:
            "TODO: con un alias"
            print(4)

        else:
            print("Error: Los argumentos a introducir son \"nombre\" \"email\" \"alias\", siendo \"alias\" opcional (importante las comillas).")

    elif args.search_id:
        if len(args.search_id) == 1:
            buscarUsuario(args.search_id[0], token)

        else:
            print("Error: Se debe introducir un argumento, ya sea el nombre o el email del usuario a buscar (introducir entre comillas)")

    elif args.delete_id:
        if len(args.delete_id) == 1:
            borrarUsuario(args.delete_id[0], token)

        else:
            print("Error: Se debe introducir un argumento, el id del usuario a eliminar")

    elif args.encrypt:
        if not args.dest_id:
            print("Error: Se debe indicar el destinatario mediante --dest_id <id>")
        else:
            if len(args.dest_id) == 1 and len(args.encrypt) == 1:
                encriptarFichero(args.encrypt[0], args.dest_id[0], token)
            else:
                print(
                    "Error: Se deben introducir dos argumentos, el nombre del fichero a encriptar y el id del usuario destinatario")

    elif args.sign:
        if len(args.sign) == 1:
            firmarFichero(args.sign[0])
        else:
            print(
                "Error: Se debe introducir un argumento, el nombre del fichero a firmar")

    elif args.enc_sign:
        if not args.dest_id:
            print("Error: Se debe indicar el destinatario mediante --dest_id <id>")
        else:
            if len(args.dest_id) == 1 and len(args.enc_sign) == 1:
                encriptarFirmarFichero(
                    args.enc_sign[0], args.dest_id[0], token)
            else:
                print("Error: Se deben introducir dos argumentos, el nombre del fichero a firmar y encriptar y el id del usuario destinatario")

    elif args.upload:
        if not args.dest_id:
            print("Error: Se debe indicar el destinatario mediante --dest_id <id>")
        else:
            if len(args.dest_id) == 1 and len(args.upload) == 1:
                subirFichero(args.upload[0], args.dest_id[0], token)
            else:
                print(
                    "Error: Se deben introducir dos argumentos, el nombre del fichero a subir y el id del usuario destinatario")

    elif args.download:
        if not args.source_id:
            print("Error: Se debe indicar el emisor mediante --source_id <id>")
        else:
            if len(args.source_id) == 1 and len(args.download) == 1:
                descargarFichero(args.download[0], args.source_id[0], token)
            else:
                print(
                    "Error: Se deben introducir dos argumentos, el id del fichero a descargar y el id del usuario emisor")

    elif args.list:
        listarFicheros(token)

    elif args.delete_file:
        if len(args.delete_file) == 1:
            borrarFichero(args.delete_file[0], token)
        else:
            print("Error: Se debe introducir un argumento, el id del fichero a borrar")


def main():
    parser = argparse.ArgumentParser(
        description='Hacer cosas con el servidor.')

    parser.add_argument('--create_id', dest='create_id', nargs='+', default=False,
                        help='Generar un ID único para el usuario. Uso --create_id <\"nombre\"> <\"email\">.')
    parser.add_argument('--search_id', dest='search_id', nargs='+', default=False,
                        help='Busca un usuario en el repositorio de identidades de SecureBox introduciendo el nombre o el email como argumento.')
    parser.add_argument('--delete_id', dest='delete_id', nargs='+', default=False,
                        help='Borra la identidad con ID id registrada en el sistema. Sólo se pueden borrar aquellas identidades creadas por el usuario que realiza la llamada.')
    parser.add_argument('--encrypt', dest='encrypt', nargs='+', default=False,
                        help='Cifra un fichero, de forma que sólo puede ser descifrado por otro usuario en concreto. Introducir nombre del fichero como argumento así como --dest_id id del destinatario.')
    parser.add_argument('--dest_id', dest='dest_id', nargs='+',
                        default=False, help='ID del receptor del fichero.')
    parser.add_argument('--sign', dest='sign', nargs='+',
                        default=False, help='Firma digitalmente un fichero.')
    parser.add_argument('--enc_sign', dest='enc_sign', nargs='+', default=False,
                        help='Firma digitalmente y cifra un fichero para un usuario en concreto. Introducir nombre del fichero como argumento así como --dest_id id del destinatario.')
    parser.add_argument('--upload', dest='upload', nargs='+', default=False,
                        help='Envia un fichero firmado y cifrado a otro usuario, cuyo ID es especificado con la opción --dest_id.')
    parser.add_argument('--download', dest='download', nargs='+', default=False,
                        help='Recupera un fichero con ID id_fichero del sistema subido por el emisor cuyo ID es especificado con la opción --source_id. Tras ser descargado, la firma es verificada y, después, se descifra el contenido.')
    parser.add_argument('--source_id', dest='source_id', nargs='+',
                        default=False, help='ID del emisor del fichero.')
    parser.add_argument('--list_files', dest='list', action='store_true', default=False,
                        help='Lista todos los ficheros pertenecientes al usuario')
    parser.add_argument('--delete_file', dest='delete_file', nargs='+',
                        default=False, help='Borra un fichero del sistema.')

    args = parser.parse_args()
    argSel(args)


if __name__ == "__main__":
    main()
