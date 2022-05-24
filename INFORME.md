# Simulación de un productor consumidor a través de un una red simple en omnet++

#### Resumen:

    En este trabajo se hicieron simulaciones de productor consumidor a través de una red que solo cuenta con un buffer intermedio, pero que se puede llenar y perder paquetes.

    En la primera parte se realizaron mediciones sin tener ningún algoritmo de control de flujo y congestión y en la segunda parte se implementó un algoritmo simple de control de flujo y congestión.

#### Autores:

- Fuentes, Tiffany
- Renison, Iván
- Schachner, Álvaro

## Índice

## Introducción

    En este trabajo se analizará una red muy simple que consta de un generador y un receptor conectados por un nodo intermedio que tiene un buffer. El esquema de esta red sería así:

![Captura de pantalla de 2022-05-19 12-09-37.png](./Imágenes%20informe/Esquema%20de%20la%20red%20parte%201.png)

    Aquí `nodeTx` es el generador y `nodeRx` es el receptor. Los canales que conectan a `nodeTx` con la cola y a la cola con `nodeRx` tienen una cantidad máxima de paquetes por segundo que se pueden mandar y un tiempo que se demora en llegar la información (que puede ser distinta en cada uno).

#### `nodeTx`:

    `nodeTx` tiene dos componentes, un generador, que sería como la capa de aplicación y una cola que sería como la capa de transporte.

    El generador lo que hace es simplemente generar paquetes cada cierta cantidad de tiempo aleatoria según alguna distribución.

    La cola se encarga de ir mandando los paquetes a medida que va pudiendo y cuando no puede los almacena en un buffer hasta que puede mandarlos.

#### `nodeRx`:

    `nodeRx` también tiene dos componentes, un consumidor, que sería como la capa de aplicación y una cola que sería como la capa de transporte.

    El consumidor lo que hace es consumir los paquetes a una cierta velocidad fija.

    La cola se encarga de recibir los paquetes y almacenarlos en un buffer hasta que el consumidor los consuma y que si se llena, descarta los nuevos paquetes que llegan y no entran.

#### La cola intermedia:

    La cola intermedia es simplemente como un router, que recibe mensajes de una de su entrada y los envía a la salida a medida que va pudiendo.

    Cuenta con un buffer para almacenar los paquetes cuando le llegan a más velocidad de la que los puede mandar y que cuando se llena, la cola descarta los nuevos paquetes que van llegando.

### Variables

    Con todo eso, queda una gran cantidad de variables en el sistema. A continuación una lista de las variables mas importante:

- Intervalo de generación: distribución del tiempo de cada cuanto se genera un paquete nuevo.

- Velocidad de consumo: La velocidad máxima a la que el consumidor puede consumir los paquetes.

- Capacidad de los links: la cantidad de paquetes por segundo que pueden pasar por cada uno de estos los links (de `nodeTx` a queue y de queue a `nodeRx`) (son variables distintas).

- Capacidad de los buffers: La cantidad de paquetes que puede haber en los distintos buffers. Hay tres buffers en total, el que está en `nodeTx`, el de la cola del medio y el de `nodeRx`, sin embargo, el buffer de `nodeTx` será infinito y así solo son limitantes los otros dos buffers.

    También hay algunas otras variables, como el tiempo de procesamiento de los paquetes o velocidad de propagación, a las cuales no se les dará mucha importancia.

### Experimentos

    Para hacer algunos experimentos, primero se fijaron algunas variables:

- El tamaño del buffer de la cola del medio y del buffer de `nodeRx` se fijó en 200 paquetes.

- La capacidad del canal entre `nodeTx` y la cola del medio se fijó en 10 paquetes por segundo.

    Y otros parámetros se hicieron variar:

- Con las capacidad del canal entre la cola del medio y `nodeRx` y la velocidad de consumo se hicieron dos casos:

      En el primer caso, la capacidad del canal entre la cola del medio y `nodeRx` es 10 paquetes por segundo y la velocidad de consumo es 5 paquetes por segundo.

      En el segundo caso, la capacidad del canal entre la cola del medio y `nodeRx` es 5 paquetes por segundo y la velocidad de consumo del receptor es 10 paquetes por segundo.

- Al intervalo de generación se le asignó una distribución ε(λ) (exponencial de media λ) con λ variando entre 0 y 1.

    Al hacer estos experimentos se tomaron varias mediciones:

