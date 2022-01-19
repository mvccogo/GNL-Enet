#include "GDNative.h"
#include "Client.h"


const uint16_t MAX_LEN = 1024 * 5;
using namespace godot;

void NetNode::_register_methods() {
	register_method("_process", &NetNode::_process);
	
	register_signal<NetNode>((char*)"packet_received", "packet_cmd_id", GODOT_VARIANT_TYPE_INT);
}


NetNode::NetNode() {
	// Initialize your NativeScript classes in their `_init()` method, not their constructor.The constructor can't access the base class's methods.
}

NetNode::~NetNode() {

}

void NetNode::_init() {


}

void NetNode::_process(float delta) {

}
