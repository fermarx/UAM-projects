'''
Practica 3 Redes 2

practica·_client.py: 
Inicia la GUI de la aplicación. Conecta al cliente con el servidor.

'''

# import the library
from appJar import gui
from PIL import Image, ImageTk
from servidor_descubrimiento import *
from thread import *
import numpy as np
import cv2
import time


class VideoClient(object):

    def __init__(self, window_size):

        # Creamos una variable que contenga el GUI principal
        self.app = gui("Redes2 - P2P", window_size)
        self.app.setGuiPadding(10, 10)

        # Preparación del interfaz
        self.app.addLabel("title", "Cliente Multimedia P2P - Redes2 ")
        self.app.addImage("video", "imgs/webcam.gif")

        # Registramos la función de captura de video
        # Esta misma función también sirve para enviar un vídeo
        #self.cap = cv2.VideoCapture(0)
        self.app.setPollTime(20)
      #  self.app.registerEvent(self.capturaVideo)

        # Añadir los botones
        self.app.addButtons(["Conectar",
                             "Mandar Video",
                             "Salir",], 
                             self.buttonsCallback)

        self.app.addButtons(["Registrarse",
                             "Listar usuarios",
                             "Consultar informacion",], 
                             self.buttonsCallback)
                             
        self.app.addButtons(["Colgar",
                             "Hold",
                             "Resume"], 
                             self.buttonsCallback)

        # Barra de estado
        # Debe actualizarse con información útil sobre la llamada (duración, FPS, etc...)
        self.app.addStatusbar(fields=2)

        self.nick_receptor = None
        self.paused = False
        
        # Hilo de llamadas entrantes
        global THREAD
        THREAD = thread_entrante('Waiting Calls Thread', self) 
        THREAD.daemon = True
        THREAD.start()


    def start(self):
        self.app.go()


    # Establece la resolución de la imagen capturada
    def setImageResolution(self, resolution):
        # Se establece la resolución de captura de la webcam
        # Puede añadirse algún valor superior si la cámara lo permite
        # pero no modificar estos
        if resolution == "LOW":
            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 160)
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 120)
        elif resolution == "MEDIUM":
            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
        elif resolution == "HIGH":
            self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
            self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)


    # Función que gestiona los callbacks de los botones
    def buttonsCallback(self, button):
        """Gestiona los botones de la gui

        Args:
            button (String): nombre del boton pulsado
        """

        if button == "Salir":
            # Cerramos conexion con el server
            quit_server()
            # Salimos de la aplicación
            self.app.stop()


        elif button == "Conectar":
            global THREAD1, THREAD2

            # Entrada del nick del usuario a conectar
            nick = self.app.textBox("Conexión",
                                    "Introduce el nick del usuario a buscar")
            if not query(nick):
                self.app.infoBox("Error", "Nick introducido no existe.")
                return

            ip,response = calling(nick)
            if ip == None:
                self.app.infoBox("Error", "Usuario no conectado.")
                return

            # Informacion del usuario llamado
            res = response
            response = str(response)
            verificar = res[:9]
            verificar.decode('utf-8')
            if verificar == 'CALL_BUSY'.encode():
                print("Receptor ocupado\n")
            
            verificar = res[0:13]
            verificar.decode('utf-8')
            response = str(res)
            # Si la llamada se acepta
            if verificar == 'CALL_ACCEPTED'.encode():
                i = 16
                receptor = ''
                # Puerto del receptor
                while response[i] != ' ':
                    receptor += response[i]
                    i += 1

                receptor_port = response[i:-1]

                # Iniciamos el hilo UDP de la recepcion de frames
                THREAD1 = thread_receive('Receive Thread', self) 
                THREAD1.daemon = True
                THREAD1.start()

                # Iniciamos el hilo UDP de envio de frames
                file = None
                THREAD2 = thread_send('Send Thread', receptor_port, ip, file, self) 
                THREAD2.daemon = True
                THREAD2.start()

                self.nick_receptor = receptor


        elif button == "Mandar Video":
            # Entrada del nick del usuario a conectar
            nick = self.app.textBox("Conexión",
                                    "Introduce el nick del usuario a buscar")
            if not query(nick):
                self.app.infoBox("Error", "Nick introducido no existe.")
                return

            ip,response = calling(nick)
            if ip == None:
                self.app.infoBox("Error", "Usuario no conectado.")
                return

            # Informacion del usuario llamado
            res = response
            response = str(response)
            verificar = res[:9]
            verificar.decode('utf-8')
            if verificar == 'CALL_BUSY'.encode():
                print("Receptor ocupado\n")
            
            verificar = res[0:13]
            verificar.decode('utf-8')
            response = str(res)
            # Si la llamada se acepta
            if verificar == 'CALL_ACCEPTED'.encode():
                i = 16
                receptor = ''
                # Puerto del receptor
                while response[i] != ' ':
                    receptor += response[i]
                    i += 1

                receptor_port = response[i:-1]

                # Iniciamos el hilo UDP de la recepcion de frames
                THREAD1 = thread_receive('Receive Thread', self) 
                THREAD1.daemon = True
                THREAD1.start()

                # Iniciamos el hilo UDP de envio de frames
                file = self.app.textBox("Fichero a mandar",
                        "Introduce del fichero que se desea mandar")
                THREAD2 = thread_send('Send Thread', receptor_port, ip, file, self) 
                THREAD2.daemon = True
                THREAD2.start()

                self.nick_receptor = receptor


        elif button == "Colgar":
            if self.nick_receptor:
                call_end(self.nick_receptor)
                print("LLAMADA COLGADA con éxito.\n")
                # Cerramos el cliente
                self.app.stop()


        elif button == "Registrarse":
            # Registramos usuario
            nick = self.app.textBox(
                "Registro", "Introduce tu nick de usuario:")
            if query(nick):
                self.app.infoBox("Error en el registro", "Nick en uso.")
                return
            pwd = self.app.textBox(
                "Registro", "Introduce tu contraseña de usuario:")
            if nick:
                respuesta = register(nick, pwd)
                if(respuesta):
                    self.app.infoBox("Registro completo", "Registro realizado con éxito.")
                else:
                    self.app.infoBox("Error en el registro", "Por favor, intentelo de nuevo.")


        elif button == "Listar usuarios":
            users = list_users()
            if(users):
                self.app.infoBox("Usuarios en el sistema", users)
            else:
                self.app.infoBox("Usuarios en el sistema", "Error al cargar datos.")


        elif button == "Consultar informacion":
            nick = self.app.textBox("Consultar Informacón", "Introduce el nick del usuario:")
            if nick:
                user_data = query(nick)
                if (user_data):
                    self.app.infoBox("Informacion de usuario", user_data)
                else:
                    self.app.infoBox("Error", "Nick no encontrado")


        elif button == "Hold":
            if self.nick_receptor:
                call_on_hold(self.nick_receptor)
                self.paused = True  # Cambiamos el flag de pausa


        elif button == "Resume":
            if self.nick_receptor:
                call_resume(self.nick_receptor)
                self.paused = True  # Cambiamos el flag de pausa



if __name__ == '__main__':

    # Nuevo usuario por defecto
    response = register(MY_USER, MY_PWD)

    # En espera de llamada en paralelo
    verificacion = response[0:2]
    verificacion.decode('utf-8')

    if verificacion:
        print("Logged as: " + MY_USER)
        vc = VideoClient("640x520")

        # Lanza el bucle principal del GUI
        # El control ya NO vuelve de esta función, por lo que todas las
        # acciones deberán ser gestionadas desde callbacks y threads
        vc.start()
