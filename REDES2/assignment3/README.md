# Redes II. Práctica 3: Multimedia
## Juan Luis Sanz Calvar y Sofia Xiaofan Fernandez
## Grupo 2391 - P14

## Introducción

Para esta última práctica hemos tenido que construir una aplicación que nos permitiese transmitir video, sin audio, entre dos clientes con acceso a el servidor vega.ii.uam.es.

La aplicacion sigue un protoclolo P2P y se ha desarrollado una version básica donde se comple:
* Comunicación unicast entre dos pares simultaneos.
* Transmisión en tiempo real de imágenes desde una webcam
* Transmisión de un fichero de video.
* Operaciones básicas: PLAY y PAUSE del flujo multimedia.

**Nota:** Para ejecutar esta práctica no es necesario ningún compilado previo ya que se realiza sobre Python3. Se puede ejecutar con el siguiente comando:
> python3 practica3_client.py

Para el correcto funcionamiento de esta práctica se deberá inlcuir una carpeta con un archivo .gif llamado webcam y el fichero de video que se desea mandar. Así mismo, se necesita la carpeta appJar encontrada en https://github.com/jarvisteach/appJar



## Desarrollo Técnico
Para el desarrollo de esta práctica también hemos utilizado el editor de código Visual Studio Code con la extensión LiveShare para poder trabajar ambos.
Se ha distribuido el código en los siguientes archivos:
* *practica3_client.py:* Fichero principal con el código esqueleto dado por los profesores y extendido.
Aquí tambien se maneja la interfaz (appJar) y la cámara (cv2).
La clase VideoClient inicia la interfaz la ventana con la que el cliente va a interactuar. Una vez inicias la applicación, el cliente se registra automaticamente, aunque luego pueda registrarse si lo desea.
Tras iniciar la aplicación, aparecerá el video recibido y botones de acción. Dichos botones se manejan en "buttonsCallBack":
    - Conectar: Crea el hilo. (Véase thread.py)
    - Mandar Video: Manda un fichero de video creando un hilo. (Véase thread.py)
    - Registrarse: Registrarse en la aplicación
    - Lista Usuarios: Visualiza la lista de usuarios en vega.ii.uam.es
    - Consultar información: Muestra la información de cualquier usuario regustrado en vega.ii.uam.es
    - Salir: Cierra la aplicación.
    - Colgar: Cuelga la llamada en la que se encuentra.


* *servidor_descubrimiento.py:* Fichero encargado de manejar las funciones pedidas. Estas funciones son llamadas desde el fichero practica3_client.py, en la funcion "buttonsCallBack", cada uno en su correspondiente acción.
    - connect_DS(): Se conecta al servidor vega.ii.uam.es:8000
    - register(nick, password): Con el nick y la contraseña proporcionada por el usuario, se registra en el servidor.
    - getIP(): Devuelve la IP del usuario
    - query(nick): Devuelve la información pedida de un usuario ya formateada.
    - list_users(): Devuelve una lista de todos los usuarios 
    - quit_server(): Sale del servidor.
    - calling(user): Llama a un usuario dado por el cliente.
    - call_end(user): Termina una llamada.
    - call_accept(user): Si el protocolo del usuario al que se está llamando es el mismo que el protocolo utilizado por el cliente, se acepta la llamda.
    - call_on_hold(user): Pone a la llamada en espera.
    - call_resume(user): Reanuda la llamada puesta en espera.


* *thread.py:* Fichero donde se manejan los hilos.
    - thread_entrante(threading.Thread): Espera a peticiones entrantes de llamadas y controla CALL_ACCEPTED y CALL_END. Este hilo es el primero en ser iniciado una vez la apliación se ha ejecutado. Se queda esperando llamadas entrantes y antes de aceptarlas, combruba que el usuario no esté ocupado y si tienen el mismo protocolo. Si la acepta, le indica el puerto UDP por el que recibbirá los frames. e inicia los hilos de recibir y enviar los frames.
    - thread_receive(threading.Thread): Hilo encargado de recibir los frames de otro usuario. Primero separa la cabecera del frame y lo descomprime. Lo envía a la pantalla del usuario para que este pueda verlo y termina si recibe un CALL_END
    - thread_send(threading.Thread): Hilo encargado de enviar los frames a otro usuario. Este hilo captura frame a frame la camara del usuario. También es el encargado de enviar un video frame a frame mediante una ocnexión UDP. Los frames son comprimidos un 50% y se le ponen una cabecera delante con información usada en thread_receive(). El hilo termina cuando un usuario decide terminar la llamada.

    Los hilos, al ser demonios, si se finaliza el proceso principal, estos también termina. 

## Conclusiones
### Técnicas
En esta práctica hemos tardado más de lo previsto ya que hemos tenido que documentarnos bastante sobre OpenCV y appJar. 
A pesar de estp, el lenguaje de programación nos ha gustado bastante ya que nos ha permitido programar algunas partes de manera más rápida y sencilla en comparación con otras prácticas.
También nos costó bastante entender el manejo de hilos y el como manejarlos.
Por problemas de tiempo en el último momento, no se ha probado las funciones "call_on_hold" y "call_resume" no han podido ser probadas por lo que no se sabe si funcionan correctamente.

### Personales
Para esta práctica nos hemos organizado correctamente y nos ha parecido más fluida sobre todo tras nuestra práctica 1, la cual nos costó bastante, ya que teníamos el repositorio muy sucio.
Ver que había funciones para los sockets ya creadas nos alivió bastante.
