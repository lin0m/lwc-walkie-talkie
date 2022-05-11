{\rtf1\ansi\ansicpg1252\cocoartf2638
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 /*\
	TinyJAMBU-128: 128-bit key, 96-bit IV (initialization vector)\
	Optimized implementation\
	The state consists of four 32-bit registers\
	state[3] || state[2] || state[1] || state[0]\
\
*/\
	const FrameBitsIV: 0x10\
	const FrameBits AD: 0x30\
	const FrameBitsPC: 0x50\
	const FrameBitsFinalization: 0x70\
	const NROUND1: 128*5\
	const NROUND2: 128*8\
\
// optimize state update function\
\
state_update(state: u32, key: ) -> ()\{\
\
\}}

//hello world!!
