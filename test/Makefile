byteCodePath := ./../byte_code/
compilerPath := ./../compiler/
parserPath := ./../parser/
utilsPath := ./../utils/
ownFile := ./

outPutName := test

all: ownFileObjectFile compilerObjectFile utilsObjectFile parserObjectFile byteCodeObjectFile
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

byteCodeObjectFile:
	$(MAKE) -C $(byteCodePath) objectFile
	mv $(byteCodePath)*.o ./

clean:
	rm -f *.o