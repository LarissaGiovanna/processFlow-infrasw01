processFlow: processFlow.c task.c run.c
	gcc -o processFlow processFlow.c task.c run.c -I.

clean:
	rm -f processFlow