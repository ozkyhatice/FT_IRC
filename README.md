# IRC  - Server Setup and Connection

This project develops an IRC application using C++. The following sections provide detailed information on setting up the server, making connections, and using sockets.

## Server Address Configuration

To run the server, the necessary configuration is made using the `sockaddr_in` structure, which represents IPv4 addresses and contains the address information required for creating a socket or listening socket.

### sockaddr_in Structure

```cpp
struct sockaddr_in {
    short            sin_family;   // Address family (AF_INET/AF_INET6)
    unsigned short   sin_port;     // Port number (converted using htons())
    struct in_addr   sin_addr;     // IP address (e.g., INADDR_ANY)
    char             sin_zero[8];  // Padding for alignment
};
```

### Structure Members:

- **sin_family**: Specifies the address family (use `AF_INET` for IPv4).
- **sin_port**: Sets the port number to listen on. The port number is converted to network byte order using `htons()`.
- **sin_addr**: Represents the IP address. If set to `INADDR_ANY`, it listens on all available IP addresses.
- **sin_zero**: Padding added to align the structure to the appropriate size.

### Creating a Socket

The `socket()` function is used to create a socket in your server application:

```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0) {
    std::cerr << "Socket creation failed" << std::endl;
    return -1;
}
```

#### `socket()` Function:

- **AF_INET**: Specifies the IPv4 address family.
- **SOCK_STREAM**: Indicates the TCP protocol, providing reliable, connection-oriented data transmission.
- **0**: Protocol number. When set to `0`, the default protocol (TCP for `SOCK_STREAM`) is automatically chosen.

If the socket creation fails, the `socket()` function will return `-1`.

### Configuring the Server Address

To configure the server's IP address and port, use the following code:

```cpp
struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;                          // IPv4 protocol
serv_addr.sin_port = htons(8080);                         // Convert the port number to network byte order
serv_addr.sin_addr.s_addr = inet_addr("192.168.1.1");     // Specified IP address
```

### Why Use `htons()`?

The `htons()` function is used to convert the port number from the host byte order (little-endian) to the network byte order (big-endian). This conversion is necessary because networks use big-endian format for data transmission.

### Listening on All IP Addresses

If you want the server to accept connections from any available IP address, you can set `sin_addr.s_addr` to `INADDR_ANY`:

```cpp
serv_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all connected IP addresses
```

This allows the server to accept connections from any IP address.

---
