#include "conn/AgentConn.h"

int main(){
    AgentConn agent("0.0.0.0", 8080);
    agent.main();
}