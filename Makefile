.SILENT: clean

all: src/flex.l src/bison.y src/arvore.c src/arvore.h
	@mkdir -p bin/jason
	@cd build/ ; cmake . ; make -s
	@cd ..

clean:
	rm -rf bin/*
	rm -rf out/*
	mkdir tmp
	mv build/CMakeLists.txt tmp
	rm -rf build/*
	mv tmp/CMakeLists.txt build
	rm -rf tmp
