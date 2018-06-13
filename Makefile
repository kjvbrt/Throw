Test: Test.cxx Throw.h
	g++ -g -pthread -std=c++1z -m64 -I/usr/include/root -L/usr/lib/root -lCore \
		-lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer \
		-lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc \
		-pthread -lm -ldl -rdynamic -I. -o Test Test.cxx

test: Test
	./Test
