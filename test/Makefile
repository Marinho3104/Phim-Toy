byteCodePath := ./../byteCode/
objectsPath := ./../objects/
parserPath := ./../parser/
utilsPath := ./../utils/
vmPath := ./../vm/
ownFile := ./

outPutName := test

all: ownFileObjectFile utilsObjectFile parserObjectFile byteCodeObjectFile objectsObjectFile vmObjectFile
	g++ -o $(outPutName) ./*.o -g
	$(MAKE) clean

ownFileObjectFile:
	g++ -c $(ownFile)*.cpp -g -std=c++20

utilsObjectFile:
	$(MAKE) -C $(utilsPath) objectFile
	mv $(utilsPath)*.o ./

parserObjectFile:
	$(MAKE) -C $(parserPath) objectFile
	mv $(parserPath)*.o ./

byteCodeObjectFile:
	$(MAKE) -C $(byteCodePath) objectFile
	mv $(byteCodePath)*.o ./

objectsObjectFile:
	$(MAKE) -C $(objectsPath) objectFile
	mv $(objectsPath)*.o ./

vmObjectFile:
	$(MAKE) -C $(vmPath) objectFile
	mv $(vmPath)*.o ./

clean:
	rm -f *.o
