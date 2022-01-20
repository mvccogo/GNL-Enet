#pragma once

enum CMD : uint16_t {
	Invalid,
	AcceptTCPConnection,
	SelfEnterWorld,
	PlyEnterWorld,
	ServerMessage,
	MovePlayer,
	Ping,
	MessageAll
};