import json
import matplotlib.pyplot as plt



def obtener_paquetes_enviadosYRecibidos(json_data: dict[str,]) -> dict[float, (int, int)]:
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


def datos_gráfico_carga(datos_crudos: dict) -> list[(float, float)]:
    """
        Devuelve una lista para hacer un gráfico con:
            Eje x = intervalo de generación
            Eje y = paquetes recibidos/paquetes enviados
    """
    res = []
    for intervalo, (paquetes_enviados, paquetes_recibidos) in datos_crudos.items():
        res.append((intervalo, paquetes_recibidos/paquetes_enviados))
    return res

def generar_grafico(x: list(int), y: list(int)):
    x = [item/200 for item in x] #Paquetes generados entre 200 (tiempo de simulación)
    y = [item/200 for item in y] #Paquetes recibidos entre 200 (tiempo de simulación)

    plt.plot(x, y)
    plt.xlabel("Paquetes enviados")
    plt.ylabel("Paquetes recibidos")
    plt.grid()
    plt.title("Grafico de carga útil")
    plt.savefig("test.svg")


