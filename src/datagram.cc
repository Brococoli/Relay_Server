#include "../include/datagram.h"

/* inline */
int Datagram::datagram_type() const {
    return datagram_type_;
}

/* inline */
void Datagram::set_datagram_type(int datagram_type){
    datagram_type_ = datagram_type;
}
