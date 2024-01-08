//This code is written by Souvik Ghosh and Debmitra Chatterjee
//Server Program in C
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#define maxSize 100
/*predefined structure
struct sockaddr_un{
       //AF_UNIX
       sa_family_t sun_family;
       //Pathname
       char sun_path[108];
    };
*/

//swap function for sorting
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// Function to perform Selection Sort
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
    // One by one move boundary of unsorted subarray
    for (i = 0; i < n - 1; i++) {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        // Swap the found minimum element
        // with the first element
        swap(&arr[min_idx], &arr[i]);
    }
}

int main(){
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_un server_address;
    struct sockaddr_un client_address;
    
    //The file should be deleted by the caller when it is no longer needed. AF_UNIX sockets can be deleted with unlink().
    unlink("server_socket");
    
    //SOCK_STREAM/SOCK_DGRAM, AF_UNIX(Unix file system )/AF_INET(Internet domain )
    server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    
    //This socket family is used to communicate between processes on the same machine.
    server_address.sun_family = AF_UNIX;
    
    //The file name referred to in addr.sun_path is created as a socket in the system file
    strcpy(server_address.sun_path, "server_socket");
    server_len = sizeof(server_address);
    
    // bind a name to a socket
    // Server ask the OS to enter information in the socket
    // RETURN VALUE- On success, zero is returned.  On error, -1 is returned
    bind(server_sockfd,(struct sockaddr *)&server_address, server_len);
    
    //listen the requests comming from the client
    // 5,backlog - the no of connections allowed in the queue. It can never be zero
    listen(server_sockfd, 5);
    
    while(1){
        int n;
        printf("server waiting.....\n");
        client_len = sizeof(client_address);
        
        //connection gets established here
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,&client_len);
        //read
        read(client_sockfd,&n,sizeof(int));
        int receivedMessage[n];
        read(client_sockfd,&receivedMessage,n*sizeof(int));
        printf("numbers received: ");
        
        for(int i=0;i<n;i++){
            printf("%d ",receivedMessage[i]);
        }
        printf("\n");
        
        //perform operation
        selectionSort(receivedMessage,n);
        printf("numbers sent: ");
        
        for(int i=0;i<n;i++){
            printf("%d ",receivedMessage[i]);
        }
        printf("\n");
        
        //write
        write(client_sockfd,&receivedMessage,n*sizeof(int));
        
        // close the socket connection
        close(client_sockfd);
    }
    return 0;
}