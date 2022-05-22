import json
from typing import Tuple
import matplotlib.pyplot as plt
import os

import runSimulations


# (json_data: dict[str,]) -> dict[float, (int, int)]
def obtener_paquetes_enviadosYRecibidos(json_data: dict) -> dict:
    """
        Obtiene los paquetes enviados en cada simulación
        Devuelve un diccionario con clave intervalo generación y valor (paquetes enviados, paquetes recibidos)
    
        Toma la json_data que fue generado por omnet con `opp_scavetool` (y prosesada con json.loads)
    """
    res = {}
    for sim in json_data.values():

        generationInterval_text = sim["config"][9]["Network.nodeTx.gen.generationInterval"]
        # Chequear que se haya obtenido mas o menos lo correcto
        assert generationInterval_text.startswith("exponential(") and generationInterval_text.endswith(")")
        generationInterval_number_text = generationInterval_text[12:-1]
        generationInterval = float(generationInterval_number_text)

        # Obtener paquetes enviados y recibidos
        assert sim["scalars"][0]["name"] == "Sent packets"
        paquetes_enviados = int(sim["scalars"][0]["value"])
        assert sim["scalars"][3]["name"] == "Received packets"
        paquetes_recibidos = int(sim["scalars"][3]["value"])


        res[generationInterval] = (paquetes_enviados, paquetes_recibidos)
    
    return res

# (datos: dict[float, (int, int)]) -> Tuple[list[float], list[float]]
def datos_gráfico_aprovechamiento(datos: dict) -> Tuple:
    """
        Devuelve una lista para hacer un gráfico con:
            Eje x = intervalo de generación
            Eje y = paquetes recibidos/paquetes enviados
    """
    # ordenar por clave
    xys = sorted(datos.items(), key=lambda x: x[0])
    res = ([], [])
    for intervalo, (paquetes_enviados, paquetes_recibidos) in xys:
        res[0].append(intervalo)
        res[1].append(paquetes_recibidos/paquetes_enviados)
    return res

# (xs: list(float), ys: list(float))
def generar_gráfico_GI_VS_aprovechamiento(xs: list, ys: list, nombre_simulación: str):
    plt.clf()
    plt.plot(xs, ys)
    plt.xlabel("Intervalo de generación")
    plt.ylabel("paquetes recibidos / paquetes enviados")
    plt.grid()
    plt.title("Gráfico de intervalo de generación vs aprovechamiento")
    plt.savefig(f"{runSimulations.carpeta_gráficos(nombre_simulación)}{os.sep}Gráfico de intervalo de generación vs aprovechamiento.svg")


# (datos: dict[float, (int, int)]) -> Tuple[list[int], list[int]]
def datos_gráfico_enviados_VS_recibidos(datos: dict) -> Tuple:
    # ordenar datos por valor de x
    xys = sorted(datos.values(), key=lambda x: x[0])
    return map(lambda xy: xy[0], xys), map(lambda xy: xy[1], xys)

# (xs: list(int), ys: list(int))
def generar_gráfico_enviadosVSrecibidos(xs: list, ys: list, nombre_simulación: str):
    xs = [item/200 for item in xs] #Paquetes generados entre 200 (tiempo de simulación)
    ys = [item/200 for item in ys] #Paquetes recibidos entre 200 (tiempo de simulación)

    plt.clf()
    plt.scatter(xs, ys)
    plt.xlabel("Paquetes enviados por segundo")
    plt.ylabel("Paquetes recibidos por segundo")
    plt.grid()
    plt.title("Gráfico de carga útil")
    plt.savefig(f"{runSimulations.carpeta_gráficos(nombre_simulación)}{os.sep}Gráfico de carga útil.svg")



def gráficos(json_file: str, nombre_simulación: str):
    with open(json_file) as f:
        json_data = json.load(f)
    datos = obtener_paquetes_enviadosYRecibidos(json_data)
    xs, ys = datos_gráfico_enviados_VS_recibidos(datos)
    generar_gráfico_enviadosVSrecibidos(xs, ys, nombre_simulación)
    xs, ys = datos_gráfico_aprovechamiento(datos)
    generar_gráfico_GI_VS_aprovechamiento(xs, ys, nombre_simulación)





