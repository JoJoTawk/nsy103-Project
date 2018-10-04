TCP:

    server :
        - build and run by passing "port" as parameter (2000 as exemple).
        - After clients connect you can use _kill nameofclient to disconnect a client, _shdn 
            shutdown the server.
    client :
        - build and run by passing "name" as parameter (unknown as exemple). then write " _connect name servername(127.0.0.1) port " to connect to the server 
         - you can use _who to see clients in the chatroom, _quit to exist the chatroom. 

UDP:

    server:
        - build and run by passing "port" as parameter (2000 as exemple).
        - After clients connect you can use _kill nameofclient to disconnect a client, _shdn 
            shutdown the server.

    client  :
        - build and run by passing "name" as parameter (unknown as exemple).
        - you can use _who to see clients in the chatroom, _quit to exist the chatroom. 
