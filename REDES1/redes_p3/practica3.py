#!/usr/bin/python
'''
    practica3.py
    Programa principal que realiza el análisis de tráfico sobre una traza PCAP.
    Autor: Javier Ramos <javier.ramos@uam.es>
    2020 EPS-UAM
'''


import sys
import argparse
from argparse import RawTextHelpFormatter
import time
import logging
import shlex
import subprocess
import pandas as pd
from io import StringIO
import os
import warnings
warnings.filterwarnings("ignore")
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
from decimal import *

'''
    Función: calcularECDF
    Entrada: 
        -datos: lista con los datos sobre los que calcular la ECDF
    Salida: :
        -datos: lista con los valores x (datos de entrada)
        -y: lista con los valores de probabilidad acumulada para cada dato de entrada
    Descripción:  Esta función recibe una lista de datos y calcula la función empírica de distribución 
    acumulada sobre los mismos. Los datos se devuelven listos para ser pintados.
'''
def calcularECDF(datos):
    datos.sort()
    n = len (datos)
    y = [(i-1)/n for i in range(1,n+1)]

    return datos,y



'''
    Función: ejecutarComandoObtenerSalida
    Entrada: 
        -comando: cadena de caracteres con el comando a ejecutar
    Salida: 
        -codigo_retorno: código numérico que indica el retorno del comando ejecutado.
        Si este valor es 0, entonces el comando ha ejecutado correctamente.
        -salida_retorno: cadena de caracteres con el retorno del comando. Este retorno
        es el mismo que obtendríamos por stdout al ejecutar un comando de terminal.

    Descripción: Esta función recibe una cadena con un comando a ejecutar, lo ejecuta y retorna
    tanto el código de resultado de la ejecución como la salida que el comando produzca por stdout
'''
def ejecutarComandoObtenerSalida(comando):
    proceso = subprocess.Popen(shlex.split(comando), stdout=subprocess.PIPE)
    salida_retorno = ''
    while True:
        
        salida_parcial = proceso.stdout.readline()
        if salida_parcial.decode() == '' and proceso.poll() is not None:
            break
        if salida_parcial:
            salida_retorno += salida_parcial.decode()
    codigo_retorno = proceso.poll()
    return codigo_retorno,salida_retorno


'''
    Función: pintarECDF
    Entrada:
        -datos: lista con los datos que se usarán para calcular y pintar la ECDF
        -nombre_fichero: cadena de caracteres con el nombre del fichero donde se guardará la imagen
        (por ejemplo figura.png)
        -titulo: cadena de caracteres con el título a pintar en la gráfica
        -titulo_x: cadena de caracteres con la etiqueta a usar para el eje X de la gráfica
        -titulo_y: cadena de caracteres con la etiqueta a usar para el eje Y de la gráfica
    Salida: 
        -Nada

    Descripción: Esta función pinta una gráfica ECDF para unos datos de entrada y la guarda en una imagen
'''
def pintarECDF(datos,nombre_fichero,titulo,titulo_x,titulo_y):
    
    x, y = calcularECDF(datos)
    x.append(x[-1])
    y.append(1) 
    fig1, ax1 = plt.subplots()
    plt.step(x, y, '-')
    _ = plt.xticks(rotation=45)
    plt.title(titulo)
    fig1.set_size_inches(12, 10)
    plt.tight_layout()
    plt.locator_params(nbins=20)
    ax1.set_xlabel(titulo_x)
    ax1.set_ylabel(titulo_y)
    plt.savefig(nombre_fichero, bbox_inches='tight')


