# Redes II. Práctica 2: Seguridad y criptografía
## Juan Luis Sanz Calvar y Sofia Xiaofan Fernandez
## Grupo 2391 - P14

## Introducción
En esta práctica se ha desarrollado el uso de un servicio de almacenamiento
llamado SecureBox que sirve tanto como repositorio de identidades como
almacén de archivosy se ha desarrollado un cliente de línea de comandos que 
usa este servicio mediante los comandos indicados más abajo (también disponibles
usando la flag --help o -h).

**Importante:** Para el correcto funcionamiento con el servicio SecureBox, se debe tener guardado el token de usuario en un fichero llamado 'token.txt' en el mismo directorio que los ficheros de código base. Este token se puede obtener
en el sitio https://vega.ii.uam.es:8080/.

**Argumentos opcionales:**

*Gestión de usuarios e identidades:*

 * --create_id CREATE_ID [CREATE_ID ...]
                        Generar un ID único para el usuario. Uso --create_id
                        <"nombre"> <"email">.
 * --search_id SEARCH_ID [SEARCH_ID ...]
                        Busca un usuario en el repositorio de identidades de
                        SecureBox introduciendo el nombre o el email como
                        argumento.
 * --delete_id DELETE_ID [DELETE_ID ...]
                        Borra la identidad con ID id registrada en el sistema.
                        Sólo se pueden borrar aquellas identidades creadas por
                        el usuario que realiza la llamada.

*Cifrado y firma de ficheros local:*

 * --encrypt ENCRYPT [ENCRYPT ...]
                        Cifra un fichero, de forma que sólo puede ser
                        descifrado por otro usuario en concreto. Introducir
                        nombre del fichero como argumento así como --dest_id
                        id del destinatario.
 * --dest_id DEST_ID [DEST_ID ...]
                        ID del receptor del fichero.
 * --sign SIGN [SIGN ...]
                        Firma digitalmente un fichero.
 * --enc_sign ENC_SIGN [ENC_SIGN ...]
                        Firma digitalmente y cifra un fichero para un usuario
                        en concreto. Introducir nombre del fichero como
                        argumento así como --dest_id id del destinatario.

*Subida y descarga de ficheros:*

 * --upload UPLOAD [UPLOAD ...]
                        Envia un fichero firmado y cifrado a otro usuario,
                        cuyo ID es especificado con la opción --dest_id.
 * --download DOWNLOAD [DOWNLOAD ...]
                        Recupera un fichero con ID id_fichero del sistema
                        subido por el emisor cuyo ID es especificado con la
                        opción --source_id. Tras ser descargado, la firma es
                        verificada y, después, se descifra el contenido.
 * --source_id SOURCE_ID [SOURCE_ID ...]
                        ID del emisor del fichero.
 * --list_files         Lista todos los ficheros pertenecientes al usuario
 * --delete_file DELETE_FILE [DELETE_FILE ...]
                        Borra un fichero del sistema.


## Desarrollo Técnico
Para el desarrollo de la práctica se ha utilizado el editor de código Visual
Studio Code y se ha usado la extensión LiveShare para poder trabajar ambos
miembros del equipo de forma simultánea sobre el directorio local de uno de ellos. 
Se ha distribuido el código en distintos archivos de forma organizada:
* *securebox_client.py:* Programa principal que contiene el main. Este programa leerá los argumentos introducidos y llamará a las funciones pertinentes en los otros archivos.
* *usuarios_identidades.py:* En este archivo se guardan todas las funciones referentes a la gestión del repositorio de identidades y usuarios.
* *cifrado_firma.py:* En este archivo se guardan todas las funciones referentes al cifrado y firma digital de los archivos para subir al servicio SecureBox así como las funciones de descifrado y comprobación de firma de los archivos recibidos.
* *ficheros.py:* En este archivo se guardan todas las funciones referentes a la gestión de ficheros con el servicio SecureBox.

**Decisiones tomadas:**
*Directorio de descargas*
En el enunciado de la práctica no se indica en qué directorio se deben guardar los ficheros descargados del servicio SecureBox, por lo que hemos decidido que el mejor sitio será en la carpeta de *$HOME/Descargas/* (o *$HOME/Downloads/*, en el caso de tener el SO en inglés) del dispositivo donde se descargan, al igual que haría un navegador común al descargar un archivo. Así también evitamos sobreescribir un fichero con el mismo nombre en el directorio raíz del programa. En la función *descargarFichero* de *ficheros.py* se obtiene ese directorio, pero al existir la posibilidad de que se haya cambiado el nombre del mismo, puede ser que se necesite cambiar esa línea (en ese caso saltará un error que lo indicará).

*Mantener archivos originales al subirlos*
En nuestra primera implementación, cuando se iba a subir un archivo al servicio SecureBox, se firmaba en un archivo aparte llamado *firmaDigital.bin* y después esa firma junto con el contenido original del archivo se cifraba sobre el mismo archivo original para después subirse (cuando se terminaba el proceso se borraba el archivo con la firma digital). Esto hacía que el archivo original ya no se pudiera leer de nuevo ya que estaba cifrado y se convirtiera en un archivo inservible para el usuario que lo había subido (la única forma de recuperar el original sería que el usuario al que se lo hemos enviado lo subiera de nuevo con nuestro usuario como destino). Para evitar eso, creamos un archivo auxiliar donde volcamos todo el contenido y la firma y el cifrado lo hacíamos sobre ese fichero auxiliar manteniendo el original. El problema de eso es que no podíamos usar el mismo nombre que el archivo original ya que es un archivo diferente, entonces al subirlo al servicio SecureBox se subía con un nombre distinto. Para ya solventar este último problema, decidimos crear un archivo auxiliar donde volcar el contenido original, después hacer el cifrado y firma sobre el archivo original y subirlo, y después volver a volcar la copia contenida en el archivo auxiliar en el original y borrar ese archivo auxiliar. Ahora al subir un archivo, se mantiene el mismo en el directorio raíz del programa.


## Conclusiones
### Técnicas
Se han desarrollado todos los aspectos exigidos en la práctica de forma correcta, y todos ellos son completamente funcionales.
Se ha dado formato al código siguiendo el auto-formateador de python3 llamado
autopep8.

### Personales
En esta práctica hemos mejorado en el uso del repositorio de gitLab, usando correctamente el archivo *.gitignore* y los branches, ya que en la primera práctica fue bastante más caótico.
Estamos muy contentos con el resultado, ya que a pesar de todos los problemas que hemos tenido, hemos conseguido solventarlos bien y realizar una práctica limpia, funcional y correcta.
Hemos aprendido sobre el uso de todos estos medios de seguridad criptográficos y hemos aplicado los conocimientos obtenidos sobre los esquemas híbridos de criptogrfafía aprendidos en las clases teóricas.
Para los próximos años estaría bien que se ofrecieran más recursos de apoyo, como archivos de código estructurados, ya que nos costó bastante empezar de cero.