#pragma once
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <Windows.h>
#include <vector>
#include "math.h"

using namespace std;

#define X_WIDTH 8
#define Y_WIDTH 8
#define max_depth 3
#define max_depth_ab 5
#define win 999
#define lose -999

#define near_the_base 100
#define capture_coefficient 30

#define SIMPLE_RULE
//#define EXTRA_RULE

//use the dumb heuristic
//#define DUM_HN

//use the smart heuristic
#define SMART_HN



class chess_state
{
public:
	enum location_type: int
	{
		OWN = 0,
		OPPONENT = 1,
		SPACE = 2,
	};

	enum action_set: int
	{
		FORWARD = 0,
		LEFT_FORWARD = 1,
		RIGHT_FORWARD = 2,
		NO_MOVE = 3,
	};

	struct location
	{
		int x; // vertical axis of one specific location
		int y; // horizontal axis
		action_set current_action;
		double evaluation;
	};

	// the current state of chess game
	// a 2D array represent the type of each space
	// row 1-2 is initial location of opponent and row 6-7 is initial location of own
	// initial map:
	// x =  0,1,2,3,4,5,6,7
	//		1 1 1 1 1 1 1 1, y = 0
	//		1 1 1 1 1 1 1 1, y = 1
	//		2 2 2 2 2 2 2 2, y = 2
	//		2 2 2 2 2 2 2 2, y = 3
	//		2 2 2 2 2 2 2 2, y = 4
	//		2 2 2 2 2 2 2 2, y = 5
	//		0 0 0 0 0 0 0 0, y = 6
	//		0 0 0 0 0 0 0 0, y = 7
	int current_state[X_WIDTH][Y_WIDTH];

	// if player is own, num = 0; if player is opponent, num = 1;
	int current_player;

	int game_over_flag;

	// construction, initilize the location of own and opponent
	chess_state(int first_player)
	{
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if ((i == 0) || (i == 1))
				{
					current_state[i][j] = OPPONENT;
				}
				else if ((i == X_WIDTH - 2) || (i == X_WIDTH - 1))
				{
					current_state[i][j] = OWN;
				}
				else
				{
					current_state[i][j] = SPACE;
				}
			}
		}

		if (first_player == 0)
		{
			current_player = 0;
		}
		else
		{
			current_player = 1;
		}
		game_over_flag = 0;
	}
	
	// input the location including x and y axis and the needed action that location needs to take
	void take_action(location input_location);
	
	// defensive heuristic: 2 * remaining_pieces
	double defensive_heuristic(chess_state input_state);

	double smart_defensive_heuristic(chess_state input_state);

	// offensive_heuristic: 2 * (30 - remaining_pieces)
	double offensive_heuristic(chess_state input_state);

	double smart_offensive_heuristic(chess_state input_state);

	// condition that game is over
	// a. a piece arrive the end 
	// b. all opponents' pieces are captured
#ifdef SIMPLE_RULE
	bool game_over(chess_state input_state);
#endif

	// condition of extra rules:
	// a. 3 workes arrive the end
	// b. one side has less than 3 workers
#ifdef EXTRA_RULE
	bool game_over(chess_state input_state);
#endif

	// find the minimum of opponent action value
	double min_value(int current_depth, chess_state input_state, int & nodes);

	// find max
	double max_value(int current_depth, chess_state input_state, int & nodes);

	// determine that whether a piece is captured by its opponent
	// in other word, check if it has opponent diagonally
	bool whether_captured(int x, int y, int player_type, chess_state input_state);

	// final minimax function to get the action current player can take
	location minimax(chess_state input_state, int & nodes);

	// max function for alpha beta pruning
	location max_alpha_beta(int current_depth, chess_state input_state, double alpha, double beta, int & nodes);

	// min function for alpha beta pruningf
	location min_alpha_beta(int current_depth, chess_state input_state, double alpha, double beta, int & nodes);

	location alpha_beta_search(chess_state &input_state, int & nodes);

	location player_action(chess_state input_state);

	location greedy_search(chess_state input_state);

	inline void print_state(chess_state input_state)
	{
		cout << "current player is: " << input_state.current_player << endl;
		cout << "current state is: " << endl;
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == 2)
				{
					cout << "_ ";
				}
				else
				{
					cout << input_state.current_state[i][j] << " ";
				}
			}
			cout << endl;
		}
		//system("pause");
		Sleep(500);
	}
};