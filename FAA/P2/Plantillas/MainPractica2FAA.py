from EstrategiaParticionado import *
from ClasificadorKNN import *
from ClusteringKMeans import *
from ClasificadorSKL import *
from matriz import *

def test_propio():

    dataset_diabetes = Datos('../ConjuntosDatosP2/pima-indians-diabetes.csv')  
    dataset_wdbc = Datos('../ConjuntosDatosP2/wdbc.csv')  
    
    tabla_res = create_table()

    k_lista = [5, 7, 10, 15, 17, 20]
    #Dataset diabetes
    for k in k_lista:
        knn_norm = ClasificadorKNN(True, k)
        media_error, d_tipica_error = crearValidacion(knn_norm, dataset_diabetes)
        tabla_res.loc[("True", f"{k}"), "diabetes - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("True", f"{k}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"
        print(media_error, d_tipica_error)

    for k in k_lista:
        knn_not_norm = ClasificadorKNN(False, k)
        media_error, d_tipica_error = crearValidacion(knn_not_norm, dataset_diabetes)
        tabla_res.loc[("False", f"{k}"), "diabetes - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("False", f"{k}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"
        print(media_error, d_tipica_error)


    # Dataset wdbc
    for k in k_lista:
        knn_norm = ClasificadorKNN(True, k)
        media_error, d_tipica_error = crearValidacion(knn_norm, dataset_wdbc)
        tabla_res.loc[("True", f"{k}"), "wdbc - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("True", f"{k}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"
        print(media_error, d_tipica_error)


    for k in k_lista:
        knn_not_norm = ClasificadorKNN(False, k)
        media_error, d_tipica_error = crearValidacion(knn_not_norm, dataset_wdbc)
        tabla_res.loc[("False", f"{k}"), "wdbc - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("False", f"{k}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"
        print(media_error, d_tipica_error)

    print(tabla_res)

    return tabla_res


def test_KMeans():
    dataset = Datos('../ConjuntosDatosP2/iris.csv')  

    k_list = [3, 1, 2, 4, 5]
    for k in k_list:
        clusters, diccionario_cluster, n = kmeans(k, dataset)
        print(f"Para K={k}, numero de iteraciones: {n}")
        for cluster, puntos in diccionario_cluster.items():
            for cl, centroide in clusters.items():
                if cl == cluster:
                    print(f"\t{len(puntos)} puntos pertenecen al cluster {cluster} con centroide {centroide}")
        if k == 3:
            matriz=matriz_confusion_diccionario(dataset.datos, diccionario_cluster, 3)
            print(matriz)


def test_SKL():
    
    vCruzada = KFold(5, shuffle=True)

    # Leemos los datos
    dataset_diabetes = Datos('../ConjuntosDatosP2/pima-indians-diabetes.csv') 
    X_D = dataset_diabetes.datos[:,[i for i in range(dataset_diabetes.datos.shape[1]-1)]]
    y_D = dataset_diabetes.datos[:,-1]
    dataset_wdbc = Datos('../ConjuntosDatosP2/wdbc.csv')  
    X_W = dataset_wdbc.datos[:,[i for i in range(dataset_wdbc.datos.shape[1]-1)]]
    y_W = dataset_wdbc.datos[:,-1]

    tabla_res = create_table()

    k_list = [5, 7, 10, 15, 17, 20]
    #Dataset diabetes
    for k in k_list:
        media_error, d_tipica_error = vecinos_proximos_SKL(X_D, y_D, k, vCruzada, True)
        tabla_res.loc[("True", f"{k}"), "diabetes - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("True", f"{k}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"

    for k in k_list:
        media_error, d_tipica_error = vecinos_proximos_SKL(X_D, y_D, k, vCruzada, False)
        tabla_res.loc[("False", f"{k}"), "diabetes - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("False", f"{k}"), "diabetes - D. Típica"] = f"{d_tipica_error:.8f}"
    
    # Dataset wdbc
    for k in k_list:
        media_error, d_tipica_error = vecinos_proximos_SKL(X_W, y_W, k, vCruzada, True)
        tabla_res.loc[("True", f"{k}"), "wdbc - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("True", f"{k}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"

    for k in k_list:
        media_error, d_tipica_error = vecinos_proximos_SKL(X_W, y_W, k, vCruzada, False)
        tabla_res.loc[("False", f"{k}"), "wdbc - Media"] = f"{media_error:.8f}"
        tabla_res.loc[("False", f"{k}"), "wdbc - D. Típica"] = f"{d_tipica_error:.8f}"

    return tabla_res


def test_KmeansSLK():
    dataset = Datos('../ConjuntosDatosP2/iris.csv')  
    
    k_list = [3, 1, 2, 4, 5]
    for k in k_list:
        centroids, pred = kmeansSKL(k, dataset)
        
        print(f"Para K={k} clusters, se han encontrado")
        i=0
        for centroid in centroids:
            num=0
            for p in pred:
                if p == i: num+=1
            print(f"\tCentroide {i}: {centroid} con {num} puntos pertenecientes")
            i+=1


def crearValidacion(clasificador, dataset):

    vc = ValidacionCruzada(5) 
    errores= clasificador.validacion(vc, dataset, clasificador,0)

    media_error = np.mean(errores)
    d_tipica_error = np.std(np.transpose(np.array(errores)), axis=0)

    return media_error, d_tipica_error
    

def create_table():

    names = ["Normalizado", "#K"]
    x = ['diabetes - Media', 'diabetes - D. Típica', 'wdbc - Media', 'wdbc - D. Típica']

    y = [("True", "5"),
         ("True", "7"),
         ("True", "10"),
         ("True", "15"),
         ("True", "17"),
         ("True", "20"),

         ("False", "5"),
         ("False", "7"),
         ("False", "10"),
         ("False", "15"),
         ("False", "17"),
         ("False", "20"),]

    col_list = pd.MultiIndex.from_tuples(y, names = names)
    return pd.DataFrame(index=col_list, columns=x, dtype=float)

