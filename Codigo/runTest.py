import os
import shutil

# Executable extention

ejecutable = "Codigo"
omnet_ini = "omnetpp.ini"
extra_ini = "extra.ini"

omnet_output = "results"
resultados_nuestros = "resultados"

# Si la carpeta ya existe eliminarla
if os.path.exists(resultados_nuestros):
    shutil.rmtree(resultados_nuestros)
os.mkdir(resultados_nuestros)

for j in [0.1, 0.4, 0.7, 1]:
    with open(extra_ini, "w") as f_extra_ini:
        f_extra_ini.write(f"Network.nodeTx.gen.generationInterval=exponential({j})")
    os.system(f".{os.sep}{ejecutable} -f {omnet_ini} -f {extra_ini} -u Cmdenv")
    shutil.move(omnet_output, f"{resultados_nuestros}{os.sep}test_geneation={j}")

os.remove(extra_ini)



