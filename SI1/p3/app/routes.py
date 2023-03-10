#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from app import app
from app import database
from flask import render_template, request, url_for, redirect, session, flash
import json
import os
import sys
import random
import pickle
import hashlib
import time
from collections import OrderedDict
from datetime import date
from app import createMongoDBFromPostgreSQLDB


USERS_FOLDER = 'usuarios'
DATA_FILE = 'data.dat'
HIST_FILE = 'historial.json'
CATALOGUE_FILE = 'catalogue/catalogue.json'
STATIC_IMG = 'static/imagenes'



@app.route('/borraCliente', methods=['POST','GET'])
def borraCliente():
    if 'customerid' in request.form:
        customerid = request.form["customerid"]
        bSQL       = request.form["txnSQL"]
        bCommit = "bCommit" in request.form
        bFallo  = "bFallo"  in request.form
        duerme  = request.form["duerme"]
        dbr = database.delCustomer(customerid, bFallo, bSQL=='1', int(duerme), bCommit)
        return render_template('borraCliente.html', dbr=dbr)
    else:
        return render_template('borraCliente.html')

@app.route('/listaClientesMes', methods=['GET', 'POST'])
def listaClientesMes():
    if 'fecha' in request.form:
        fecha  = request.form['fecha']
        mes    = request.form['mes']
        anio   = request.form['anio']
        umbral = request.form['minimo']
        intervalo = request.form['intervalo']
        use_prepare = 'prepare'  in request.form
        break0 = 'break0' in request.form
        niter  = request.form['iter']
        # connect fuera para comparar tiempos con más precisión
        db_conn = database.dbConnect()
        t0=round(time.time() * 1000)
        dbr = database.getListaCliMes(db_conn, mes, anio, int(umbral), int(intervalo), use_prepare, break0, int(niter))
        t1=round(time.time() * 1000)
        database.dbCloseConnect(db_conn)

        return render_template('listaClientesMes.html',
            fecha = fecha,
            mes   = mes,
            anio  = anio,
            umbral = int(umbral),
            intervalo = int(intervalo),
            use_prepare = use_prepare,
            break0 = break0,
            tiempo = str(int(t1-t0)),
            dbr=dbr
           )
    else:
        return render_template('listaClientesMes.html')

@app.route('/topUSA', methods=['GET', 'POST'])
def topUSA():
    if session.get('username') is not None:
        username = session['username']

    else:
        username = ""

    topUSA_life = createMongoDBFromPostgreSQLDB.mongo_life()
    movies_life={}
    movies_life=topUSA_life
    dic_life = {}
    for x in movies_life:
        dic_life = {'title': x.get("title"),
         'year': x.get("year"),
         'genres': x.get("genres"),
         'actors': x.get("actors"),
         'directors': x.get("directors"),
        }

    topUSA_Allen = createMongoDBFromPostgreSQLDB.mongo_woodyAllen()
    movies_allen={}
    movies_allen=topUSA_Allen
    dic_allen = {}
    for x in movies_allen:
        dic_allen = {'title': x.get("title"),
         'year': x.get("year"),
         'genres': x.get("genres"),
         'actors': x.get("actors"),
         'directors': x.get("directors"),
        }

    topUSA_ParsonsGalecki = createMongoDBFromPostgreSQLDB.mongo_parsonsGalecki()
    movies_galecki={}
    movies_galecki=topUSA_ParsonsGalecki
    dic_galecki = {}
    for x in movies_galecki:
        dic_galecki = {'title': x.get("title"),
         'year': x.get("year"),
         'genres': x.get("genres"),
         'actors': x.get("actors"),
         'directors': x.get("directors"),
        }

    return render_template('topUSA.html', error="", username=username, dic_life=dic_life, dic_allen=dic_allen, dic_galecki=dic_galecki)

###############################################################################
###############################################################################

@app.route('/', methods=['GET', 'POST'])
@app.route('/principal.html', methods=['GET', 'POST'])
def principal():
    if session.get('nickname'):
        flash(session['nickname'])
    return render_template('principal.html')

@app.route('/indice.html', methods=['GET', 'POST'])
def indice():

    movies_filtro = ""
    movies = database.db_getCatalogue()

    topventas = database.db_get_top_ventas()

    if session.get('username') is not None:
        username = session['username']
    else:
        username = ""

    return render_template('indice.html', title="Home", error="", username=username, movies=movies, filtrar="", movies_filtro="", topventas=topventas)

@app.route('/registro.html', methods=['GET', 'POST'])
def registro():
    error=""
    if request.method == 'POST':
        
        nick = request.form.get('nickname')
        cont = request.form.get('cont')
        email = request.form.get('email')
        card = request.form.get('creditcard')
        income = random.randint(0, 101)
        if not (nick and cont and email and card):
            flash('Rellene todos los campos')
            return render_template('registro.html')
        firstname = ""
        lastname = ""
        address1=""
        address2=""
        city=""
        state=""
        zip=""
        country=""
        region=""
        phone=""
        creditcardtype=""
        creditcardexpiration=""
        age=""
        gender=""
        register = database.db_registro(firstname, lastname, address1, address2, city, state, zip, country, region, email, phone, creditcardtype, card, creditcardexpiration, nick, cont, age, income, gender)
        #register =database.db_registro(nick, cont, email, card)
        if register == -1:
            error = "El usuario ya existe."
        else:
            error = "¡Usuario creado con éxito! Ya puedes entrar a tu cuenta."
    else:
        nick = ""
    return render_template('registro.html', error=error, username=nick)

@app.route('/login.html', methods=['GET', 'POST'])
def login():
    error = ""
    if request.method == 'POST':
        nick = request.form.get('nickname')
        cont = request.form.get('password')
        if not (nick and cont):
            flash('Faltan campos que rellenar')
            return render_template('login.html')

        # user registered
        login = database.db_login(nick)
        if login == -1 or str(login) == "[]":
            error = "nick no registrado."

        #password correct?
        if login[0][0] == cont:
            session['nickname'] = nick
            session.modified = True
            return redirect(url_for('indice'))
        else:
            error = "Error en login"
            return render_template('login.html', nickname="", error=error, username_cookie=request.cookies.get('userID'))
    
    if session.get('nickname') is not None:
        nickname = session['nickname']
    else:
        nickname = ""

    return render_template('login.html', nickname=nickname, error=error, username_cookie=request.cookies.get('userID'))

@app.route('/logout.html', methods=['GET', 'POST'])
def logout():
    session.pop('nickname', None)
    return redirect(url_for('index'))

@app.route('/producto/<id>.html', methods=['GET', 'POST'])
def producto(id):
    film = database.db_getMovie(id)

    if request.method == 'POST':
        if int(request.form.get('amount')) < 0:
            return render_template('producto.html', title=film[1], film=film)

        if session.get('carrito'):
            session['carrito'][int(id)] = int(request.form.get('amount'))

            if(int(request.form.get('amount')) == 0):
                session['carrito'].pop(int(id), None)
        else:
            session['carrito'] = {int(id): int(request.form.get('amount'))}

    return render_template('producto.html', title=film[1], film=film)

@app.route('/carrito.html', methods=['GET', 'POST'])
def carrito():
    total = 0
    films = []
    if not session.get('carrito'):
        session['carrito'] = {}

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