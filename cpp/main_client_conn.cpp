#include "conn/ClientConn.h"

int main(){
    ClientConn client("0.0.0.0",8080);
    client.main();
}