- Se midió la cantidad de paquetes generados por el generador, recibidos por el consumidor y descartados en las colas.

- Se midió la cantidad de paquetes en cada buffer en cada momento.

- Se midió la demora entre que un paquete se genera y que se recibe para cada paquete que se recibe.

    A continuación, los gráficos de paquetes en cada buffer para cada caso para un intervalo de generación de paquetes de distribución ε(0.2):

| Caso 1                                                                                        | Caso 2                                                                                        |
| --------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------- |
| ![Buffers_sizes_generation=0.2.svg](./Gráficos_parte1_caso1/Buffers_sizes_generation=0.2.svg) | ![Buffers_sizes_generation=0.2.svg](./Gráficos_parte1_caso2/Buffers_sizes_generation=0.2.svg) |

    Se puede ver que estos dos gráficos son muy parecidos. La principal diferencia que tienen es que el buffer de la cola del medio y el buffer del consumidor están intercambiados. Esto posiblemente se deba a que los casos son análogos, siendo la diferencia entre ellos, que en el primero el cuello de botella es la velocidad de consumo en el receptor y en el segundo es la velocidad de trasmisión entre la cola del medio y el consumidor.

    Los gráficos de los paquetes enviados - recibidos, y descartados - de retraso son prácticamente iguales para ambos casos, así que solo se mostrarán los gráficos del caso 1.

    A continuación los gráficos de las cosas medidas para λ = 0.1, 0.2, 0.3:

| λ = 0.1                                                                                                           | λ = 0.2                                                                                                           | λ = 0.3                                                                                                           |
| ----------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------- |
| ![Buffers_sizes_generation=0.1.svg](./Gráficos_parte1_caso1/Buffers_sizes_generation=0.1.svg)                     | ![Buffers_sizes_generation=0.2.svg](./Gráficos_parte1_caso1/Buffers_sizes_generation=0.2.svg)                     | ![Buffers_sizes_generation=0.3.svg](./Gráficos_parte1_caso1/Buffers_sizes_generation=0.3.svg)                     |
| ![Sent_vs_recived_packets_generation=0.1.svg](./Gráficos_parte1_caso1/Sent_vs_recived_packets_generation=0.1.svg) | ![Sent_vs_recived_packets_generation=0.2.svg](./Gráficos_parte1_caso1/Sent_vs_recived_packets_generation=0.2.svg) | ![Sent_vs_recived_packets_generation=0.3.svg](./Gráficos_parte1_caso1/Sent_vs_recived_packets_generation=0.3.svg) |
| ![Resived_packet_delay_generation=0.1.svg](./Gráficos_parte1_caso1/Resived_packet_delay_generation=0.1.svg)       | ![Resived_packet_delay_generation=0.2.svg](./Gráficos_parte1_caso1/Resived_packet_delay_generation=0.2.svg)       | ![Resived_packet_delay_generation=0.3.svg](./Gráficos_parte1_caso1/Resived_packet_delay_generation=0.3.svg)       |

    El motivo porque el cuál se eligieron los valores 0.1, 0.2 y 0.3 es que en 0.1 la limitante principal ya es la salida del generador y al achicarlo mas se vuelve aun más notorio eso y en 0.3 la red ya se llegan a enviar bien todos los paquetes y aumentarlo más solo hace que esté mas sobrada la red.

    Esto se puede ver mejor en gráficos que comparen los paquetes enviados, recibidos y el intervalo de generación.

    El siguiente gráfico compara la cantidad de paquetes enviados con respecto a la cantidad de paquetes recibidos por segundo (la cantidad de paquetes enviados aumenta cuando disminuye el intervalo de generación).

![Gráfico de carga útil.svg](./Gráficos_parte1_caso1/Gráfico%20de%20carga%20útil.svg)

    Y este gráfico es de la proporción de paquetes que llegan según el intervalo de generación.

![Gráfico de intervalo de generación vs aprovechamiento.svg](./Gráficos_parte1_caso1/Gráfico%20de%20intervalo%20de%20generación%20vs%20aprovechamiento.svg)

    En ambos gráficos se puede ver como aumentar el intervalo de generación hace que la cantidad de paquetes enviados y recibidos sea casi igual, mientras que cuando se disminuye es mayor la diferencia.

