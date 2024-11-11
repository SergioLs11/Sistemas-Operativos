TARGET = proyecto
SRC = proyecto.c

CC = gcc
CFLAGS = -Wall


all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


run_sistema:
	./$(TARGET) sistema -p pipePSC -s pipeSSC -t 10

run_suscriptor:
	./$(TARGET) suscriptor -s pipeSSC

run_publicador:
	./$(TARGET) publicador -p pipePSC -t 5

clean:
	rm -f $(TARGET)
