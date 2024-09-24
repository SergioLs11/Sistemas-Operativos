# Comunicación unidireccional mediantes PIPES
## PIPES
Los pipes (o tuberías) en sistemas operativos hace referencia a uyn mecanismo de comunicación de muchos procesos al tiempo, estos funcionan como canales unidireccionales o bidireccionales, la diferencia entre ellos se da principalmente en si el sentido de la comunicación es unicamente de un cliente al servidor(o viseversa en un esquema cliente servidor) o si este se da de ambos lados, dando a entender que tanto el cliente como el servidor tienen la capacidad de enviar y recibir información.

Como se mencionó anteriormente, se estará usando un esquema cliente servidor de manera unidireccional, en la cual, el cliente va a enviar flujos de información en forma de texto, y en el servidor se van a ver reflejados estos mensajes hasta que se envíe el mensaje "fin", en este caso se terminará la comunicación de manera simultanea.