## Métodos

    Para comenzar con la solución del problema de los paquetes, se modificó el esquema de red, esta vez con un canal de comunicación exclusivo entre `nodeTx` y `nodeRx`, para que `nodeRx` le envíe mensajes de tipo "feedback" a `nodeTx`. El nuevo esquema tendrá la siguiente forma:

![Captura de pantalla de 2022-05-19 12-09-37.png](./Imágenes%20informe/Esquema%20de%20la%20red%20parte%202.png)

    Surgieron varias formas para solucionar la pérdida de paquetes por problemas de flujo (buffer del receptor lleno) y de congestión (buffer en la cola intermedia lleno). Algunas aproximaciones consideradas fueron:

1. Que se envíe un paquete y se espere hasta que el receptor envíe una confirmación de que le llegó el paquete, es muy parecido al algoritmo de parada y espera. El problema principal que conlleva este método es que no se alcanza a aprovechar la capacidad de subida del canal.

2. Luego, como una clase de evolución al primer método, que se envían paquetes continuamente hasta que algún buffer intermedio esté "casi lleno", y que cuando esto suceda se envié una señal al emisor que haga que este pare de enviar paquetes hasta que el buffer se vacíe.

3. Finalmente, lo que terminó siendo implementado, es una mejora del segundo, haciendo que cuando un buffer está "casi lleno" se envía una señal para que el lado del emisor disminuya la velocidad transmisión de paquetes (en lugar de pararla completamente).

    Con mas detalle, primero se establece un umbral de alrededor un 70~80% de la capacidad máxima del buffer, si se alcanza el umbral en el receptor, simplemente generará un mensaje de feedback y lo enviará por el canal exclusivo.
Si se alcanza el umbral en la queue intermedia, esta generará y enviará un mensaje de feedback por el canal normal de comunicación, `nodeRx` se enterará que dicho mensaje es para `nodeTx` y lo enviará por el canal exclusivo en lugar de enviarselo a la capa de aplicación.

    Cuando llega el mensaje de feedback a nodeTx, este disminuirá su transmisión de paquetes encolandolos al rededor de un 10% mas tarde de lo que deberían ser encolados, después de T segundos la velocidad de transmisión volverá a lo normal.

    Notese que siguiendo la misma idea, este modelo se puede generalizar con una cantidad arbitraria de queues intermedias.

## Resultados

    Para poder evaluar los resultados del algoritmo, al ser la misma solución tanto para un problema de congestión y un problema de flujo, se modificaron los siguientes valores para que se pudieran observar ambos problemas en una sola simulación:

- La tasa de transferencia desde el emisor a la cola del medio pasa a ser 10 paquetes por segundo.

- La tasa de transferencia de la cola del medio al receptor pasa a ser 8 paquetes por segundo, además el tamaño del buffer pasa de 200 a 100.

- La velocidad de consumo del receptor pasa a ser 5 paquetes por segundo.

    De esta manera se asegura  que en algún momento se va a llenar el buffer de la cola del medio ya que la tasa de transmisión del emisor a la cola del medio es más rápida que la de la cola del medio al receptor y también que habrá un problema de control de flujo ya que el emisor recibe paquetes más rápido de lo que los consume.

    A continuación, el gráfico de paquetes en cada buffer para un intervalo de generación de paquetes de distribución ε(0.1):

![Buffers_sizes_generation=0.1.svg](./Gráficos_parte2/Buffers_sizes_generation=0.1.svg)

    Se puede observar claramente en los buffers del receptor y de la cola intermedia que cada vez que la cola se acerca a su tamaño máximo y se envía el paquete de advertencia al emisor baja considerablemente la tasa de transmisión lo que causa que las colas logren descongestionarse. Además se puede ver que después de cierto tiempo se va llenando la cola nuevamente, esto debido a que después de que pasa cierto tiempo sin recibir un paquete de advertencia el emisor aumenta la tasa de transferencia.

    Viendo el gráfico se puede ver que los casos de congestión son bastante parecidos a los algoritmos de control de congestión que maneja TCP, el motivo por el que se hizo de esta manera y porque baja tanto la tasa de transmisión es para asegurar que se baje lo suficiente como para prevenir la pérdida de paquetes.

    Otra cosa que se puede observar claramente además de la variación de la cantidad de paquetes en cola es la cantidad de paquetes que quedan en la cola del emisor, esto debido a que con un intervalo de generación de 0.1 los paquetes se generan muy rápido comparado a la tasa de transmisión del emisor a la cola del medio y se va a ver reflejado más adelante en los gráficos de carga enviada vs carga recibida.

    A continuación los mismos gráfico de la parte 1 de todos los datos medidos para λ = 0.1, 0.2, 0.3:

