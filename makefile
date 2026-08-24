processFlow: processFlow.c task.c run.c job.c
	gcc -o processFlow processFlow.c task.c run.c job.c -I.

clean:
	rm -f processFlow