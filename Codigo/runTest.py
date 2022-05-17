import os
import shutil

ejecutable = "Codigo.exe"
omnet_ini = "omnetpp.ini"
extra_ini = "extra.ini"

omnet_output = "results"
resultados_nuestros = "resultados"

# Si la carpeta ya existe eliminarla
try:
    shutil.rmtree(resultados_nuestros)
except:
    pass
os.mkdir(resultados_nuestros)

for j in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1]:
    with open(extra_ini, "w") as f_extra_ini:
        f_extra_ini.write(f"Network.nodeTx.gen.generationInterval=exponential({j})")
    os.system(f"{ejecutable} -f {omnet_ini} -f {extra_ini} -u Cmdenv")
    shutil.move(omnet_output, f"{resultados_nuestros}{os.sep}test_geneation={j}")

os.remove(extra_ini)