| λ = 0.1                                                                                                     | λ = 0.2                                                                                                     | λ = 0.3                                                                                                     |
| ----------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------- |
| ![Buffers_sizes_generation=0.1.svg](./Gráficos_parte2/Buffers_sizes_generation=0.1.svg)                     | ![Buffers_sizes_generation=0.2.svg](./Gráficos_parte2/Buffers_sizes_generation=0.2.svg)                     | ![Buffers_sizes_generation=0.3.svg](./Gráficos_parte2/Buffers_sizes_generation=0.3.svg)                     |
| ![Sent_vs_recived_packets_generation=0.1.svg](./Gráficos_parte2/Sent_vs_recived_packets_generation=0.1.svg) | ![Sent_vs_recived_packets_generation=0.2.svg](./Gráficos_parte2/Sent_vs_recived_packets_generation=0.2.svg) | ![Sent_vs_recived_packets_generation=0.3.svg](./Gráficos_parte2/Sent_vs_recived_packets_generation=0.3.svg) |
| ![Resived_packet_delay_generation=0.1.svg](./Gráficos_parte2/Resived_packet_delay_generation=0.1.svg)       | ![Resived_packet_delay_generation=0.2.svg](./Gráficos_parte2/Resived_packet_delay_generation=0.2.svg)       | ![Resived_packet_delay_generation=0.3.svg](./Gráficos_parte2/Resived_packet_delay_generation=0.3.svg)       |

    Se puede observar que cuando el intervalo es mayor o igual a 0.2 los gráficos son muy parecidos a los vistos en el segmento anterior (sin algoritmo de control de congestión/flujo), esto debido a que para el intervalo de generación de paquetes las tasas de transmisión son lo suficientemente altas como para evitar congestiones o problemas de flujo.

    El caso más interesante es cuando el intervalo es igual a 0.1, anteriormente se vió que en este caso la mayoría de paquetes quedan en la cola del emisor debido a la velocidad de la tasa de transferencia y esto se puede ver en el gráfico de paquetes enviados vs recibidos, ya que parece que sólo se recibe la mitad de los paquetes que se envían, sin embargo esto no se debe a que se pierdan paquetes, ya que en el gráfico se puede ver que no hay paquetes perdidos, si no que se debe a los paquetes que quedan en las colas (la gran mayoría en la cola del emisor). Otra cosa que se puede observar en este caso es como el retardo va aumentando constantemente, esto porque cada vez los paquetes tienen que pasaron por una cola mas larga antes de ser enviados.

    A continuación se pueden observar los gráficos de carga enviada vs carga recibida y de paquetes enviados/recibidos por intervalo:

![Gráfico de carga útil.svg](./Gráficos_parte2/Gráfico%20de%20carga%20útil.svg)

![Gráfico de intervalo de generación vs aprovechamiento.svg](./Gráficos_parte2/Gráfico%20de%20intervalo%20de%20generación%20vs%20aprovechamiento.svg)

    Como se comentó anteriormente, a primera vista se puede ver que son muy parecidos a los gráficos obtenidos en el segmento anterior, donde no hay algoritmo de control de congestión/flujo, sin embargo en el análisis realizado anteriormente para el intervalo de 0.1 se pudo observar que cuando los paquetes se generan muy rápido, debido a la baja velocidad de transmisión la mayoría queda en la cola del emisor que es bastante grande, sin embargo no se pierden paquetes a lo largo de la red.

## Implmentación

    Primero se creó un nuevo tipo de paquete `FeedbackPkt` para la transmisión de mensajes de control al emisor.
