./bin/entropyof:
	mkdir -p ./bin
	g++ ./src/entropyof.c -o ./bin/entropyof

install:
	cp ./bin/entropyof /usr/local/bin

clean:
	rm ./bin/entropyof
