from EstrategiaParticionado import *
from Clasificador import *
from ClasificadorNBSKL import *
from sklearn.preprocessing import OneHotEncoder


def test_propio():

    numParticiones = 5
    numeroEjec = 2
    porTest = 30
    
    tabla_res = create_table()
    clasificador = ClasificadorNaiveBayes()
    # Without Laplace Tic-tac-toe
    dataset = Datos('../ConjuntosDatosIntroFAA/tic-tac-toe.csv')  
    media_error, d_tipica_error = testVS("Tic-tac-toe", clasificador, dataset, 0, numeroEjec, porTest)
    tabla_res.loc[("Simple", "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Simple", "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    media_error, d_tipica_error = testVC("Tic-tac-toe", clasificador, dataset, 0, numParticiones)
    tabla_res.loc[("Cruzada", "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Cruzada", "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    # With Laplace Tic-tac-toe
    media_error, d_tipica_error = testVS("Tic-tac-toe", clasificador, dataset, 1, numeroEjec, porTest)
    tabla_res.loc[("Simple", "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Simple", "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    media_error, d_tipica_error = testVC("Tic-tac-toe", clasificador, dataset, 1, numParticiones)
    tabla_res.loc[("Cruzada", "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Cruzada", "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    clasificador = ClasificadorNaiveBayes()
    # Without Laplace German
    dataset = Datos('../ConjuntosDatosIntroFAA/german.csv')
    media_error, d_tipica_error = testVS("German", clasificador, dataset, 0, numeroEjec, porTest)
    tabla_res.loc[("Simple", "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Simple", "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    media_error, d_tipica_error = testVC("German", clasificador, dataset, 0, numParticiones)
    tabla_res.loc[("Cruzada", "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Cruzada", "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    # With Laplace German
    media_error, d_tipica_error = testVS("German", clasificador, dataset, 1, numeroEjec, porTest)
    tabla_res.loc[("Simple", "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Simple", "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    media_error, d_tipica_error = testVC("German", clasificador, dataset, 1, numParticiones)
    tabla_res.loc[("Cruzada", "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("Cruzada", "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    return tabla_res


def test_SKLearn():
    
    numParticiones = 5
    porTest = 0.3

    vSimple, vCruzada, X_T, y_T, X_G, y_G = iniciarSKL(numParticiones, porTest)
    tabla_res = create_table_SKL()

    # Multinomial
    multinomial_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res)

    # Gaussian
    gaussian_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res)

    # Categorical
    categorical_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res)

    return tabla_res


def test_SKLearn_1HE():
    
    numParticiones = 5
    porTest = 0.3

    vSimple, vCruzada, X_T, y_T, X_G, y_G = iniciarSKL(numParticiones, porTest)
    tabla_res = create_table_SKL1HE()
    
    encoder = OneHotEncoder(sparse=False)
    X_T1HE = encoder.fit_transform(X_T)

    # Multinomial
    multinomial_test_1HE(vSimple, vCruzada, X_T1HE, y_T, tabla_res)

    # Gaussian
    gaussian_test_1HE(vSimple, vCruzada, X_T1HE, y_T, tabla_res)

    # Categorical
    categorical_test_1HE(vSimple, vCruzada, X_T1HE, y_T, tabla_res)

    return tabla_res


def testVS(name, clasificador, dataset, laplace, numeroEjec, porTest):

    estrategiaVS = ValidacionSimple(numeroEjec, porTest)
    errores = clasificador.validacion(estrategiaVS,dataset,clasificador, laplace)

    media_error = np.mean(errores)
    d_tipica_error = np.std(np.transpose(np.array(errores)), axis=0)

    return media_error, d_tipica_error


def testVC(name, clasificador, dataset, laplace, numParticiones):

    estrategiaVC = ValidacionCruzada(numParticiones)
    errores = clasificador.validacion(estrategiaVC,dataset,clasificador, laplace)
    
    media_error = np.mean(errores)
    d_tipica_error = np.std(np.transpose(np.array(errores)), axis=0)
    
    return media_error, d_tipica_error


def create_table():  
    
    names = ["Validación", "Laplace"]
    x = ['Tic-tac-toe - Media', 'Tic-tac-toe - D. Típica', 'German - Media', 'German - D. Típica']
    y = [("Simple", "Si"),
         ("Simple", "No"),
         ("Cruzada", "Si"),
         ("Cruzada", "No")]

    col_list = pd.MultiIndex.from_tuples(y, names = names)
    return pd.DataFrame(index=col_list, columns=x, dtype=float)


def create_table_SKL():

    x = ['Tic-tac-toe - Media', 'Tic-tac-toe - D. Típica', 'German - Media', 'German - D. Típica']
    y = [("CategoricalNB", "Cruzada", "Si"),
        ("CategoricalNB", "Cruzada", "No"),
        ("CategoricalNB", "Simple", "Si"),
        ("CategoricalNB", "Simple", "No"),
        
        ("GaussianNB", "Cruzada", "nan"),
        ("GaussianNB", "Simple", "nan"),
        
        ("MultinomialNB", "Cruzada", "Si"),
        ("MultinomialNB", "Cruzada", "No"),
        ("MultinomialNB", "Simple", "Si"),
        ("MultinomialNB", "Simple", "No"),]

    names = ["Classifier", "Validación", "Laplace"]

    col_list = pd.MultiIndex.from_tuples(y, names = names)

    return pd.DataFrame(index=col_list, columns=x, dtype=float)


def create_table_SKL1HE():

    x = ["Media", "D. Típica"]
    y = [("CategoricalNB", "Cruzada", "Si"),
        ("CategoricalNB", "Cruzada", "No"),
        ("CategoricalNB", "Simple", "Si"),
        ("CategoricalNB", "Simple", "No"),
        
        ("GaussianNB", "Cruzada", "nan"),
        ("GaussianNB", "Simple", "nan"),
        
        ("MultinomialNB", "Cruzada", "Si"),
        ("MultinomialNB", "Cruzada", "No"),
        ("MultinomialNB", "Simple", "Si"),
        ("MultinomialNB", "Simple", "No"),]

    names = ["Classifier", "Validación", "Laplace"]

    col_list = pd.MultiIndex.from_tuples(y, names = names)

    return pd.DataFrame(index=col_list, columns=x, dtype=float)


def multinomial_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res):
   
    # Multinomial Tic-tac-toe without Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vSimple, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vCruzada, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    
    # Multinomial Tic-tac-toe with Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vSimple, 1)
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vCruzada, 1)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    # Multinomial German without Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_G, y_G, vSimple, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_G, y_G, vCruzada, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    # Multinomial German with Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_G, y_G, vSimple, 1)
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_G, y_G, vCruzada, 1)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"


def gaussian_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res):

    # Gaussian Tic-tac-toe
    media_error, d_tipica_error = GaussianNBSKL(X_T, y_T, vSimple)
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = GaussianNBSKL(X_T, y_T, vCruzada)
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    # Gaussian German
    media_error, d_tipica_error = GaussianNBSKL(X_G, y_G, vSimple)
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "German - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = GaussianNBSKL(X_G, y_G, vCruzada)
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "German - D. Típica"] = f"{d_tipica_error:.8f}"


def categorical_test(vSimple, vCruzada, X_T, y_T, X_G, y_G, tabla_res):
    
    # Categorical Tic-tac-toe without Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vSimple, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vCruzada, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    
    # Categorical Tic-tac-toe with Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vSimple, 1)
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vCruzada, 1)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "Tic-tac-toe - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "Tic-tac-toe - D. Típica"] = f"{d_tipica_error:.8f}"

    # Categorical German without Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_G, y_G, vSimple, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_G, y_G, vCruzada, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "German - D. Típica"] = f"{d_tipica_error:.8f}"

    # Categorical German with Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_G, y_G, vSimple, 1)
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_G, y_G, vCruzada, 1)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "German - Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "German - D. Típica"] = f"{d_tipica_error:.8f}"


def multinomial_test_1HE(vSimple, vCruzada, X_T, y_T, tabla_res):
    
    # Multinomial Tic-tac-toe without Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vSimple, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "No"), "D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vCruzada, 1.0e-10)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "No"), "D. Típica"] = f"{d_tipica_error:.8f}"
    
    # Multinomial Tic-tac-toe with Laplace
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vSimple, 1)
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Simple', "Si"), "D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = MultinomialNBSKL(X_T, y_T, vCruzada, 1)
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("MultinomialNB", 'Cruzada', "Si"), "D. Típica"] = f"{d_tipica_error:.8f}"


def gaussian_test_1HE(vSimple, vCruzada, X_T, y_T, tabla_res):
    
    # Gaussian Tic-tac-toe
    media_error, d_tipica_error = GaussianNBSKL(X_T, y_T, vSimple)
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Simple', "nan"), "D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = GaussianNBSKL(X_T, y_T, vCruzada)
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("GaussianNB", 'Cruzada', "nan"), "D. Típica"] = f"{d_tipica_error:.8f}"


def categorical_test_1HE(vSimple, vCruzada, X_T, y_T, tabla_res):
    
    # Categorical Tic-tac-toe without Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vSimple, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "No"), "D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vCruzada, 1.0e-10)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "No"), "D. Típica"] = f"{d_tipica_error:.8f}"
    
    # Categorical Tic-tac-toe with Laplace
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vSimple, 1)
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Simple', "Si"), "D. Típica"] = f"{d_tipica_error:.8f}"
    media_error, d_tipica_error = CategoricalNBSKL(X_T, y_T, vCruzada, 1)
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "Media"] = f"{media_error:.8f}"
    tabla_res.loc[("CategoricalNB", 'Cruzada', "Si"), "D. Típica"] = f"{d_tipica_error:.8f}"
    

test_propio()