'''
    Función: pintarSerieTemporal
    Entrada:
        -x: lista de tiempos en formato epoch y granularidad segundos
        -y: lista con los valores a graficar
        -nombre_fichero: cadena de caracteres con el nombre del fichero donde se guardará la imagen
        (por ejemplo figura.png)
        -titulo: cadena de caracteres con el título a pintar en la gráfica
        -titulo_x: cadena de caracteres con la etiqueta a usar para el eje X de la gráfica
        -titulo_y: cadena de caracteres con la etiqueta a usar para el eje Y de la gráfica
    Salida: 
        -Nada

    Descripción: Esta función pinta una serie temporal dados unos datos x e y de entrada y la guarda en una imagen
'''
def pintarSerieTemporal(x,y,nombre_fichero,titulo,titulo_x,titulo_y):
   
    fig1, ax1 = plt.subplots()
    plt.plot(x, y, '-')
    _ = plt.xticks(rotation=45)
    plt.title(titulo)
    fig1.set_size_inches(12, 10)
    plt.gcf().autofmt_xdate()
    plt.gca().xaxis.set_major_locator(mtick.FixedLocator(x))
    plt.gca().xaxis.set_major_formatter(mtick.FuncFormatter(lambda pos,_: time.strftime("%d-%m-%Y %H:%M:%S",time.localtime(pos))))
    plt.tight_layout()
    plt.locator_params(nbins=20)
    ax1.set_xlabel(titulo_x)
    ax1.set_ylabel(titulo_y)
    plt.savefig(nombre_fichero, bbox_inches='tight')


