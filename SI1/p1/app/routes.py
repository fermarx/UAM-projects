#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from app import app
from flask import render_template, request, url_for, redirect, session, flash
import json
import os
import sys
import random
import pickle
import hashlib
from collections import OrderedDict
from datetime import date

USERS_FOLDER = 'usuarios'
DATA_FILE = 'data.dat'
HIST_FILE = 'historial.json'
CATALOGUE_FILE = 'catalogue/catalogue.json'
STATIC_IMG = 'static/imagenes'


@app.route('/', methods=['GET', 'POST'])
@app.route('/principal.html', methods=['GET', 'POST'])
def principal():
    if session.get('nickname'):
        flash(session['nickname'])
    return render_template('principal.html')


@app.route('/indice.html', methods=['GET', 'POST'])
def indice():
    pelis = None

    if request.method == 'POST':
        #get the filters
        titulo = request.form.get('title')
        if (not request.form.get('title')) and request.form.get('category') == 'Ninguno':
            return redirect(url_for('indice'))

        with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
            catalogue = json.loads(data_file.read())
            pelis = catalogue['peliculas']
            if request.form.get('category') != 'Ninguno':
                pelis = list(filter(lambda f: request.form.get('category') in f['categoria'], pelis))
            pelis = list(filter(lambda f: titulo.lower() in f['titulo'].lower(), pelis))

        return render_template('indice.html', title='Búsqueda', films=pelis, search_query=titulo)

    else:
        #with no filters
        with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
            catalogue = json.loads(data_file.read())
            pelis = catalogue['peliculas']
            pelis = sorted(pelis, key=(lambda m : m['titulo']))[:10]

        return render_template('indice.html', title='Home', films=pelis, search_query=None)


@app.route('/registro.html', methods=['GET', 'POST'])
def registro():
    if request.method == 'POST':
        
        nick = request.form.get('nickname')
        cont = request.form.get('cont')
        email = request.form.get('email')
        card = request.form.get('creditcard')
        if not (nick and cont and email and card):
            flash('Rellene todos los campos')
            return render_template('registro.html')
            
        #user available
        users = next(os.walk(os.path.join(app.root_path, USERS_FOLDER)))[1]
        if nick in users:
            flash('Usuario no disponible')
            return render_template('registro.html')

        #encript the password
        md5 = hashlib.md5()
        md5.update(cont.encode('utf-8'))
        pas = md5.hexdigest()

        #store data in directory usuarios
        data = {'nickname': nick, 'password': pas, 'email': email, 'card': card , 'cash': random.randint(0, 100), 'carrito':{}}
        os.mkdir(os.path.join(app.root_path, USERS_FOLDER, nick.lower()))
        with open(os.path.join(app.root_path, USERS_FOLDER, nick.lower(), DATA_FILE), 'wb') as file:
            pickle.dump(data, file)

        #historial del usuario
        with open(os.path.join(app.root_path, USERS_FOLDER, nick.lower(), HIST_FILE), 'w') as file:
            history = {'historial': []}
            json.dump(history, file)
        if (request.files.get('file')):
            os.mkdir(os.path.join(app.root_path, STATIC_IMG, nick.lower()))
            request.files.get('file').save(os.path.join(app.root_path, STATIC_IMG, nick.lower(), PHOTO_FILE))

        return redirect(url_for('indice'))
    else:
        return render_template('registro.html')


@app.route('/login.html', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        nick = request.form.get('nickname')
        cont = request.form.get('password')
        if not (nick and cont):
            flash('Faltan campos que rellenar')
            return render_template('login.html')

        # user registered
        usuarios = next(os.walk(os.path.join(app.root_path, USERS_FOLDER)))[1]
        if  nick.lower() not in usuarios:
            flash('El usuario no existe')
            return render_template('login.html')

        #password correct?
        md5 = hashlib.md5()
        md5.update(cont.encode('utf-8'))
        pas = md5.hexdigest()
        with open(os.path.join(app.root_path, USERS_FOLDER,  nick.lower(), DATA_FILE), 'rb') as file:
            userdata = pickle.load(file)
        if pas != userdata['password']:
            flash('Contraseña incorrecta')
            return render_template('login.html')
        
        #charging
        session['nickname'] = userdata['nickname']
        session['email'] = userdata['email']
        session['card'] = userdata['card']
        session['cash'] = userdata['cash']
        session['slug_nick'] =  nick.lower()

        #cargar el carrito
        if session.get('carrito'):
            userdata['carrito'].update(session['carrito'])
            session['carrito'].update(userdata['carrito'])
        else:
            session['carrito'] = userdata['carrito']
        return redirect(url_for('indice'))
    else:
        last_nick = request.cookies.get('nickname')
        return render_template('login.html', last_nick=last_nick)

@app.route('/logout.html', methods=['GET', 'POST'])
def logout():
    # guardar carrito

    # Reading userdata
    with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), DATA_FILE), 'rb') as file:
        userdata = pickle.load(file)
    # Updating
    userdata['cash'] = session['cash']
    userdata['carrito'] = session['carrito']
    # Storing updates
    with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), DATA_FILE), 'wb') as file:
        pickle.dump(userdata, file)

    redirect(url_for('indice')).set_cookie('last_nick', session['nickname'])

    # borra datos carrito
    session.pop('carrito', None)

    #borra datos del usuario
    session.pop('nickname', None)

    return redirect(url_for('indice'))


