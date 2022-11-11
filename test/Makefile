
byteCodePath := ./../byteCode/
parserPath := ./../parser/
utilsPath := ./../utils/
ownFile := ./

outPutName := test

all: ownFileObjectFile utilsObjectFile parserObjectFile byteCodeObjectFile
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

clean:
	rm -f *.o
