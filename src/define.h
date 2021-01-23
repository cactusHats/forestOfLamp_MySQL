#pragma once

enum class Color {
	Red,
	Green,
	Blue,
	NUM_COLOR
};

//OSC通信
#define IP_HOST "localhost"
#define PORT_TO_LCON 9000 //ledControlへのポート

//DBからデータを読み取る間隔[sec]
#define READ_INTERVAL 1