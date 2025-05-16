#include "lwip/tcp.h"
#include "lwip/netif.h"
#include <stdio.h>

// Callback for received data
static err_t tcp_echo_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
  if (p != NULL) {
    // Echo the data back to the client
    tcp_write(tpcb, p->payload, p->len, TCP_WRITE_FLAG_COPY);
    pbuf_free(p); // Free the received buffer
  }
  return ERR_OK;
}

// Callback for new connections
static err_t tcp_echo_accept(void *arg, struct tcp_pcb *newpcb, err_t err) {
  tcp_recv(newpcb, tcp_echo_recv); // Register receive callback
  printf("Client connected!\r\n");
  return ERR_OK;
}

// Initialize the TCP echo server
void tcp_echoserver_init(void) {
  struct tcp_pcb *pcb = tcp_new(); // Create a new TCP control block
  if (pcb != NULL) {
    tcp_bind(pcb, IP_ADDR_ANY, 7); // Bind to port 7 (echo protocol)
    tcp_listen(pcb);               // Start listening
    tcp_accept(pcb, tcp_echo_accept); // Register accept callback
    printf("TCP Echo Server started on port 7!\r\n");
  }
}
