#pragma once

enum CMD {
	Invalid,
	AcceptTCPConnection,
	SelfEnterWorld,
	PlyEnterWorld,
	ServerMessage,
	MovePlayer,
	Ping,
	MessageAll
};