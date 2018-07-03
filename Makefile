libThrow.a: Throw.h Throw.cxx
	g++ -c Throw.cxx -g -pthread -std=c++1z -m64 -I/usr/include/root \
		-L/usr/lib/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad \
		-lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore \
		-lThread -lMultiProc -pthread -lm -ldl -rdynamic -I. -o Throw.o
	ar rcs libThrow.a Throw.o
	rm Throw.o

Test: Test.cxx libThrow.a
	g++ Test.cxx -g -pthread -std=c++1z -m64 -I/usr/include/root -L/usr/lib/root \
		-lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
		-lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc \
		-pthread -lm -ldl -rdynamic -I. -L. -lThrow -o Test

test: Test
	./Test

clean:
	rm Test
	rm *.pdf
	rm libThrow.a
