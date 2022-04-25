#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3490

#define MAXDATASIZE 100

//rellenas toda la info necesaria para mandar
//mando la info al socket
//empiezo a hacer el receive

int main(int argc, char *argv[])  //argc -> longitud  [./client, 127.0.01)] -> argc=2  argv -> arreglo con los parametros que veo desde la consola (puedo mandar desde la consola informacion en ese parametro)
{
 int socket_file_descriptor, bytes_received;   //descriptor del socket (un entero) - numero de bytes 
 char message[MAXDATASIZE];   //buffer (mensaje)
 struct hostent *he;      //contiene campos con información del host
 struct sockaddr_in their_addr;  //guarda info sobre quien se va a conectar


 
 char second_message[MAXDATASIZE] = "RECIBIDO!";
 int quantityOfBytes = sizeof(second_message);

 char bytesEnviados[MAXDATASIZE] = ("Bytes Enviados: %d\n",quantityOfBytes);

 if (argc != 2 )   
 {
  fprintf(stderr, "Por favor ingrese el hostname del cliente\n");   //envia un mensaje de error 
  exit(1);      //finaliza la ejecucion
 }

 if ((he=gethostbyname(argv[1])) == NULL)   //gethostbyname retorna puntero a estructura hostent - toma la estructura vacia "he" y reviso si está null 
 {
  perror("gesthostbyname");
  exit(1);
 }

//socket(AF_INET, SOCK_STREAM, 0) -> AF-INET es la familia de direcciones - SOCK_STREAM creo un socket de flujo (uso read y write) TCP/IP - 0 indica que no se va a especificar el protocolo
 if ((socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)  //se crea el socket (función que permite la conectividad ida y vuelta entre varias entidades)
 {
  perror("socket");
  exit(1);
 }

 their_addr.sin_family = AF_INET;
 their_addr.sin_port   = htons(PORT);   //converts a u_short from host to TCP/IP network byte order
 their_addr.sin_addr   = *((struct in_addr *)he->h_addr);  //traspaso la informacion de quien se va a conectar al socket
 bzero(&(their_addr.sin_zero), 8);    //rellena con ceros la parte sin_zero

 if (connect(socket_file_descriptor, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)   //conecta el socket servidor al cliente y viceversa
 {
  perror("connect");
  exit(1);
 }

while(1)
{
 if ((bytes_received=recv(socket_file_descriptor, message, MAXDATASIZE, 0)) == -1)  //recibe del servidor un mensaje, lo guarda en el buffer y retorna la cantidad de bytes que se recibieron
 {
  perror("recv");
  exit(1);
 }

 message[bytes_received] = '\0';   //a la ultima posicion del mensaje le pongo un /0 porque así terminan los strings

 printf("Received: %s\n", message);
 printf("Bytes Recidos: %d",strlen(message));

 if (send(socket_file_descriptor, second_message, quantityOfBytes, 0) == -1) perror("send");  //12 son la cantidad de bytes a enviar - 0 es una bandera
 else printf("Enviado\n");


}
 close(socket_file_descriptor);

 return 0;
}
