processFlow: processFlow.c task.c 
	gcc -o processFlow processFlow.c task.c -I.

clean:
	rm -f processFlow