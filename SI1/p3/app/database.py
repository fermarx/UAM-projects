# -*- coding: utf-8 -*-

import os
import sys, traceback, time

from sqlalchemy import create_engine
from sqlalchemy import Table, Column, Integer, String, MetaData, ForeignKey, text
from sqlalchemy.sql import select

# configurar el motor de sqlalchemy
db_engine = create_engine("postgresql://alumnodb:alumnodb@localhost/si1", echo=False, execution_options={"autocommit":False})
db_meta = MetaData(bind=db_engine,reflect = True)

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

##############################################################################
##############################################################################

query_delInventory = "UPDATE inventory " +\
                     "SET sales=sales-orderdetail.quantity " +\
                     "FROM orderdetail, orders " +\
                     "WHERE inventory.prod_id=orderdetail.prod_id AND " +\
                     "orderdetail.orderid=orders.orderid AND " +\
                     "orders.customerid="
query_delOrderdetail = "DELETE FROM orderdetail " +\
                       "USING orders " +\
                       "WHERE orderdetail.orderid=orders.orderid AND " +\
                       "orders.customerid="
query_delOrders = "DELETE FROM orders " +\
                  "WHERE orders.customerid="
query_delCustomers = "DELETE FROM customers " +\
                     "WHERE customerid="


def dbConnect():
    return db_engine.connect()

def dbCloseConnect(db_conn):
    db_conn.close()

def getListaCliMes(db_conn, mes, anio, iumbral, iintervalo, use_prepare, break0, niter):

    # TODO: implementar la consulta; asignar nombre 'cc' al contador resultante
    # TODO: ejecutar la consulta 
    # - mediante PREPARE, EXECUTE, DEALLOCATE si use_prepare es True
    # - mediante db_conn.execute() si es False

    # Array con resultados de la consulta para cada umbral
    dbr=[]

    for ii in range(niter):

        # TODO: ...

        # Guardar resultado de la query
        dbr.append({"umbral":iumbral,"contador":res['cc']})

        # TODO: si break0 es True, salir si contador resultante es cero
        
        # Actualizacion de umbral
        iumbral = iumbral + iintervalo
                
    return dbr

def getMovies(anio):
    # conexion a la base de datos
    db_conn = db_engine.connect()

    query="select movietitle from imdb_movies where year = '" + anio + "'"
    resultproxy=db_conn.execute(query)

    a = []
    for rowproxy in resultproxy:
        d={}
        # rowproxy.items() returns an array like [(key0, value0), (key1, value1)]
        for tup in rowproxy.items():
            # build up the dictionary
            d[tup[0]] = tup[1]
        a.append(d)
        
    resultproxy.close()  
    
    db_conn.close()  
    
    return a
    
def getCustomer(username, password):
    # conexion a la base de datos
    db_conn = db_engine.connect()

    query="select * from customers where username='" + username + "' and password='" + password + "'"
    res=db_conn.execute(query).first()
    
    db_conn.close()  

    if res is None:
        return None
    else:
        return {'firstname': res['firstname'], 'lastname': res['lastname']}

def _parse_error_message(exc_str):
    ret = ""
    ind = exc_str.find("\n")
    ret = exc_str[:ind]
    return ret
    
def _check_db_customer(db_conn, dbr, customerid):
    query_str = "SELECT firstname, lastname FROM customers where customerid="+str(customerid)

    ret = db_conn.execute(query_str)
    if ret.rowcount == 0:
        dbr.append("There is no user with ID="+str(customerid))
    else:
        ret_lst = list(ret)
        str_ret = "Found customer with ID="+str(customerid)+\
               " -> Firstname: "+str(ret_lst[0][0])+" Lastname: "+str(ret_lst[0][1])+"\n"
        dbr.append(str_ret)

        query_items_str = "SELECT sum(quantity) FROM orders, orderdetail "+\
                          "WHERE orders.orderid=orderdetail.orderid AND customerid="+str(customerid)
        ret2 = db_conn.execute(query_items_str)
        ret2_lst = list(ret2)
        ret_num = ret2_lst[0][0]
        if ret_num is None:
            dbr.append("There is no purchased items by customer ID="+str(customerid))
        else:
            ret2_lst = list(ret2)
            str_ret2 = "Total number of purchased items by customer with ID="+str(customerid)+" : "+str(ret_num)
            dbr.append(str_ret2)
        ret2.close()

    ret.close()

