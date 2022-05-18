import os
import shutil
import sys

carpeta_código = "Codigo"

generation_intervals = [0.1, 0.2, 0.3]

resultados_nuestros = "Resultados"

def carpeta_resultados(genInter: float, parte: int, sep = os.sep):
    return f"{resultados_nuestros}_parte{parte}{sep}sim_geneation={genInter}"

def correr_simulaciones(parte: int):
    """
        Corre las simulaciones, usa el argumento para el nombre de la carpeta de los resultados
    """
    ejecutable = f"{carpeta_código}{os.sep}{carpeta_código}" # El make hace que el ejecutable tenga el mismo nombre que la carpeta en la que está
    if os.name == "nt": # En windows agregar .exe
        ejecutable += ".exe"

    omnet_ini = f"{carpeta_código}{os.sep}omnetpp.ini"
    extra_ini = f"{carpeta_código}{os.sep}extra.ini"

    omnet_output = f"{carpeta_código}{os.sep}results"

    # Chequer que existan los archivos y carpetas necesarios
    if not os.path.exists(ejecutable):
        raise Exception(f"No se encuentra '{ejecutable}'")
    if not os.path.exists(omnet_ini):
        raise Exception(f"No se encuentra '{omnet_ini}'")

    # Si la carpeta ya existe eliminarla
    if os.path.exists(resultados_nuestros):
        shutil.rmtree(resultados_nuestros)
    os.mkdir(resultados_nuestros)

    for j in generation_intervals:
        with open(extra_ini, "w") as f_extra_ini:
            f_extra_ini.write(f"Network.nodeTx.gen.generationInterval=exponential({j})")

        # Ejecutar la simulación
        x = os.system(f".{os.sep}{ejecutable} -f {omnet_ini} -f {extra_ini} -n {carpeta_código} -u Cmdenv")
        if x != 0:
            raise Exception(f"Error al ejecutar simulación '{ejecutable}'")

        shutil.move(omnet_output, carpeta_resultados(j, parte))

    os.remove(extra_ini)

def exportar_graficos(parte: int):
    """
        Crea los gráficos, usa el argumento para el nombre de la carpeta de los resultados
    """
    general_anf = "General.anf"
    carpeta_gráficos = f"Gráficos_parte{parte}"

    # Chequear que exista el archivo anf
    if not os.path.exists(general_anf):
        raise Exception(f"No se encuentra '{general_anf}'")

    # Si la carpeta ya existe eliminarla
    if os.path.exists(carpeta_gráficos):
        shutil.rmtree(carpeta_gráficos)

    # Crear la carpeta
    os.mkdir(carpeta_gráficos)

    with open(general_anf, "r") as f_general_anf:
        lineas = f_general_anf.readlines()

    # Verificar que las lineas que se van a modificar sean de adentro del input
    assert lineas[2] == "    <inputs>\n" and lineas[5] == "    </inputs>\n"

    for j in generation_intervals:
        # modificar <input pattern="..."/> en general_anf
        lineas[3] = f'        <input pattern="{carpeta_resultados(j, parte, sep="/")}/General-*.vec"/>\n'
        lineas[4] = f'        <input pattern="{carpeta_resultados(j, parte, sep="/")}/General-*.sca"/>\n'
        with open(general_anf, "w") as f_general_anf:
            # Escribir las lineas
            f_general_anf.writelines(lineas)

        # Crear los gráficos
        x = os.system(f"opp_charttool imageexport {general_anf}")
        assert x == 0

        # Mover los gráficos a la carpeta de gráficos
        svg_files = filter(lambda x: x.endswith(".svg"), os.listdir())
        for svg_file in svg_files:
            shutil.move(svg_file, f"{carpeta_gráficos}{os.sep}SIM_gen={j}_{svg_file}")

def main(args: list):
    match args:
        case ["parte1"]:
            parte = 1
        case ["parte2"]:
            parte = 2
        case _:
            print("USO: python3 runTest.py [parte1, parte2]")
            raise Exception("Argumentos inválidos")
    correr_simulaciones(parte)
    exportar_graficos(parte)

if __name__ == "__main__":
    # Obtener argumentos
    args = sys.argv[1:]
    main(args)

