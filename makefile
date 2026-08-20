processFlow: processFlow.c
	gcc -o processFlow processFlow.c

clean:
	rm -f processFlow