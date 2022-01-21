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



enum CharacterAction : uint16_t {
	CHARACTER_START_MOVE,
	CHARACTER_STOP_MOVE,
	CHARACTER_INPUT_CHANGED

};