'''
    Función: pintarTarta
    Entrada:
        -etiquetas: lista con cadenas de caracteres que contienen las etiquetas a usar en el gráfico de tarta
        -valores: lista con los valores a graficar
        -nombre_fichero: cadena de caracteres con el nombre del fichero donde se guardará la imagen
        (por ejemplo figura.png)
        -titulo: cadena de caracteres con el título a pintar en la gráfica
        
    Salida: 
        -Nada

    Descripción: Esta función pinta un gráfico de tarta dadas unas etiquetas y valores de entrada y lo guarda en una imagen
'''
def pintarTarta(etiquetas,valores,nombre_fichero,titulo):
  
    explode = tuple([0.05]*(len(etiquetas)))
 
    fig1, ax1 = plt.subplots()
    plt.pie(valores, autopct='%1.1f%%', startangle=90, pctdistance=0.85)
    plt.legend(etiquetas, loc="best")
    plt.title(titulo)
    centre_circle = plt.Circle((0,0),0.70,fc='white')
    fig1 = plt.gcf()
    fig1.gca().add_artist(centre_circle)
    fig1.set_size_inches(12, 10)
    ax1.axis('equal')  
    plt.tight_layout()
    plt.savefig(nombre_fichero, bbox_inches='tight')

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Programa principal que realiza el análisis de tráfico sobre una traza PCAP',
    formatter_class=RawTextHelpFormatter)
    parser.add_argument('--trace', dest='tracefile', default=False,help='Fichero de traza a usar',required=True)
    parser.add_argument('--mac', dest='mac', default=False,help='MAC usada para filtrar',required=True)
    parser.add_argument('--ip_flujo_tcp', dest='ip_flujo_tcp', default=False,help='IP para filtrar por el flujo TCP',required=True)
    parser.add_argument('--port_flujo_udp', dest='port_flujo_udp', default=False,help='Puerto para filtrar por el flujo UDP',required=True)
    parser.add_argument('--debug', dest='debug', default=False, action='store_true',help='Activar Debug messages')
    args = parser.parse_args()

    if args.debug:
        logging.basicConfig(level = logging.DEBUG, format = '[%(asctime)s %(levelname)s]\t%(message)s')
    else:
        logging.basicConfig(level = logging.INFO, format = '[%(asctime)s %(levelname)s]\t%(message)s')

    #Creamos un directorio a donde volcaremos los resultado e imágenes

    if not os.path.isdir('resultados'):
        os.mkdir('resultados')
  
    #Ejemplo de ejecución de comando tshark y parseo de salida. Se parte toda la salida en líneas usando el separador \n
    logging.info('Ejecutando tshark para obtener el número de paquetes')
    codigo,salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e frame.number'.format(args.tracefile))
    nlineas = 0
    for linea in salida.split('\n'):
        if linea != '':
            nlineas +=1

    print('{} paquetes en la traza {}'.format(nlineas,args.tracefile))
   
    #porcentaje de ipv4 y no ipv4
    logging.info('Ejecutando tshark para obtener el número de paquetes IPv4 y no IPv4')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e frame.number eth.type == 0x0800'.format(args.tracefile))
    n_ip = 0
    for linea in salida.split('\n'):
        if linea != '':
            n_ip +=1
    porcentaje = n_ip*100/nlineas
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e frame.number eth.type != 0x0800'.format(args.tracefile))
    n_no_ip = 0
    for linea in salida.split('\n'):
        if linea != '':
            n_no_ip +=1
    porcentaje2 = n_no_ip*100/nlineas
    print("Porcentaje de tráfico IPv4: {}\nPorcentaje de tráfico no IPv4: {}".format(porcentaje, porcentaje2))
    
    #TODO: Añadir código para obtener el porcentaje de tráfico TPC,UDP y OTROS sobre el tráfico IP
    logging.info('Ejecutando tshark para obtener el número de paquetes TCP UDP y Otros')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e tcp.port eth.type == 0x0800'.format(args.tracefile))
    n_tcp = 0
    for linea in salida.split('\n'):
        if linea != '':
            n_tcp +=1
    porcentaje = n_tcp*100/n_ip
    print("Porcentaje de tráfico TCP: {}\n".format(porcentaje))
    
    n_udp = 0
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.port eth.type == 0x0800'.format(args.tracefile))
    for linea in salida.split('\n'):
        if linea != '':
            n_udp +=1
    porcentaje = n_udp*100/n_ip
    print("Porcentaje de tráfico UDP: {}\n".format(porcentaje))
    
    n_others = 0
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.port -e tcp.port eth.type == 0x0800'.format(args.tracefile))
    for linea in salida.split('\n'):
        if linea == '' or linea == '\t':
            n_others +=1
    porcentaje = n_others*100/n_ip
    print("Porcentaje de tráfico OTROS: {}\n".format(porcentaje))


    #Obtención de top direcciones IP
    #TODO: Añadir código para obtener los datos y generar la gráfica de top IP origen por paquetes
    logging.info('Ejecutando tshark para obtener el top IPs origen por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e ip.src'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_ip_src_paquetes.dat", "w")
    for ip in lista:
        if ip != '':
            f.write("%s\n"%(ip))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_ip_src_paquetes.dat")
    f.close()
    if os.path.exists("salida_ip_src_paquetes.dat"):
        os.remove("salida_ip_src_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop ips origen y cuantos paquetes han mandado(Num paquetes, Direccion):")
    
    num = []
    ips = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        ips.append(aux[1])
    pintarTarta(ips, num, "tarta_top_src_ips_paquetes.png", "Top ips origen por paquetes")




    #TODO: Añadir código para obtener los datos y generar la gráfica de top IP origen por bytes
    logging.info('Ejecutando tshark para obtener el top IPs origen por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e ip.src -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        #1.1.1.1 1 seria el tamaño mínimo que son 9 carácteres
        if len(item) < 9:
            lista.remove(item)
        else:
            item = item.split('\t')
            total_bytes += int(item[1])
            if item[0] in res.keys():
                res[item[0]] += int(item[1])
            else:
                res[item[0]] = int(item[1])

    ips = []
    num = []
    print("\nTop IPs origen y cuantos bytes han mandado(Direccion, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        ips.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(ips, num, "tarta_top_src_ips_bytes.png", "Top ips origen por bytes")

    
   

    #TODO: Añadir código para obtener los datos y generar la gráfica de top IP destino por paquetes
    logging.info('Ejecutando tshark para obtener el top IPs destino por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e ip.dst'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_ip_dst_paquetes.dat", "w")
    for ip in lista:
        if ip != '':
            f.write("%s\n"%(ip))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_ip_dst_paquetes.dat")
    f.close()
    if os.path.exists("salida_ip_dst_paquetes.dat"):
        os.remove("salida_ip_dst_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop ips destino y cuantos paquetes han recibido(Num paquetes, Direccion):")
    
    num = []
    ips = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        ips.append(aux[1])
    pintarTarta(ips, num, "tarta_top_dst_ips_paquetes.png", "Top ips destino por paquetes")

    #TODO: Añadir código para obtener los datos y generar la gráfica de top IP destino por bytes
    logging.info('Ejecutando tshark para obtener el top IPs destino por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e ip.dst -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        #1.1.1.1 1 seria el tamaño mínimo que son 9 carácteres
        if len(item) < 9:
            lista.remove(item)
        else:
            item = item.split('\t')
            total_bytes += int(item[1])
            if item[0] in res.keys():
                res[item[0]] += int(item[1])
            else:
                res[item[0]] = int(item[1])

    ips = []
    num = []
    print("\nTop IPs destino y cuantos bytes han recibido(Direccion, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        ips.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(ips, num, "tarta_top_dst_ips_bytes.png", "Top ips destino por bytes")
   
    
    #Obtención de top puertos TCP
    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto origen TCP por bytes
    logging.info('Ejecutando tshark para obtener el top puertos TCP origen por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e tcp.srcport -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        if len(item) < 3:
            lista.remove(item)
        else:
            item = item.split('\t')
            if item[0] != '':
                total_bytes += int(item[1])
                if item[0] in res.keys():
                    res[item[0]] += int(item[1])
                else:
                    res[item[0]] = int(item[1])

    tcps = []
    num = []
    print("\nTop puertos tcp origen y cuantos bytes han mandado(Puerto, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        tcps.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(tcps, num, "tarta_top_src_tcp_bytes.png", "Top tcp origen por bytes")

    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto destino TCP por bytes
    logging.info('Ejecutando tshark para obtener el top puertos TCP destino por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e tcp.dstport -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        if len(item) < 3:
            lista.remove(item)
        else:
            item = item.split('\t')
            if item[0] != '':
                total_bytes += int(item[1])
                if item[0] in res.keys():
                    res[item[0]] += int(item[1])
                else:
                    res[item[0]] = int(item[1])

    tcps = []
    num = []
    print("\nTop puertos tcp destino y cuantos bytes han recibido(Puerto, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        tcps.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(tcps, num, "tarta_top_dst_tcp_bytes.png", "Top tcp destino por bytes")
  
    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto origen TCP por paquetes
    logging.info('Ejecutando tshark para obtener el top puertos TCP origen por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e tcp.srcport'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_tcp_src_paquetes.dat", "w")
    for tcp in lista:
        if tcp != '':
            f.write("%s\n"%(tcp))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_tcp_src_paquetes.dat")
    f.close()
    if os.path.exists("salida_tcp_src_paquetes.dat"):
        os.remove("salida_tcp_src_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop puerto tcp origen y cuantos paquetes han mandado(Num paquetes, Puerto):")
    
    num = []
    tcps = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        tcps.append(aux[1])
    pintarTarta(tcps, num, "tarta_top_src_tcp_paquetes.png", "Top tcp origen por paquetes")
 
    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto destino  TCP por paquetes
    logging.info('Ejecutando tshark para obtener el top puertos TCP destino por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e tcp.dstport'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_tcp_dst_paquetes.dat", "w")
    for tcp in lista:
        if tcp != '':
            f.write("%s\n"%(tcp))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_tcp_dst_paquetes.dat")
    f.close()
    if os.path.exists("salida_tcp_dst_paquetes.dat"):
        os.remove("salida_tcp_dst_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop puertos tcp destino y cuantos paquetes han recibido(Num paquetes, Puerto):")
    
    num = []
    tcps = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        tcps.append(aux[1])
    pintarTarta(tcps, num, "tarta_top_dst_tcp_paquetes.png", "Top tcp destino por paquetes")

    #Obtención de top puertos UDP
    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto origen UDP por bytes
    logging.info('Ejecutando tshark para obtener el top puertos UDP origen por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.srcport -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        # minimo seria 1\t1 que son 3 chars
        if len(item) < 3:
            lista.remove(item)
        else:
            item = item.split('\t')
            if item[0] != '':    
                total_bytes += int(item[1])
                if item[0] in res.keys():
                    res[item[0]] += int(item[1])
                else:
                    res[item[0]] = int(item[1])

    udps = []
    num = []
    print("\nTop puertos udp origen y cuantos bytes han mandado(Puerto, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        udps.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(udps, num, "tarta_top_src_udp_bytes.png", "Top udp origen por bytes")

    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto destino UDP por bytes
    logging.info('Ejecutando tshark para obtener el top puertos UDP destino por bytes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.dstport -e frame.len'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    res = {}
    total_bytes = 0
    for item in lista:
        if len(item) < 3:
            lista.remove(item)
        else:
            item = item.split('\t')
            if item[0] != '':
                total_bytes += int(item[1])
                if item[0] in res.keys():
                    res[item[0]] += int(item[1])
                else:
                    res[item[0]] = int(item[1])

    udps = []
    num = []
    print("\nTop puertos udp destino y cuantos bytes han recibido(Puerto, Bytes):")
    for i in range(5):
        key = max(res, key=res.get)
        print(key, res[key])
        udps.append(key)
        num.append(res[key])
        res.pop(key)
    pintarTarta(udps, num, "tarta_top_dst_udp_bytes.png", "Top udp destino por bytes")

    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto origen UDP por paquetes
    logging.info('Ejecutando tshark para obtener el top puertos UDP origen por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.srcport'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_udp_src_paquetes.dat", "w")
    for udp in lista:
        if udp != '':
            f.write("%s\n"%(udp))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_udp_src_paquetes.dat")
    f.close()
    if os.path.exists("salida_udp_src_paquetes.dat"):
        os.remove("salida_udp_src_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop puerto udp origen y cuantos paquetes han mandado(Num paquetes, Puerto):")
    
    udps = []
    udps = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        udps.append(aux[1])
    pintarTarta(udps, num, "tarta_top_src_udp_paquetes.png", "Top udp origen por paquetes")

    #TODO: Añadir código para obtener los datos y generar la gráfica de top puerto destino UDP por paquetes
    logging.info('Ejecutando tshark para obtener el top puertos UDP destino por paquetes')
    codigo, salida = ejecutarComandoObtenerSalida('tshark -r {} -T fields -e udp.dstport'.format(args.tracefile))
    lista = salida.split('\n')
    lista.sort()
    f = open("salida_udp_dst_paquetes.dat", "w")
    for udp in lista:
        if udp != '':
            f.write("%s\n"%(udp))
    codigo,salida = ejecutarComandoObtenerSalida("uniq -c salida_udp_dst_paquetes.dat")
    f.close()
    if os.path.exists("salida_udp_dst_paquetes.dat"):
        os.remove("salida_udp_dst_paquetes.dat")
    salida = salida.split('\n')
    salida.sort(reverse=True)
    print("\nTop puertos udp destino y cuantos paquetes han recibido(Num paquetes, Puerto):")
    
    num = []
    udps = []
    res = salida[:5]
    for item in res:
        while item[0] == ' ':
            item = item[1:]
        print(item)
        aux = item.split(' ')
        num.append(aux[0])
        udps.append(aux[1])
    pintarTarta(udps, num, "tarta_top_dst_udp_paquetes.png", "Top udp destino por paquetes")

    #Obtención de series temporales de ancho de banda
    #TODO: Añadir código para obtener los datos y generar la gráfica de la serie temporal de ancho de banda con MAC como origen
    logging.info('Ejecutando tshark para obtener el tráfico de paquetes de nivel 2 con mac como origen')
    codigo, salida = ejecutarComandoObtenerSalida("tshark -r {} -qz io,stat,1,,eth.src=={}".format(args.tracefile, args.mac))
    codigo, tiempo_ini = ejecutarComandoObtenerSalida("tshark -r {} -T fields -e frame.time_epoch -Y 'eth.src=={} and frame.number==1'".format(args.tracefile, args.mac))
    tiempo_ini = tiempo_ini[:-1]
    tiempo_ini = float(tiempo_ini)
    lista = []
    segs = []
    salida = salida.split('\n')
    salida = salida[12:-2]
    for linea in salida:
        item = linea[23:31]
        seg = linea[2:5]
        if item != '':
            item.strip()
            lista.append(int(item)*8)
        if seg != '':
            seg.strip()
            segs.append(int(seg) + tiempo_ini)
    if len(lista) > 0 and len(segs) > 0 and len(segs) == len(lista):
        print("Pintando grafica tráfico de paquetes con mac src")
        pintarSerieTemporal(segs,lista,"ecdf_trafico_paquetes_src.png","Tamaños de los paquetes de nivel 2 con mac como origen","Tiempo en segundos desde el comienzo","Bits/segundo")
    else:
        print("No hay tráfico de paquetes de nivel 2 con {} como mac origen.".format(args.mac))


    #TODO: Añadir código para obtener los datos y generar la gráfica de la serie temporal de ancho de banda con MAC como destino
    logging.info('Ejecutando tshark para obtener el tráfico de paquetes de nivel 2 con mac como destino')
    codigo, salida = ejecutarComandoObtenerSalida("tshark -r {} -qz io,stat,1,,eth.dst=={}".format(args.tracefile, args.mac))
    lista = []
    segs = []
    salida = salida.split('\n')
    salida = salida[12:-2]
    for linea in salida:
        item = linea[23:30]
        seg = linea[2:5]
        if item != '':
            item.strip()
            lista.append(int(item)*8)
        if seg != '':
            seg.strip()
            segs.append(int(seg) + tiempo_ini)
    if len(lista) > 0 and len(segs) > 0 and len(segs) == len(lista):
        print("Pintando grafica tráfico de paquetes con mac dst")
        pintarSerieTemporal(segs,lista,"ecdf_trafico_paquetes_dst.png","Tamaños de los paquetes de nivel 2 con mac como destino","Tiempo en segundos desde el comienzo","Bits/segundo")
    else:
        print("No hay tráfico de paquetes de nivel 2 con {} como mac destino.".format(args.mac))

    #Obtención de las ECDF de tamaño de los paquetes
    #TODO: Añadir código para obtener los datos y generar la gráfica de la ECDF de los tamaños de los paquetes a nivel 2
    logging.info('Ejecutando tshark para obtener los tamaños de paquetes de nivel 2')
    codigo, salida_src = ejecutarComandoObtenerSalida("tshark -r {} -T fields -e frame.len eth.src=={}".format(args.tracefile, args.mac))
    salida_src = salida_src.split('\n')
    codigo, salida_dst = ejecutarComandoObtenerSalida("tshark -r {} -T fields -e frame.len eth.dst=={}".format(args.tracefile, args.mac))
    salida_dst = salida_dst.split('\n')
    sizes_src = []
    sizes_dst = []
    for item in salida_src:
        # Sin texto de ip y no vaciod
        if item != '':
            sizes_src.append(int(item))
    if len(sizes_src) > 0:
        print("Pintando grafica tamaño de paquetes con mac src")
        pintarECDF(sizes_src,"ecdf_tamaños_paquetes_nivel_2_src.png","Tamaños de los paquetes de nivel 2 con mac como origen","Tamaño(Bytes)","Paquetes")
    else:
        print("No hay paquetes de nivel 2 con {} como mac origen.".format(args.mac))


    for item in salida_dst:
        # Sin texto de ip y no vaciod
        if item != '':
            sizes_dst.append(int(item))
    if len(sizes_dst) > 0:
        print("Pintando grafica tamaño de paquetes con mac dst")
        pintarECDF(sizes_dst,"ecdf_tamaños_paquetes_nivel_2_dst.png","Tamaños de los paquetes de nivel 2 con mac como destino","Tamaño(Bytes)","Paquetes")
    else:
        print("No hay paquetes de nivel 2 con {} como mac destino.".format(args.mac))

    #Obtención de las ECDF de tamaño de los tiempos entre llegadas
    #TODO: Añadir código para obtener los datos y generar la gráfica de la ECDF de los tiempos entre llegadas para el flujo TCP
    logging.info('Ejecutando tshark para obtener los ttiempos entre paquetes TCP')
    codigo, salida = ejecutarComandoObtenerSalida("tshark -r {} -T fields -e frame.time_delta -Y 'tcp'".format(args.tracefile))
    salida = salida.split('\n')
    times = []
    for item in salida:
        # Sera una string vacia si no es tcp
        if item != '' and item != '\t':
            times.append(float(item))

    if len(times) > 0:
        print("Pintando grafica timedelta tcp")
        pintarECDF(times,"ecdf_timedelta_tcp.png","Tiempos entre paquetes tcp","Tiempo(s)","Paquetes")
    else:
        print("No hay pquetes tcp.")
    #TODO: Añadir código para obtener los datos y generar la gráfica de la ECDF de los tiempos entre llegadas para el flujo UDP
    logging.info('Ejecutando tshark para obtener los tiempo entre paquetes UDP')
    codigo, salida = ejecutarComandoObtenerSalida("tshark -r {} -T fields -e frame.time_delta -Y 'udp'".format(args.tracefile))
    salida = salida.split('\n')
    times = []
    for item in salida:
        # Sin texto de ip y no vaciod
        if item != '' and item != '\t':
            times.append(float(item))

    if len(times) > 0:
        print("Pintando grafica timedelta tcp")
        pintarECDF(times,"ecdf_timedelta_udp.png","Tiempos entre paquetes udp","Tiempo(s)","Paquetes")
    else:
        print("No hay paquetes udp.")
