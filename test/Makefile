byteCodePath := ./../byteCode/
builtInPath := ./../built_in/
parserPath := ./../parser/
utilsPath := ./../utils/
vmPath := ./../vm/
compilerPath := ./../compiler/
ownFile := ./

outPutName := test

all: ownFileObjectFile utilsObjectFile parserObjectFile byteCodeObjectFile compilerObjectFile
	g++ -o $(outPutName) ./*.o -g
	$(MAKE) clean

ownFileObjectFile:
	g++ -c $(ownFile)*.cpp -g -std=c++20

utilsObjectFile:
	$(MAKE) -C $(utilsPath) objectFile
	mv $(utilsPath)*.o ./

compilerObjectFile:
	$(MAKE) -C $(compilerPath) objectFile
	mv $(compilerPath)*.o ./

parserObjectFile:
	$(MAKE) -C $(parserPath) objectFile
	mv $(parserPath)*.o ./

byteCodeObjectFile:
	$(MAKE) -C $(byteCodePath) objectFile
	mv $(byteCodePath)*.o ./

builtInObjectFile:
	$(MAKE) -C $(builtInPath) objectFile
	mv $(builtInPath)*.o ./

vmObjectFile:
	$(MAKE) -C $(vmPath) objectFile
	mv $(vmPath)*.o ./

clean:
	rm -f *.o