@app.route('/producto/<id>.html', methods=['GET', 'POST'])
def producto(id):
    with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
        catalogue = json.loads(data_file.read())
        film = list(filter(lambda f: f['id'] == int(id), catalogue['peliculas']))[0]

    if request.method == 'POST':
        if int(request.form.get('amount')) < 0:
            return render_template('producto.html', title=film['titulo'], film=film)

        if session.get('carrito'):
            session['carrito'][int(id)] = int(request.form.get('amount'))

            if(int(request.form.get('amount')) == 0):
                session['carrito'].pop(int(id), None)
        else:
            session['carrito'] = {int(id): int(request.form.get('amount'))}

    return render_template('producto.html', title=film['titulo'], film=film)


@app.route('/carrito.html', methods=['GET', 'POST'])
def carrito():
    total = 0
    if not session.get('carrito'):
        session['carrito'] = {}

    if request.method == 'POST':
        session['carrito'].pop(int(request.form.get('prod_id')), None)

    with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
        catalogue = json.loads(data_file.read())
        films = catalogue['peliculas']
        films = list(filter(lambda f: int(f['id']) in session['carrito'].keys(), films))

    for f in films:
        f['amount'] = session['carrito'][f['id']]
        total += f['precio']*f['amount']

    total = round(total, 2)

    return render_template('carrito.html', films=films, total=total)


@app.route('/comprar', methods=['POST'])
def comprar():
    if not session.get('nickname'):
        return redirect(url_for('login'))
    if not session.get('carrito'):
        session['carrito'] = {}

    with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
        catalogue = json.loads(data_file.read())
        films = catalogue['peliculas']
        films = list(filter(lambda f: int(f['id']) in session['carrito'].keys(), films))

    total = 0
    for f in films:
        f['amount'] = session['carrito'][f['id']]
        total += f['precio']*f['amount']

    if session['cash'] >= total:
        session['cash'] -= total
        session['cash'] = round(session['cash'], 2)

        with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), HIST_FILE), encoding="utf-8") as data_file:
            history = json.loads(data_file.read())

        history['historial'].extend([{'id': f['id'], 'date': date.today().strftime("%d-%b-%Y"), 'price': f['precio'], 'amount': f['amount']} for f in films])

        with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), HIST_FILE), 'w') as file:
            json.dump(history, file)

        session['carrito'] = {}

        # Reading userdata
        with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), DATA_FILE), 'rb') as file:
            userdata = pickle.load(file)
        # Updating
        userdata['cash'] = session['cash']
        userdata['carrito'] = session['carrito']
        # Storing updates
        with open(os.path.join(app.root_path, USERS_FOLDER, session['nickname'].lower(), DATA_FILE), 'wb') as file:
            pickle.dump(userdata, file)

        return redirect(url_for('indice'))

    else:
        flash('Recargue saldo')
        return redirect(url_for('carrito'))


def _cmp_dates(date):
    # Format is dd-mmm-yyyy
    months = {'Jan':'01', 'Feb':'02', 'Mar':'03', 'Apr':'04', 'May':'05', 'Jun':'06',
              'Jul':'07', 'Aug':'08', 'Sep':'09', 'Oct':'10', 'Nov':'11', 'Dec':'12'}
    d = date[:2]
    m = months[date[3:6]]
    y = date[7:]
    return int(y+m+d)


@app.route('/historial.html', methods=['GET'])
def historial():
    if not session.get('nickname'):
        return redirect(url_for('login'))

    with open(os.path.join(app.root_path, USERS_FOLDER,  session.get('nickname').lower(), HIST_FILE), encoding="utf-8") as data_file:
        history = json.loads(data_file.read())
        history = history['historial']

    with open(os.path.join(app.root_path, CATALOGUE_FILE), encoding="utf-8") as data_file:
        catalogue = json.loads(data_file.read())
        films = catalogue['peliculas']

    for dh in history:
        for df in films:
            if dh['id'] == df['id']:
                dh.update(df)
                break

    dates = [f['date'] for f in history]
    #dates.sort(key=_cmp_dates)
    hist = OrderedDict()
    for date in dates:
        hist[date] = []
    for f in history:
        hist[f['date']].append(f)

    return render_template('historial.html', history=hist)

@app.route('/saldo', methods=['GET', 'POST'])
def saldo():
    if not session.get('nickname'):
        return redirect(url_for('login'))
            
    if request.method == 'POST':
        if request.form.get('cash'):
            try:
                session['cash'] += float(request.form.get('cash'))
                session['cash'] = round(session['cash'], 2)
            except ValueError:
                flash('Introduzca saldo correcto')
        return redirect(url_for('carrito'))
        
@app.route('/connectedusers', methods=['GET'])
def connectedusers():
    return str(random.randint(10, 1000))
