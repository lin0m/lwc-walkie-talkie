/*
	TinyJAMBU-128: 128-bit key, 96-bit IV (initialization vector)
	Optimized implementation
	The state consists of four 32-bit registers
	state[3] || state[2] || state[1] || state[0]

*/
	const FrameBitsIV: 0x10;
	const FrameBits AD: 0x30;
	const FrameBitsPC: 0x50;
	const FrameBitsFinalization: 0x70;
	const NROUND1: 128*5;
	const NROUND2: 128*8;

// optimize state update function

state_update(state: u32, key: /*const unsigned char*/,number_of_steps: u32) -> (){	// I'm not sure how to convert const unsigned char to rust equivalent 
	let i: u32;
	let (t1, t2, t3, t4):(u32, u32, u32, u32);
	
	// in each iteration we compute 128 rounds of the state update function
	for i in 0.128.number_of_steps{
		t1 = (state[1] >> 15) | (state[2] << 17)
		t2 = (state[1] >> 6) | (state[2] << 26)
		t3 = (state[1] >> 21) | (state[2] << 11)
		t4 = (state[1] >> 27) | (state[2] << 5)

	}
}