def _delCustomerExec(customerid, bFallo, duerme, bCommit):
    dbr = []
    db_conn = dbConnect()

    # Previous database check
    dbr.append("Checking customer data BEFORE trying to delete")
    _check_db_customer(db_conn, dbr, customerid)

    try:
        # Init transaction BEGIN
        ret = db_conn.execute("BEGIN")
        ret.close()
        dbr.append("BEGIN transaction")
        # Updating inventory
        ret = db_conn.execute(query_delInventory+str(customerid))
        ret.close()
        dbr.append("Updated inventory entries for customerid="+str(customerid))

        if bFallo:
        # Forcing database error and rolling back
            # Deleting orderdetail data
            ret = db_conn.execute(query_delOrderdetail+str(customerid))
            ret.close()
            dbr.append("Deleted orderdetail entries for customerid="+str(customerid))
            # Intermediate Commit
            if bCommit:
                ret = db_conn.execute("COMMIT")
                ret.close()
                dbr.append("Intermediate Commit")
                ret = db_conn.execute("BEGIN")
                ret.close()
                dbr.append("BEGIN transaction again")
            # Deleting customer data (WITHOUT DELETING orders data before!)
            ret = db_conn.execute(query_delCustomers+str(customerid))
            ret.close()
            dbr.append("Deleted customers entry for customerid="+str(customerid))
            # Deleting orders data
            ret = db_conn.execute(query_delOrders+str(customerid))
            ret.close()
            dbr.append("Deleted orders entries for customerid="+str(customerid))

        else:
        # Good transaction
            # Deleting orderdetail data
            ret = db_conn.execute(query_delOrderdetail+str(customerid))
            ret.close()
            dbr.append("Deleted orderdetail entries for customerid="+str(customerid))

            time.sleep(duerme)

            # Deleting orders data
            ret = db_conn.execute(query_delOrders+str(customerid))
            ret.close()
            dbr.append("Deleted orders entries for customerid="+str(customerid))
            # Deleting customer data
            ret = db_conn.execute(query_delCustomers+str(customerid))
            ret.close()
            dbr.append("Deleted customers entry for customerid="+str(customerid))

    except Exception as e:
        ret = db_conn.execute("ROLLBACK")
        ret.close()
        dbr.append("An Error ocurred during transaction. Rolling back. "+_parse_error_message(str(e)))

    else:
        ret = db_conn.execute("COMMIT")
        ret.close()
        dbr.append("Customer data deleted successfully")

    dbr.append("Checking customer data AFTER trying to delete")
    _check_db_customer(db_conn, dbr, customerid)
    dbCloseConnect(db_conn)
    return dbr

def _delCustomerAlc(customerid, bFallo, duerme, bCommit):
    dbr = []
    db_conn = dbConnect()

    # Previous database check
    dbr.append("Checking customer data BEFORE trying to delete")
    _check_db_customer(db_conn, dbr, customerid)

    try:
        # Init transaction BEGIN
        trans = db_conn.begin()
        dbr.append("BEGIN transaction")
        # Updating inventory
        ret = db_conn.execute(query_delInventory+str(customerid))
        ret.close()
        dbr.append("Updated inventory entries for customerid="+str(customerid))

        if bFallo:
        # Forcing database error and rolling back
            # Deleting orderdetail data
            ret = db_conn.execute(query_delOrderdetail+str(customerid))
            ret.close()
            dbr.append("Deleted orderdetail entries for customerid="+str(customerid))
            # Intermediate Commit
            if bCommit:
                trans.commit()
                dbr.append("Intermediate Commit")
                trans = db_conn.begin()
                dbr.append("BEGIN transaction again")
            # Deleting customer data (WITHOUT DELETING orders data before!)
            ret = db_conn.execute(query_delCustomers+str(customerid))
            ret.close()
            dbr.append("Deleted customers entry for customerid="+str(customerid))
            # Deleting orders data
            ret = db_conn.execute(query_delOrders+str(customerid))
            ret.close()
            dbr.append("Deleted orders entries for customerid="+str(customerid))

        else:
        # Good transaction
            # Deleting orderdetail data
            ret = db_conn.execute(query_delOrderdetail+str(customerid))
            ret.close()
            dbr.append("Deleted orderdetail entries for customerid="+str(customerid))

            time.sleep(duerme)

            # Deleting orders data
            ret = db_conn.execute(query_delOrders+str(customerid))
            ret.close()
            dbr.append("Deleted orders entries for customerid="+str(customerid))
            # Deleting customer data
            ret = db_conn.execute(query_delCustomers+str(customerid))
            ret.close()
            dbr.append("Deleted customers entry for customerid="+str(customerid))

    except Exception as e:
        trans.rollback()
        dbr.append("An Error ocurred during transaction. Rolling back. "+_parse_error_message(str(e)))
    else:
        trans.commit()
        dbr.append("Customer data deleted successfully")

    dbr.append("Checking customer data AFTER trying to delete")
    _check_db_customer(db_conn, dbr, customerid)
    dbCloseConnect(db_conn)
    return dbr

def delCustomer(customerid, bFallo, bSQL, duerme, bCommit):
    # Array de trazas a mostrar en la página
    dbr=[]

    # TODO: Ejecutar consultas de borrado
    # - ordenar consultas según se desee provocar un error (bFallo True) o no
    # - ejecutar commit intermedio si bCommit es True
    # - usar sentencias SQL ('BEGIN', 'COMMIT', ...) si bSQL es True
    # - suspender la ejecución 'duerme' segundos en el punto adecuado para forzar deadlock
    # - ir guardando trazas mediante dbr.append()

    if bSQL:
        dbr = _delCustomerExec(customerid, bFallo, duerme, bCommit)
    else:
        dbr = _delCustomerAlc(customerid, bFallo, duerme, bCommit)

    return dbr

def db_topUSA():
    try:
        db_result = dbConnect().execute("select distinct movietitle as title, year,\
        array_to_string(array_agg(distinct genre), ', ') as genres,\
        array_to_string(array_agg(distinct actorname), '; ') as actors\
        from imdb_movies\
        natural join imdb_moviecountries\
        natural join imdb_moviegenres\
        natural join imdb_actormovies\
        natural join imdb_actors\
        where country='USA' group by movietitle, year order by year desc, movietitle desc limit 800;")
        return list(db_result)
    except:
        print("Se va por except")
        return []

def db_getDirectors(movie):
    try:
        directors = dbConnect().execute("select directorname from imdb_directormovies natural join imdb_directors natural join imdb_movies where movietitle='"+movietitle+"';")
        return list(directors)
    except:
        return []
