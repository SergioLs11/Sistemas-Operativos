# Comunicación unidireccional mediantes PIPES
### PIPES
Los pipes (o tuberías) en sistemas operativos hace referencia a uyn mecanismo de comunicación de muchos procesos al tiempo, estos funcionan como canales unidireccionales o bidireccionales, la diferencia entre ellos se da principalmente en si el sentido de la comunicación es unicamente de un cliente al servidor(o viseversa en un esquema cliente servidor) o si este se da de ambos lados, dando a entender que tanto el cliente como el servidor tienen la capacidad de enviar y recibir información.

Como se mencionó anteriormente, se estará usando un esquema cliente servidor de manera unidireccional, en la cual, el cliente va a enviar flujos de información en forma de texto, y en el servidor se van a ver reflejados estos mensajes hasta que se envíe el mensaje "fin", en este caso se terminará la comunicación de manera simultanea.

### Servidor
En este caso, el servidor va a ser el encaragado de recibir los mensajes enviados por el cliente, para esto se debe crear un archivo tipo *FIFO_FILE* para que se puedan intercambiar los mensajes, así mismo se va a inicializar un archivo de nodo, conteniendo los siguientes parametros:

1. FIFO_FILE: Buscará y usará el archivo de este tipo creado anteriormente con el nombre "MIFIFO".
2. S_IFIFO: Es un indicador que el archivo efectivamente es tipo FIFO, permitiendo la comunicación entre procesos.
3. 0640: Representa los permisos que van a tener por parte del propietario y el grupo que haga parte de este:
   1. El propietarios tiene permisos de lectura y escritura (6).
   2. El grupo al que pertenece el archivo tiene permisos de lectura (4).
   3. Otros usuarios no van a tener acceso (0).
4. El tercer parametro (0) se usa cuando se crean dispositivos, como estamos usando PIPES esto se ignora.

 Al ya tener creada este nodo se necesita abrir el archivo tipo FIFO, este se abre modo lectura con la bandera *O_RDONLY* y marcando el final de la comunicación con el código *strcpy(final, "fin")*, en el cual cuando se recibe el texto "fin" se cerrará el archivo.

### Cliente
En cuanto al lado del cliente, se configurarán algunos parametros similares, como lo pueden ser el cierre del archivo con la pablabra clave "fin" o la creación del archivo tipo FIFO. Pasando a la apertura del archivo, cambia la segunda bandera, ya que esta se declara como *O_CREAT|O_WRONLY* especificando que si el archivo no existe debe ser creado, y además se abrirá en modo escritura para enviar los mensajes al servidor. Por último escribirá el mensaje a enviar en el archivo con la función *write* e imprimiendo en la pantalla el mensaje que se envió y su tamaño. 
 
