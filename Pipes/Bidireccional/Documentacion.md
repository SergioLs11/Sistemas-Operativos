

# Comunicación bidireccional mediantes PIPES
### PIPES
Los pipes (o tuberías) en sistemas operativos hace referencia a un mecanismo de comunicación de muchos procesos al tiempo, estos funcionan como canales unidireccionales o bidireccionales, la diferencia entre ellos se da principalmente en si el sentido de la comunicación es unicamente de un cliente al servidor(o viseversa en un esquema cliente servidor) o si este se da de ambos lados, dando a entender que tanto el cliente como el servidor tienen la capacidad de enviar y recibir información.

En este ejemplo se va a realizar una comunicación bidireccional, el cliente le envía mensajes al servidor, y el servidor va a responder con este mismo mensaje escrito al revés. Se recomienda primero ver el ejemplo sobre pipies unidireccionales que se encuentra en este mismo repositorio.

### Servidor
En este caso, el servidor va a ser el encaragado de recibir los mensajes enviados por el cliente, para esto se debe crear un archivo tipo *FIFO_FILE* para que se puedan intercambiar los mensajes, así mismo se va a inicializar un archivo de nodo, conteniendo los siguientes parametros:

1. FIFO_FILE: Buscará y usará el archivo de este tipo creado anteriormente con el nombre "/tmp/fifo_twoway".
2. S_IFIFO: Es un indicador que el archivo efectivamente es tipo FIFO, permitiendo la comunicación entre procesos.
3. 0660: Representa los permisos que van a tener por parte del propietario y el grupo que haga parte de este:
   1. El propietarios tiene permisos de lectura y escritura (6).
   2. El grupo al que pertenece el archivo tiene permisos de lectura y escritura (6).
   3. Otros usuarios no van a tener acceso (0).
4. El tercer parametro (0) se usa cuando se crean dispositivos, como estamos usando PIPES esto se ignora.

 Al ya tener creada este nodo se necesita abrir el archivo tipo FIFO, este se abre modo lectura y escritura con la bandera *O_RDWR* y marcando el final de la comunicación con el código *strcpy(final, "fin")*, en el cual cuando se recibe el texto "fin" se cerrará el archivo.

 Hasta el momento se ha realizado casi la misma configuración o codificación de una tubería unidireccional, para que se haga efectiva la comunicación bidireccional se necesita que el servidor responda algo al mensaje del cliente, de esta manera se usará la función *de_cabeza*, la cual va a ser la encargada de invertir el orden del mensaje recibido por el servidor, seguido de escribir en el pipe usado este mensaje para así su transmisión.

### Cliente
En cuanto al lado del cliente, se configurarán algunos parametros similares, como lo pueden ser el cierre del archivo con la pablabra clave "fin" o la creación del archivo tipo FIFO. Pasando a la apertura del archivo, cambia la segunda bandera, ya que esta se declara como *O_CREAT|O_RDWR* especificando que si el archivo no existe debe ser creado, y además se abrirá en modo lectura y escritura para enviar los mensajes al servidor y recibir los que el servidor responda. Por último escribirá el mensaje a enviar en el archivo con la función *write*, en la cual se especificará el PIPE por el cual se va a enviar y el mensaje a enviar ya dentro del contenedor o buffer, finalizando imprimiendo en la pantalla el mensaje que se envió y su tamaño. 
 
