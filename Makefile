## Instrucciones para deployar:
## Escribir make deploy en la consola.
## Cuando pida contraseña de root, proporcionarla.
## Cuando pida user de github, proporcionarlo.

## Cada vez que se haga "make deploy" el make automáticamente
## bajará lo que no esté bajado y compilará lo no compilado.
## Si se desea rehacer el proceso, escribir "sudo make clean", lo que reiniciará todo.

deploy: dependencias tp

dependencias: so-commons-library

so-commons-library:
	$(call mostrarTitulo,$@)
	git clone https://github.com/sisoputnfrba/so-commons-library ../so-commons-library
	cd ../so-commons-library; sudo make install

tp: dataNode fileSystem master worker yama

dataNode:
	$(call mostrarTitulo,$@)
	cd dataNode/Debug; make all

fileSystem:
	$(call mostrarTitulo,$@)
	cd fileSystem/Debug; make all

master:
	$(call mostrarTitulo,$@)
	cd master/Debug; make all

worker:
	$(call mostrarTitulo,$@)
	cd worker/Debug; make all

yama:
	$(call mostrarTitulo,$@)
	cd yama/Debug; make all

clean:
	$(call mostrarTitulo,$@)
	rm -rf ../so-commons-library

define mostrarTitulo
	@echo
	@echo "########### $(1) ###########################################"
endef