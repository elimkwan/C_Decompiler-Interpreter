all: asmdump.exe

asmdump.exe: asmdumpmain.c
	gcc -std=c99 -Wall asmdumpmain.c ConvertOpcodeHexfile.c decompiler.c -o asmdump.exe

test: asmdump.exe
	python tests/runtests-4.0.py ./asmdump.exe

clean:
	rm asmdump.exe

zip: clean
    cd .. && python -c 'import shutil; shutil.make_archive(“../asmdump-4.0", "zip", “..”, base_dir=“./asmdump-4.0”);’