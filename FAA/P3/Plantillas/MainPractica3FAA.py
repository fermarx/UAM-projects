from EstrategiaParticionado import *
from ClasificadorRL import *
from ClasificadorSKL import *
from ConfusionMatrix import *
import matplotlib.pyplot as plt


def test_propio():

    dataset_diabetes = Datos('../ConjuntosDatos/pima-indians-diabetes.csv')  
    dataset_wdbc = Datos('../ConjuntosDatos/wdbc.csv')  
    
    tabla_res = create_table()
    epocas = [500, 1000, 1500]
    ctesaprend = [0.25, 0.5, 0.75, 1]

    for epoca in epocas:
        for cteaprend in ctesaprend:
            clasificador = ClasificadorRL(epoca, cteaprend)
            media_error, d_tipica_error = crearValidacion(clasificador, dataset_diabetes)
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "diabetes - Media"] = f"{media_error:.8f}"
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"
    
    for epoca in epocas:
        for cteaprend in ctesaprend:
            clasificador = ClasificadorRL(epoca, cteaprend)
            media_error, d_tipica_error = crearValidacion(clasificador, dataset_wdbc)
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "wdbc - Media"] = f"{media_error:.8f}"
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"

    return tabla_res


def test_SKL():

    vCruzada = KFold(5, shuffle=True)

    # Leemos los datos
    dataset_diabetes = Datos('../ConjuntosDatos/pima-indians-diabetes.csv')  
    X_D = dataset_diabetes.datos[:,[i for i in range(dataset_diabetes.datos.shape[1]-1)]]
    y_D = dataset_diabetes.datos[:,-1]
    dataset_wdbc = Datos('../ConjuntosDatos/wdbc.csv')  
    X_W = dataset_wdbc.datos[:,[i for i in range(dataset_wdbc.datos.shape[1]-1)]]
    y_W = dataset_wdbc.datos[:,-1]

    tabla_res = create_table()
    epocas = [500, 1000, 1500]
    ctesaprend = [0.25, 0.5, 0.75, 1]

    for epoca in epocas:
        for cteaprend in ctesaprend:
            media_error, d_tipica_error = regresion_logistica_SKL(X_D, y_D, epoca, vCruzada, cteaprend)
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "diabetes - Media"] = f"{media_error:.8f}"
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"

    for epoca in epocas:
        for cteaprend in ctesaprend:
            media_error, d_tipica_error = regresion_logistica_SKL(X_W, y_W, epoca, vCruzada, cteaprend)
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "wdbc - Media"] = f"{media_error:.8f}"
            tabla_res.loc[(f"{cteaprend}", f"{epoca}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"

    return tabla_res


def test_ROC():
    dataset_diabetes = Datos('../ConjuntosDatos/pima-indians-diabetes.csv')  
    dataset_wdbc = Datos('../ConjuntosDatos/wdbc.csv')  
    roc_diabetes(dataset_diabetes)
    roc_wdbc(dataset_wdbc)


def roc_diabetes(dataset_diabetes):
    
    clasificadores = [ClasificadorNaiveBayes(False), ClasificadorKNN(True, 11), ClasificadorRL(1000, 1)]

    X = []
    Y = []
    for clasificador in clasificadores:
        tp, fp = matriz_ROC(dataset_diabetes, clasificador)
        X.append(fp)
        Y.append(tp)
    
    plt.figure(figsize=(10,10))
    _, ax = plt.subplots()
    knn = plt.scatter(X[0], Y[0], marker='o', color='r')
    lr = plt.scatter(X[1], Y[1], marker='o', color='g')
    nb = plt.scatter(X[2], Y[2], marker='o', color='b')
    
    ax.plot([0, 1], [0, 1], ls="--", c=".1")
    plt.xlabel("Tasa de Falsos Positivos")
    plt.ylabel("Tasa de Verdaderos Positivos")
    plt.title(f"Espacio ROC pima-indians-diabetes")
    ax.legend((knn, lr, nb), 
    ("KNN", "Regresion Logistica", "Naive Bayes"), scatterpoints=1, loc='lower right', ncol=1, fontsize=8)


def roc_wdbc(dataset_wdbc):

    clasificadores = [ClasificadorNaiveBayes(False), ClasificadorKNN(True, 11), ClasificadorRL(1000, 1)]

    X = []
    Y = []
    for clasificador in clasificadores:
        tp, fp = matriz_ROC(dataset_wdbc, clasificador)
        X.append(fp)
        Y.append(tp)

    plt.figure(figsize=(10,10))
    _, ax = plt.subplots()
    knn = plt.scatter(X[0], Y[0], marker='o', color='r')
    lr = plt.scatter(X[1], Y[1], marker='o', color='g')
    nb = plt.scatter(X[2], Y[2], marker='o', color='b')
    
    ax.plot([0, 1], [0, 1], ls="--", c=".1")
    plt.xlabel("Tasa de Falsos Positivos")
    plt.ylabel("Tasa de Verdaderos Positivos")
    plt.title(f"Espacio ROC wdbc")
    ax.legend((knn, lr, nb), 
    ("KNN", "Regresion Logistica", "Naive Bayes"), scatterpoints=1, loc='lower right', ncol=1, fontsize=8)


def test_curva_ROC():
    dataset_diabetes = Datos('../ConjuntosDatos/pima-indians-diabetes.csv')  
    dataset_wdbc = Datos('../ConjuntosDatos/wdbc.csv')  
    rl = ClasificadorRL(1000, 1)
    vSimple = ValidacionSimple(1, 50)
    curva_roc(dataset_diabetes, rl, vSimple, "diabetes")
    curva_roc(dataset_wdbc, rl, vSimple, "wdbc")
 

def get_confianza(vSimple, dataset, rl):
    particiones = vSimple.creaParticiones(dataset)
    datos_train  = dataset.datos[particiones[0].indicesTrain]
    datos_test = dataset.datos[particiones[0].indicesTest]
    rl.entrenamiento(datos_train, dataset.nominalAtributos, dataset.diccionario)
    return rl.confianza(datos_test)


def curva_roc(dataset, rl, vSimple, name):

    conf = get_confianza(vSimple, dataset, rl)
    
    tp, fp, x, y = 0, 0, [], []
    for score, vp_s, fp_s in sorted(conf, key=lambda x: x[0], reverse=True):
        if vp_s: tp += 1
        elif fp_s: fp += 1
        x.append(tp)
        y.append(fp)

    X, Y = [], []
    for x_z, y_z in zip(x, y):
        X.append(x_z/max(x))
        Y.append(y_z/max(y))

    fig, ax = plt.subplots()
    ax.plot(X,Y)
    ax.set_aspect('equal')
    ax.set_xlim([0, 1])
    ax.set_ylim([0, 1])
    ax.set_title(f"Curva ROC {name}")
    ax.set_xlabel("False Positive Rate")
    ax.set_ylabel("True Positive Rate")


def crearValidacion(clasificador, dataset):

    vc = ValidacionCruzada(5) 
    errores= clasificador.validacion(vc, dataset, clasificador)

    media_error = np.mean(errores)
    d_tipica_error = np.std(np.transpose(np.array(errores)), axis=0)

    return media_error, d_tipica_error
    

def create_table():

    names = ["Cte Aprendizaje", "# epoch"]
    x = ['diabetes - Media', 'diabetes - D. Típica', 'wdbc - Media', 'wdbc - D. Típica']

    y = [("0.25", "500"),
         ("0.25", "1000"),
         ("0.25", "1500"),

         ("0.5", "500"),
         ("0.5", "1000"),
         ("0.5", "1500"),

         ("0.75", "500"),
         ("0.75", "1000"),
         ("0.75", "1500"),

         ("1","500"),
         ("1","1000"),
         ("1", "1500"),]

    col_list = pd.MultiIndex.from_tuples(y, names = names)
    return pd.DataFrame(index=col_list, columns=x, dtype=float)

