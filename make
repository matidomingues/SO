gcc -o bin/client client/client.c includes/transporter.c includes/conn/pipe.c -g
gcc -o bin/server server/server.c includes/linkedlist.c includes/csv.c includes/transporter.c includes/conn/pipe.c -g