Como se puede ver en el esquema en [Métodos](#métodos), se agregó una nueva cola al Network con el fin de permitir una comunicación directa entre `nodeRx` y `nodeTx`. Hablando de `nodeTx` y `nodeRx`, estos fueron cambbiados, agregando nuevos submódulos `TransportTx` y `TransportRx` para que ambos nodos tengan forma de estar conectados por ambas colas.

    En el lado de C++, se crearon nuevas clases para `TransportTx`, `TransportRx` y `FeedbackPkt`, también se modificó el módulo de `Queue` para que también pueda manejar mensajes de control.

### `Queue.cc`

    Se establece una variable de umbral equivalente al 80% de la capacidad máxima de su buffer, si dicho buffer alcanza (o supera) el umbral, creará un mensaje de control para avisar sobre problemas de congestión.

### `TransportRx.cc`

    Su funcionamiento es escencialmente como el de `Queue.cc`, la diferencia es en el manejo de paquetes, inicialmente todos los paquetes se envían a la capa de aplicación, pero si se reciben mensajes de control entonces estos deben haber sido enviados por la cola `queue0` para señalizar posibles problemas de congestión, este paquete entonces será enviado por medio de `queue1` directo a `TransportRx`.

    También se establece una variable umbral que es un 70% de la capacidad máxima de su buffer, la razón por la que se elige un valor menor que la de `queue0` es porque pueden ya haber muchos paquetes en la red, esto dará mas espacio a `TransportRx` para poder manejar dichos paquetes.
Cuando se alcanza su umbral, este creará un mensaje de control y lo enviará por `queue1` avisando que habrán problemas de flujo.

### `TransportTx.cc`

    Tiene una funcionalidad similar a la de `Queue.cc`, así se saltearan los detalles básicos, lo interesante viene en como maneja el control de flujo y congestión.

    Se establecen 2 constantes `CONTROL_TIMEOUT` y `CONTROL_REGAIN_TIME` (con valores de 0.5 y 1.0 segundos en la implementación, respectivamente).
`CONTROL_TIMEOUT` es el un cooldown que ignora mensajes de control, ya que, como los nodos de la red seguro tienen muchos paquetes a ser procesados, una vez se alcanza el umbral, seguirá procesando y enviando mas mensajes de control. `CONTROL_REGAIN_TIME` es otro temporizador cuyo fin es el de "retomar" el tiempo de transmisión de paquetes.

    Pero ¿A qué se refiere con retomar? `TransportTx` guarda un escalar `contScalar` que establece que tan tarde se enviará un paquete, inicialmente su valor es 1.0, que en efecto hará que dicho paquete se encole en el tiempo que tardará en transmitirse, pero cuando hay un mensaje de control el escalar aumentará un 0.1%, es decir, se encolará un 10% mas tarde.
Esto continuará si se reciben mas mensajes de control, pero eventualmente los buffer de las colas tienen oportunidad de liberarse, así que para aprovechar mejor la capacidad de la red luego de un tiempo (definido por `CONTROL_REGAIN_TIME`) se disminuirá el tiempo de encolación un 10% (hasta que este sea el valor "normal" de 1.0, no se deberían encolar paquetes en menos tiempo del que tardan en transmitirse).

## Discusión

    En esta simulación todo está muy simplificado, ya que:

- Se usa una red muy simple, en la que a diferencia de una red real hay muy pocos nodos, no suceden eventos como la caída de un enrutador o servidor y las velocidades de los enlaces y los nodos no cambian dinamicamente.

- Se de dejan fijos muchos parámetros que se podrían modificar, como las velocidades de procesamiento, los tamaños de los buffers y la distribución de la generación de paquetes nuevos (se varia el parámetro λ en la distribución ε(λ), pero no se prueban otras distribuciones como la normal o la uniforme)

- Se prueba un solo algoritmo, cuando en realidad hay muchas otras formas de solucionar le problema de control de flujo y congestión.

    En otros trabajos se podría investigar que pasa al cambiar algunas de esas cosas.

    En particular, para el algoritmo se podría:

- Modificarlo para que reaccione correctamente ante eventos como caída de enrutadores y optimizar mejor el tiempo que pasa antes de que el transmisor aumente/disminuya la transmisión de datos.





    Una limitación que tiene el algoritmo es que debido a que se corrigieron ambos problemas (congestión y flujo) con la misma solución se baja la transmisión del emisor al enrutador, cuando, por ejemplo, en el caso de un problema de flujo basta con disminuir la tasa de transferencia del enrutador al receptor y no hace falta cambiar la tasa del emisor al enrutador, esto se puede ver claramente en los resultados anteriores ya que con un intervalo de generación de 0.1 el delay va aumentando linealmente.


