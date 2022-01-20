#pragma once

enum CMD : uint16_t {
	Invalid,
	AcceptConnection,
	SelfEnterWorld,
	PlyEnterWorld,
	ServerMessage,
	MovePlayer,
	Ping,
	MessageAll
};