#include<stdio.h>
#include<string.h>    // for strlen
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    //used variables
    
    //I got help from https://www.geeksforgeeks.org/strftime-function-in-c/ for strftime function 
    time_t t ;
    struct tm *tmp ;
    char program_answer[3000];
    char get_date[] = "GET_DATE";
    char answer[] = "ANSWER: ";
    char ir [] = "INCORRECT REQUEST\n";
    
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char *message,user_msg[3000];
    
     
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);
     
    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");
     
    // Listen
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
    c = sizeof(struct sockaddr_in);
    new_socket= accept(socket_desc,(struct sockaddr *)&client,(socklen_t*)&c);
    if (new_socket<0)
    {
        puts("Accept failed");
        return 1;
    }
    
    
    while(1)
    {
        puts("Connection accepted");
        message = "\nUse CONTROL+C from the terminal where the c file is opened for quit.\nIf you want to continue, enter the message:\n";
        write(new_socket, message, strlen(message));
        recv(new_socket, user_msg , 2000 , 0);
        
        //Checking the content of GET_DATE in user_msg.
        int flag = 0;
        int x = 0;
        while(x!=8){
            if (user_msg[x] != get_date[x]){
                flag = 1;
                break;
            }
            x++;
        }

        //INCORRECT REQUEST message !
        if(flag == 1){
            write(new_socket, ir, strlen(ir));
        }
        else{
            //Checking the content of % in user_msg.
            flag =1;
            for(int i = 0;i<strlen(user_msg);i++){
                if(user_msg[i] == '%'){
                    flag = 0;
                    break;
                }
            }
            time( &t );
            tmp = localtime( &t );
            strftime(program_answer, sizeof(program_answer), user_msg, tmp);
            //incorect request control
            for(int i = 0;i<strlen(program_answer);i++){
                if(program_answer[i] == '%'){
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                //Print Correct request on screen
                for(int i = 0;i<8;i++){
                    program_answer[i] = answer[i];
                }
                write(new_socket, program_answer, strlen(program_answer));
            }
            //INCORRECT REQUEST message !
            else {
                write(new_socket, ir, strlen(ir));
            }
        }
        
    }    

    return 0;
}

