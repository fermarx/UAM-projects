'''
Practica 3 Redes 2

thread.py: 
Crea diferentes hilos que la aplicación utilizará

'''

import logging
import threading
import time
import ctypes
import cv2
import socket
from datetime import *
from practica3_client import *
from servidor_descubrimiento import *


VIDEO = ('127.0.0.1',PORT_UDP) # URL dispositivo actual video
LLAMADAS = ('127.0.0.1',PORT_CONTROL) # URL dispositivo actual llamadas
BUFFER = 8111


def thread_function(name):
    logging.info("Thread %s: starting", name)
    time.sleep(2)
    logging.info("Thread %s: finishing", name)
    format = "%(asctime)s: %(message)s"


class thread_entrante(threading.Thread):

    def __init__(self, name, w): 
        threading.Thread.__init__(self) 
        self.name = name
        self.w = w

    def get_id(self): 
        if hasattr(self, '_thread_id'): 
            return self._thread_id 
        for id, thread in threading._active.items(): 
            if thread is self: 
                return id

    def raise_exception(self): 
        thread_id = self.get_id() 
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,ctypes.py_object(SystemExit)) 
        if res > 1: 
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
            print('Exception raise failure\n') 

    def run(self):
        global THREAD1, THREAD2
        print('Hilo de espera de llamadas activo...\n')
        # Abrimos el puerto de llamadas, el del server
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind(LLAMADAS)
            s.listen(1)
            # Esperamos a recibir una llamada
            llamando = False
            while True: 
                conn, addr = s.accept()
                if conn:
                    solicitud = conn.recv(1024)
                    if solicitud != '':
                        if llamando == True:
                            if solicitud.decode()[:8] == "CALL_END":
                                # El receptor solicita finalizar la llamada
                                print("Llamada finalizada.\n")
                                respuesta = 'OK_CALL_END' + MY_USER
                                # Terminamos los hilos de envio y recepcion de frames 
                                THREAD1.raise_exception() 
                                THREAD1.join()
                                THREAD1.raise_exception()
                                THREAD1.join()

                            elif solicitud.decode()[:8] == "CALL_HOLD":
                                print("Llamada en pausa, espere a que el usuario vuelva a reanudarla.\n")
                                threading.Lock().acquire()
                            elif solicitud.decode()[:8] == "CALL_RESUME":
                                print("Llamada en pausa, espere a que el usuario vuelva a reanudarla.\n")
                                threading.Lock().release()

                            else:
                                # Usuario ya en llamada, se envia CALL_BUSY al llamante
                                print("Usuario ocupado.\n")
                                respuesta = 'CALL_BUSY ' + MY_USER
                            conn.send(respuesta.encode())

                        else:
                            # Usuario no en llamada, se podra aceptar la llamada
                            solicitud = solicitud.decode()
                            print('Llamada entrante: ' + solicitud)

                            # Sacamos el nick y el puerto del usuario que nos llama
                            i = 8
                            user_nick = ''
                            while solicitud[i] != ' ':
                                user_nick += solicitud[i]
                                i += 1

                            user_port = solicitud[i+1:]
                            
                            if call_accept(user_nick) == True:
                                respuesta = 'CALL_ACCEPTED ' + MY_USER + ' ' + str(PORT_UDP)
                                conn.send(respuesta.encode())

                                # Iniciamos el hilo UDP de la recepcion de frames
                                THREAD1 = thread_receive('Receive Video Call Thread', self.w) 
                                THREAD1.daemon = True
                                THREAD1.start()

                                # Iniciamos el hilo UDP de envio de frames
                                file=None
                                THREAD2 = thread_send('Send Video Call Thread', user_nick, user_port, file, self.w) 
                                THREAD2.daemon = True
                                THREAD2.start()

                                llamando = True

                            else:
                                respuesta = 'CALL_DENIED ' + MY_USER
                                conn.send(respuesta.encode())   


class thread_receive(threading.Thread):

    def __init__(self, name, w): 
        threading.Thread.__init__(self) 
        self.name = name
        self.w = w

    def get_id(self): 
        if hasattr(self, '_thread_id'): 
            return self._thread_id 
        for id, thread in threading._active.items(): 
            if thread is self: 
                return id

    def raise_exception(self): 
        thread_id = self.get_id() 
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,ctypes.py_object(SystemExit)) 
        if res > 1: 
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
            print('Exception raise failure\n') 

    def run(self):
        print('Hilo para recibir llamada listo...\n')

        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            # Iniciamos el servidor de recepcion
            s.bind(VIDEO)

            while True:             
                data = b'' 
                # Obtenemos los mensajes con los frames
                while True:
                    data = s.recv(BUFFER)
                    if data:
                        # Separamos la cabecera del frame por el separador '#'
                        data_split = data.split(b'#',4)

                        # Descompresión de los datos, una vez recibidos
                        frame = cv2.imdecode(np.frombuffer(data_split[4],np.uint8), 1)
                        frame = cv2.resize(frame, (640, 480))
                        # Conversión de formato para su uso en el GUI
                        cv2_im = cv2.cvtColor(frame,cv2.COLOR_BGR2RGB)
                        img_tk = ImageTk.PhotoImage(Image.fromarray(cv2_im))
                        self.w.app.setImageData("thread_receive", img_tk)


class thread_send(threading.Thread):
    
    def __init__(self, name, receptor_port, receptor_ip, file, w): 
        threading.Thread.__init__(self) 
        self.name = name
        self.receptor_ip = receptor_ip
        self.receptor_port = receptor_port
        self.file = file
        self.w = w

    def get_id(self): 
        if hasattr(self, '_thread_id'): 
            return self._thread_id 
        for id, thread in threading._active.items(): 
            if thread is self: 
                return id

    def raise_exception(self): 
        thread_id = self.get_id() 
        res = ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id,ctypes.py_object(SystemExit)) 
        if res > 1: 
            ctypes.pythonapi.PyThreadState_SetAsyncExc(thread_id, 0) 
            print('Exception raise failure\n') 
    
    def run(self):
        # Capturamos la camara 
        if self.file != None:
            self.w.cap = cv2.VideoCapture(self.file)
        else:
            self.w.cap = cv2.VideoCapture(0)
        self.w.app.setPollTime(20)

        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            # Vamos enviando frame a frame el video
            i = 1
            while True:
                frame = None
                header = None
                data = b''
                
                ret, frame = self.w.cap.read() # lectura de un frame de vídeo
                frame = cv2.resize(frame, (640, 480))
                cv2_im = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                img_tk = ImageTk.PhotoImage(Image.fromarray(cv2_im))

                # Lo mostramos en el GUI
                self.w.app.setImageData("thread_send", img_tk)

                # Compresión JPG al 50% de resolución (se puede variar)
                encode_param = [cv2.IMWRITE_JPEG_QUALITY,50]
                result,encimg = cv2.imencode('.jpg',frame,encode_param)
                if result == False: 
                    print('Error al codificar imagen')
                encimg = encimg.tobytes()

                # Los datos "encimg" ya están listos para su envío por la red                
                header = (str(i) + '#' + str(datetime.now()) + '#' + '640x480' + '#' + '20' + '#').encode()   
                data = header + encimg
                s.sendto(data, (self.receptor_ip, int(self.receptor_port)))
                i += 1
