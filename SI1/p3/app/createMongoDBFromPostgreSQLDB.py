# primero este script luego app par que lea de la 
# db (en mongodb) creada por este script 
# utiliza python y sqlalchemy para acceder a postgres
# utliliza pymongo para interactucar con mongodb
import os
import pymongo
import sys
import inspect
from sqlalchemy import create_engine
from app import database

dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
dir2 = os.path.dirname(dir)
sys.path.insert(0, dir2)

#db_topUSA = Table('topUSA', db_meta, autoload=True, autoload_with=db_engine)

client = pymongo.MongoClient('mongodb://localhost:27017/')
db = client['si1']
if "si1" in client.list_database_names():
  db.collection.drop()
  client.drop_database('si1')
else:
  print('Base de datos creada')

collection = db['topUSA']
films = database.db_topUSA() # crear una tabla con las topUSA

for film in films:
  dic = {'title': film[0],
         'year': film[1],
         'genres': film[2],
         'directors': "",
         'actors': film[3]
  }
  directors=database.db_getDirectors(film[0])
  if directors:
    for director in directors:
      dic["directors"]=dic["directors"]+str(director)
    else:
      dic["directors"]="Sin director"
  col = collection.insert_one(dic)

def mongo_life():
  try:
    life=db.topUSA.find({'title':{'$regrex': 'Life'}},{'_id':0})
    return list(life)
  except:
    return[]

def mongo_woodyAllen():
  try:
    allen=db.topUSA.find({'directors': 'Allen, Woody', 'year':{'$regrex': '199'}},{'_id':0})
    return list(allen)
  except:
    return []

def mongo_parsonsGalecki():
  try:
    galecki=db.topUSA.find({'$all':[{'actors':{'$regrex': 'Galecki, Johnny'}}, {'actors':{'$regrex': 'Parsons, Jim (II)'}}]},{'_id':0})
    return list(galecki)
  except:
    return []
