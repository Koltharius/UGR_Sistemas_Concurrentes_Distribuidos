JAVASRCS:=$(wildcard *.java)
CLASSES_BASE:=$(shell grep class *.java | cut -d' ' -f2)
CLASSES:=$(addsuffix .class,$(CLASSES_BASE))

all: Barbero Fumadores Productor-Consumidor monitor_classes

Barbero:
	javac -g Barbero.java

Fumadores:
	javac -g Fumadores.java

Productor-Consumidor:
	javac -g Productor-Consumidor.java

monitor_classes: $(wildcard monitor/*.class)
	javac -g monitor/*.java -d .

clean:
	rm -rf *.class monitor/*.class

msg_clases:
	@echo "fuentes java = " $(JAVASRCS)
	@echo "clases = " $(CLASSES_BASE)
