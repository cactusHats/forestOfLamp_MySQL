#pragma once

enum class Color {
	Red,
	Green,
	Blue,
	NUM_COLOR
};

//OSC�ʐM
#define IP_HOST "localhost"
#define PORT_TO_LCON 9000 //ledControl�ւ̃|�[�g

//DB����f�[�^��ǂݎ��Ԋu[sec]
#define READ_INTERVAL 1