*\
	TinyJAMBU-128: 128-bit key, 96-bit IV (initialization vector)
	Optimized implementation
	The state consists of four 32-bit registers
	state[3] || state[2] || state[1] || state[0]

*/\
	const FrameBitsIV: 0x10
	const FrameBits AD: 0x30
	const FrameBitsPC: 0x50
	const FrameBitsFinalization: 0x70
	const NROUND1: 128*5
	const NROUND2: 128*8

// optimize state update function

state_update(state: u32, key: ) -> (){

}

