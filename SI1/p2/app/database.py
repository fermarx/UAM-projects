# -*- coding: utf-8 -*-

import os
import sys, traceback
from sqlalchemy import create_engine
from sqlalchemy import Table, Column, Integer, String, MetaData, ForeignKey, text
from sqlalchemy.sql import select, insert, join, and_, or_, not_, update
from sqlalchemy.exc import IntegrityError
from sqlalchemy.dialects.postgresql import aggregate_order_by
from sqlalchemy import func
from sqlalchemy import cast
from psycopg2.errors import UniqueViolation

# configurar el motor de sqlalchemy
db_engine = create_engine("postgresql://alumnodb:alumnodb@localhost/si1", echo=False)
db_meta = MetaData(bind=db_engine)
# cargar una tabla
db_table_movies = Table('imdb_movies', db_meta, autoload=True, autoload_with=db_engine)
db_customers = Table('customers', db_meta, autoload=True, autoload_with=db_engine)
db_dirmovies = Table('imdb_directormovies', db_meta, autoload=True, autoload_with=db_engine)
db_actmovies = Table('imdb_actormovies', db_meta, autoload=True, autoload_with=db_engine)
db_directors = Table('imdb_directors', db_meta, autoload=True, autoload_with=db_engine)
db_actors = Table('imdb_actors', db_meta, autoload=True, autoload_with=db_engine)
db_genmovies = Table('imdb_moviegenres', db_meta, autoload=True, autoload_with=db_engine)
db_products = Table('products', db_meta, autoload=True, autoload_with=db_engine)
db_orders = Table('orders', db_meta, autoload=True, autoload_with=db_engine)
db_orderdetail = Table('orderdetail', db_meta, autoload=True, autoload_with=db_engine)

def db_listOfMovies1949():
    try:
        # conexion a la base de datos
        db_conn = None
        db_conn = db_engine.connect()
        
        # Seleccionar las peliculas del anno 1949
        db_movies_1949 = select([db_table_movies]).where(text("year = '1949'"))
        db_result = db_conn.execute(db_movies_1949)
        #db_result = db_conn.execute("Select * from imdb_movies where year = '1949'")
        
        db_conn.close()
        
        return  list(db_result)
    except:
        if db_conn is not None:
            db_conn.close()
        print("Exception in DB access:")
        print("-"*60)
        traceback.print_exc(file=sys.stderr)
        print("-"*60)

        return 'Something is broken'

def db_getCatalogue():
    try:
        db_connect = db_engine.connect()
        db_result = db_connect.execute("select prod_id,movietitle,price from products inner join imdb_movies on imdb_movies.movieid = products.movieid order by prod_id limit 20;")

        return db_result
    except:

        return []

def db_getMovie(id):
    try:
        db_connect = db_engine.connect()
        db_result = db_connect.execute("select prod_id,movietitle,year,price from products inner join imdb_movies on imdb_movies.movieid = products.movieid where prod_id ="+str(id)+";")

        return list(db_result)[0]
    except:

        return []

def db_get_top_ventas():
    try:
        db_connect = db_engine.connect()
        db_result = db_connect.execute("select * from getTopVentas("+"2014"+","+"2020"+");").fetchall()

        return db_result
    except:
        print("Error")

        return []


#Unir tabla customer con el carrito previo
def db_customer_carrito(userid, carrito):
    db_conn = None
    db_conn = db_engine.connect()
    if db_carrito_exists(userid) == False:
        result = db_create_order(userid)
        cid = result[0][0]
    else:
        cid = db_obtain_carrito(userid)
    for item in carrito.keys():
        db_insert_product(item, orderid, carrito[item])
    return db_get_carrito(cid)

# Comprueba si existe un carrito del usuario en la tabla customers
def db_carrito_exists(userid):
    db_conn = None
    db_conn = db_engine.connect()
    query = select([func.count()]).select_from(db_orders).where(
        and_(db_orders.customerid == userid,
             db_orders.status == None))
    resultq = list(db_conn.execute(query))
    db_conn.close()
    if(resultq[0][0] == 0):
        return False
    else:
        return True

#Carrito del usuario en forma de diccionario
def db_get_carrito(orderid):
    db_conn = None
    db_conn = db_engine.connect()
    query = select([db_orderdetail.prod_id, db_orderdetail.quantity]).select_from(db_orderdetail).where(
        db_orderdetail.orderid == orderid
    )
    result = list(db_conn.execute(query))
    dict = {}
    for item in result:
        dict[item[0]] = item[1]
    db_conn.close()
    return dict

# Obtenemos el carrito del usuario, si no ha aparecido previamente mediante la funcion carrito_exists
def db_obtain_carrito(userid):
    db_conn = None
    db_conn = db_engine.connect()
    queryo = select([db_orders.orderid]).select_from(db_orders).where(
        and_(db_orders.customerid == userid,
             db_orders.status == None))
    queryo = list(db_conn.execute(queryo))
    db_conn.close()
    return check[0][0]

# Creamos un carrito vacío
def db_create_cart(userid):
    db_conn = None
    db_conn = db_engine.connect()
    query = db_orders.insert().values(customerid=userid)
    db_conn.execute(query)
    query2 = select([db_orders.orderid]).where(
        and_(
            db_orders.customerid == userid,
            db_orders.status == None
        )
    )
    result = db_conn.execute(query2)
    result = list(result)
    db_conn.close()
    return result

#Insertar el nuevo usuario en la tabla customers de la BD
def db_registro(firstname, lastname, address1, address2, city, state, zip, country, region, email, phone, creditcardtype, card, creditcardexpiration, nick, cont, age, income, gender):
    try:
        db_conn = None
        db_conn = db_engine.connect()

        id = db_conn.execute("select customerid from customers order by customerid desc limit 1;")

        db_result = db_conn.execute("insert into customers values ("+str(list(id)[0][0] + 1)+", '"+firstname+"', '"+lastname+"', '"+address1+"', '"+address2+"', '"+city+"', '"+state+"', '"+zip+"', '"+country+"', '"+region+"', '"+email+"', '"+phone+"', '"+creditcardtype+"', '"+card+"', '"+creditcardexpiration+"', '"+nick+"', '"+cont+"', '"+age+"', '"+str(income)+"', '"+gender+"');")

        return db_result

    except:
        if db_conn is not None:
            db_conn.close()
        print("Exception in DB access:")
        print("-"*60)
        traceback.print_exc(file=sys.stderr)
        print("-"*60)

        return 'Something gone wrong'

def db_login(username):
    try:
        db_connect = db_engine.connect()
        db_result = db_connect.execute("select password from customers where username = '"+username+"';")

        return list(db_result)
    except:
        if db_connect is not None:
            db_connect.close()
        return -1