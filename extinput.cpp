#include "extinput.h"

#define PORT "/dev/ttyUSB0"
using namespace LibSerial ;
void gete(int* can){
std::vector<char> a ;
SerialStream my_serial_stream(PORT, SerialStreamBuf::BAUD_19200,SerialStreamBuf::CHAR_SIZE_8,SerialStreamBuf::PARITY_NONE,1,SerialStreamBuf::FLOW_CONTROL_NONE);
//SerialStream my_serial_stream;
my_serial_stream.SetBaudRate( SerialStreamBuf::BAUD_19200);
my_serial_stream.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
my_serial_stream.SetParity(SerialStreamBuf::PARITY_NONE);
my_serial_stream.SetNumOfStopBits(1);
my_serial_stream.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);
my_serial_stream.Open(PORT,std::ios_base::in);
while(1){

if(my_serial_stream.rdbuf()->in_avail()==0)
{
std::string g;
for(int i = 0; i < a.size();i++){

g.push_back(a[i]);
}

if(a.size()){
*can = atoi(g.c_str());
}
a.clear();
usleep(100);
}else{
char c = my_serial_stream.rdbuf()->sbumpc();

a.push_back(c);


//if(int(a)<0.0){*can += 2;}
//if(int(a)==0){*can -= 1;}

}
}}

Extinput::Extinput()
{
 // m_pt = new std::thread(gete,&m_val);
//ctor
}
