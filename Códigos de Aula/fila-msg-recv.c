// Arquivo mq-recv.c: recebe mensagens de uma fila de mensagens POSIX.
// Em Linux, compile usando: gcc -o fila-msg-recv fila-msg-recv.c -lrt
#include<stdio.h>
#include<stdlib.h>
#include<mqueue.h>
#include<sys/stat.h>

#define QUEUE "/my_queue"

int main(void) {
   mqd_t queue; // descritor da fila de mensagens
   struct mq_attr attr; // atributos da fila de mensagens
   int msg; // as mensagens são números inteiros

   // abre ou cria a fila com permissoes 0666
   if((queue = mq_open (QUEUE, O_RDWR|O_CREAT, 0666, &attr)) < 0){
      perror ("mq_open");
      exit (1);
   }

   // le o atributos default da fila de mensagens criada 
   if (mq_getattr(queue, &attr) == -1) {
      perror("mq_getattr");
      exit (1);
   }
   printf("Maximum # of messages on queue:   %ld\n", attr.mq_maxmsg);
   printf("Maximum message size:             %ld\n", attr.mq_msgsize);

   // recebe cada mensagem e imprime seu conteudo
   for (;;) {
      //if ((mq_receive (queue, (void*) &msg, sizeof(msg), 0)) < 0)
      if ((mq_receive (queue, (void*) &msg, attr.mq_msgsize, 0)) < 0)
         perror("mq_receive:") ;
      else
         printf ("Received msg value %d\n", msg);
   }
   return 0;
}
