#include "conn/AgentConn.h"

int main(){
    AgentConn agent("0.0.0.0", 6969);
    agent.main();
}