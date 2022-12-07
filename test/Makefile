
compilerPath := ./../compiler/
parserPath := ./../parser/
utilsPath := ./../utils/
ownFile := ./

outPutName := test

all: ownFileObjectFile compilerObjectFile utilsObjectFile parserObjectFile
	g++ -o $(outPutName) ./*.o -g
	$(MAKE) clean

compilerObjectFile:
	$(MAKE) -C $(compilerPath) objectFile
	mv $(compilerPath)*.o ./

ownFileObjectFile:
	g++ -c $(ownFile)*.cpp -g

utilsObjectFile:
	$(MAKE) -C $(utilsPath) objectFile
	mv $(utilsPath)*.o ./

parserObjectFile:
	$(MAKE) -C $(parserPath) objectFile
	mv $(parserPath)*.o ./

clean:
	rm -f *.o