#include "../common/include/protocol.h"

void Status_into_chars(StatusElection status, char* c){
    switch(status) {
        case canceled:
            strcpy(c, "canceled");
        case active:
            strcpy(c, "active");
        case closed:
            strcpy(c, "closed");
    }
}

StatusElection Status_from_chars(char* status){
    if(strcmp("active",status) == 0){
        return active;
    }
    else if(strcmp("closed",status) == 0){
        return closed;
    }
    else if(strcmp("canceled",status) == 0){
        return canceled;
    }
}