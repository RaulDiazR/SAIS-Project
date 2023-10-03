import time
from memory_profiler import profile

# Esta función calcula los buckets/cubetas para los caracteres en la cadena de entrada T
def obtenerBuckets(T):
    count = {}  # Un diccionario para contar la ocurrencia de cada carácter
    buckets = {}  # Un diccionario para almacenar los rangos (buckets) de cada carácter
    for c in T:
        count[c] = count.get(c, 0) + 1  # Se cuentan las ocurrencias de cada carácter
    start = 0
    for c in sorted(count.keys()):  # Se ordenan los caracteres
        buckets[c] = (start, start + count[c])  # Se asigna un rango (bucket) a cada carácter
        start += count[c]  # Se actualiza la posición de inicio para el siguiente carácter
    return buckets

# Esta es la función principal del algoritmo SAIS
def sais(T):
    t = ["_"] * len(T)  # Se inicializa un arreglo 't' con la misma longitud que T, lleno de _

    t[len(T) - 1] = "S"  # Se marca el último carácter como "S", pues se sabe que es el carácter sentinela

    # Se etermina si cada carácter es "S" o "L" basándose en comparaciones con el siguiente carácter
    for i in range(len(T) - 1, 0, -1):
        if T[i - 1] == T[i]:
            t[i - 1] = t[i]
        else:
            t[i - 1] = "S" if T[i - 1] < T[i] else "L"

    # S calculan los buckets para los caracteres en la cadena de entrada T
    buckets = obtenerBuckets(T)

    count = {}  # Se inicializa un diccionario para contar los caracteres
    SA = [-1] * len(T)  # Se inicializa el arreglo de sufijos con valores de -1
    LMS = {}  # Se inicializa un diccionario para almacenar las posiciones de las subcadenas LMS
    end = None

    # Se itera a través de la cadena de entrada T hacia atrás
    for i in range(len(T) - 1, 0, -1):
        if t[i] == "S" and t[i - 1] == "L":
            # Para un carácter de tipo S seguido de un carácter de tipo L (subcadena LMS)
            revoffset = count[T[i]] = count.get(T[i], 0) + 1  # Se cuentan las ocurrencias del carácter
            # Se calcula la posición de esta subcadena LMS y se actualiza el arreglo de sufijos
            SA[buckets[T[i]][1] - revoffset] = i

            if end is not None:
                LMS[i] = end  # Se almacena la posición final de esta subcadena LMS
            end = i

    LMS[len(T) - 1] = len(T) - 1  # El último carácter siempre es una subcadena LMS
    count = {}  # Se reinicia el diccionario de conteo

    # Se itera a través del arreglo de sufijos para colocar las subcadenas LMS en sus posiciones correctas
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                # Se calcula la posición de esta subcadena LMS y actualizar el arreglo de sufijos
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1  # Se actualiza el conteo de este carácter

    count = {}  # Se reinicia el diccionario de conteo

    # Se itera a través del arreglo de sufijos nuevamente para refinar las posiciones de los caracteres de tipo S
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                # Se calcula la posición de este carácter de tipo S y actualizar el arreglo de sufijos
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    namesp = [-1] * len(T)  # Se Inicializa una lista para almacenar los nombres de las subcadenas LMS
    name = 0
    prev = None

    # Se asignan nombres únicos a las subcadenas LMS y almacenarlos en 'namesp'
    for i in range(len(SA)):
        if t[SA[i]] == "S" and t[SA[i] - 1] == "L":
            if prev is not None and T[SA[prev]:LMS[SA[prev]]] != T[SA[i]:LMS[SA[i]]]:
                name += 1
            prev = i
            namesp[SA[i]] = name

    names = []
    SApIdx = []

    # Se recopilan los nombres de las subcadenas LMS y sus posiciones
    for i in range(len(T)):
        if namesp[i] != -1:
            names.append(namesp[i])
            SApIdx.append(i)

    # Si hay más de un nombre único, Se llama recursivamente a 'sais' para ordenarlos
    if name < len(names) - 1:
        names = sais(names)

    names = list(reversed(names))  # Se revierten los nombres ordenados

    SA = [-1] * len(T)  # Se reinicia el arreglo de sufijos
    count = {}  # Se reinicia el diccionario de conteo

    # Se colocan las subcadenas LMS en sus posiciones correctas en el arreglo de sufijos
    for i in range(len(names)):
        pos = SApIdx[names[i]]
        revoffset = count[T[pos]] = count.get(T[pos], 0) + 1
        SA[buckets[T[pos]][1] - revoffset] = pos

    count = {}  # Se reinicia el diccionario de conteo

    # Se refinan las posiciones de los caracteres de tipo S en el arreglo de sufijos
    for i in range(len(T)):
        if SA[i] >= 0:
            if t[SA[i] - 1] == "L":
                symbol = T[SA[i] - 1]
                offset = count.get(symbol, 0)
                # Se calcula la posición de este carácter de tipo S y actualizar el arreglo de sufijos
                SA[buckets[symbol][0] + offset] = SA[i] - 1
                count[symbol] = offset + 1

    count = {}  # Se reinicia el diccionario de conteo

    # Se refinan las posiciones de los caracteres de tipo S nuevamente en el arreglo de sufijos
    for i in range(len(T) - 1, 0, -1):
        if SA[i] > 0:
            if t[SA[i] - 1] == "S":
                symbol = T[SA[i] - 1]
                revoffset = count[symbol] = count.get(symbol, 0) + 1
                # Se calcula la posición de este carácter de tipo S y actualizar el arreglo de sufijos
                SA[buckets[symbol][1] - revoffset] = SA[i] - 1

    return SA  # Se devuelve el arreglo de sufijos generado

# Función para leer un archivo y devolver su contenido como una cadena
def leer_archivo(nombre_archivo):
    try:
        with open(nombre_archivo, 'r', encoding='utf-8') as archivo:
            contenido = archivo.read()
        return contenido
    except FileNotFoundError:
        print(f"Archivo '{nombre_archivo}' no encontrado.")
        return None

# Se obtiene el nombre del archivo de entrada del usuario
nombre_archivo = "./Libros/book4.txt"

# Se lee el contenido del archivo
contenido_archivo = leer_archivo(nombre_archivo)

if contenido_archivo:

    # Se convierte el contenido del archivo en una lista de códigos de caracteres
    T = [ord(c) for c in contenido_archivo]
    T.append(0)

    # Se genera el arreglo de sufijos para la cadena de entrada
    SA = sais(T)
