#include "state.h"

void chess_state::take_action(location input_location)
{
	int action = input_location.current_action;
	if ((input_location.x < 0) || (input_location.x >= X_WIDTH) || (input_location.y < 0) || (input_location.y >= Y_WIDTH))
	{
		return;
	}
	switch (action)
	{
	case FORWARD:
		if (current_player == 0)
		{
			// move own player forward (up)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x - 1][input_location.y] = OWN;
			current_player = 1;
		}
		else
		{
			// move opponent player forward (down)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x + 1][input_location.y] = OPPONENT;
			current_player = 0;
		}
		break;
	case LEFT_FORWARD:
		if (current_player == 0)
		{
			// move own player forward (up)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x - 1][input_location.y - 1] = OWN;
			current_player = 1;
		}
		else
		{
			// move opponent player forward (down)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x + 1][input_location.y - 1] = OPPONENT;
			current_player = 0;
		}
		break;
	case RIGHT_FORWARD:
		if (current_player == 0)
		{
			// move own player forward (up)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x - 1][input_location.y + 1] = OWN;
			current_player = 1;
		}
		else
		{
			// move opponent player forward (down)
			current_state[input_location.x][input_location.y] = SPACE;
			current_state[input_location.x + 1][input_location.y + 1] = OPPONENT;
			current_player = 0;
		}
		break;
	default:
		break;
	}
	//current_player = 0;
	return;
}

double chess_state::defensive_heuristic(chess_state input_state)
{
	int remaining_piece = 0;
	switch (input_state.current_player)
	{
	case 0: // own
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OWN)
				{
					remaining_piece++;
					
				}
			}
		}
		break;
	case 1: // opponent
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OPPONENT)
				{
					remaining_piece++;
				}
			}
		}
		break;
	default:
		break;
	}
	return 2 * remaining_piece + (double)rand() / RAND_MAX;
}

double chess_state::smart_defensive_heuristic(chess_state input_state)
{
	if (game_over(input_state)) {
		if (input_state.current_player == OWN) {
			return win * 100;
		}
		else {
			return -win * 100;
		}
	}
	//double self_utility = defensive_heuristic(input_state) + 
	//double final_value = remaining_piece + distance_coefficient * near_the_base - can_be_captured * capture_coefficient
	//	+ win_percentage * win;
	/*double final_value = 2 * defensive_heuristic(input_state) + offensive_heuristic(input_state);
	return final_value;*/
	int self_remaining_piece = 0;
	int opponent_piece = 0;
	switch (input_state.current_player)
	{
	case 0: // own
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OWN)
				{
					self_remaining_piece++;

				}
			}
		}
		break;
	case 1: // opponent
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OPPONENT)
				{
					opponent_piece++;
				}
			}
		}
		break;
	default:
		break;
	}
	// 0 goes up; 1 goes down: 0's goal is reach row0, 1's goal is reach row7

	double distance_coefficient = 0;
	double can_be_captured = 0;
	double win_percentage = 0;
	int utility = 0;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				// calculate the distance from this piece to the base
				int goal_row = input_state.current_player * (X_WIDTH - 1);
				int distance_to_goal = abs(goal_row - i);
				int worthy_point = distance_to_goal;
				utility += worthy_point;
				//distance_coefficient += distance_to_base; 
				for (int dir = -1; dir <= 1; dir++)
				{
					if (input_state.current_state[i + 2 * (2 * input_state.current_player - 1)][j + dir] ==
						(location_type)((input_state.current_player + 1) % 2) && (dir == -1 || dir == 1))
					{
						can_be_captured++;
					}
				}
			}
		}
	}
	double final_value = utility + self_remaining_piece - opponent_piece + can_be_captured;
	return final_value;

}

double chess_state::offensive_heuristic(chess_state input_state)
{
	int remaining_piece = 0;
	int nega = 0;
	int almost_win = 0;
	int capture = 0;
	int neighbor = 0;
	switch (input_state.current_player)
	{
	case 0: // own
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OPPONENT)
				{/*
					for (int k = i; k < i + 3; k++)
					{
						for (int m = 0; m < Y_WIDTH; m++)
						{
							if (k <= X_WIDTH - 1)
							{
								if (input_state.current_state[k][m] == OWN)
								{
									neighbor++;
								}
							}
						}
					}
					if (input_state.current_state[i + 1][j] == OPPONENT)
					{
						nega = nega - 500;
					}
					if (input_state.current_state[i + 1][j + 1] == OPPONENT || input_state.current_state[i + 1][j - 1] == OPPONENT)
					{
						nega = nega - 200;
					}
					else if (input_state.current_state[i + 1][j] == SPACE && i == X_WIDTH - 1)
					{
						nega = nega + 2000;
					}
					if (input_state.current_state[i + 1][j + 1] == OWN || input_state.current_state[i + 1][j - 1] == OWN)
					{
						capture += 1000;
					}
					almost_win = almost_win + 10 * (X_WIDTH - i);*/
					remaining_piece++;
				}
			}
		}
		break;
	case 1: // opponent
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OWN)
				{/*
					for (int k = i; k < i - 3; k++)
					{
						for (int m = 0; m < Y_WIDTH; m++)
						{
							if (k >= 0)
							{
								if (input_state.current_state[k][m] == OWN)
								{
									neighbor++;
								}
							}
						}
					}
					if (input_state.current_state[i - 1][j] == OWN)
					{
						nega = nega - 500;
					}
					if (input_state.current_state[i - 1][j + 1] == OWN || current_state[i - 1][j - 1] == OWN)
					{
						nega = nega - 200;
					}
					else if (input_state.current_state[i - 1][j] == SPACE && i == 0)
					{
						nega = nega + 2000;
					}
					if (input_state.current_state[i - 1][j + 1] == OPPONENT || input_state.current_state[i - 1][j - 1] == OPPONENT)
					{
						capture += 1000;
					}
					almost_win = almost_win + 150 * i;*/
					remaining_piece++;
				}
			}
		}
		break;
	default:
		break;
	}
	//cout << 2 * (30 - remaining_piece) << endl;
	return 10 * (30 - remaining_piece) + (double)rand()/RAND_MAX  + nega - almost_win + capture + 100*neighbor;
}

double chess_state::smart_offensive_heuristic(chess_state input_state)
{
	if (game_over(input_state)) {
		if (input_state.current_player == OWN) {
			return win * 100;
		}
		else {
			return -win * 100;
		}
	}
	int self_remaining_piece = 0;
	int opponent_piece = 0;
	switch (input_state.current_player)
	{
	case 0: // own
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OWN)
				{
					self_remaining_piece++;

				}
			}
		}
		break;
	case 1: // opponent
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OPPONENT)
				{
					opponent_piece++;
				}
			}
		}
		break;
	default:
		break;
	}
	// 0 goes up; 1 goes down: 0's goal is reach row0, 1's goal is reach row7

	double distance_coefficient = 0;
	double can_be_captured = 0;
	double win_percentage = 0;
	int utility = 0;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				// calculate the distance from this piece to the base
				int goal_row = input_state.current_player * (X_WIDTH - 1);
				int distance_to_goal = abs(goal_row - i);
				int worthy_point = X_WIDTH - distance_to_goal;
				utility += worthy_point;
				//distance_coefficient += distance_to_base; 
				if (i == goal_row)
				{
					win_percentage++;
				}
				for (int dir = -1; dir <= 1; dir++)
				{
					if (input_state.current_state[i + 2 * (2 * input_state.current_player - 1)][j + dir] ==
						(location_type)((input_state.current_player + 1) % 2) && (dir == -1 || dir == 1))
					{
						can_be_captured++;
					}
				}
			}
		}
	}
	double final_value = utility + self_remaining_piece - opponent_piece + can_be_captured;
	return final_value;
}

#ifdef SIMPLE_RULE
bool chess_state::game_over(chess_state input_state)
{
	if (input_state.game_over_flag) return true;
	int legal_move = 0;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			for (int dir = -1; dir <= 1; dir++)
			{
				if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (i + 2 * input_state.current_player - 1 >= 0))
					&& (((input_state.current_state[i + 2 * input_state.current_player - 1][j + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
						|| ((input_state.current_state[i + 2 * input_state.current_player - 1][j + dir] == SPACE) && (dir == 0))))
				{
					legal_move++;
				}
			}
		}
	}
	if (legal_move == 0)
	{
		return true;
	}
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OWN)
			{
				return true;
			}
		}
	}
	for (int i = X_WIDTH - 1; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OPPONENT)
			{
				return true;
			}
		}
	}
	
	int own_captured = 0;
	int opponent_captured = 0;
	
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OWN)
				{
					own_captured++;
					
				}
			}
		}
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == OPPONENT)
				{
					opponent_captured++;
					
				}
			}
		}
	
	
	if (own_captured == 0 || opponent_captured == 0)
	{
		return true;
	}
	return false;
}
#endif

#ifdef EXTRA_RULE
bool chess_state::game_over(chess_state input_state)
{
	if (input_state.game_over_flag) return true;
	int legal_move = 0;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			for (int dir = -1; dir <= 1; dir++)
			{
				if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (i + 2 * input_state.current_player - 1 >= 0))
					&& (((input_state.current_state[i + 2 * input_state.current_player - 1][j + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
						|| ((input_state.current_state[i + 2 * input_state.current_player - 1][j + dir] == SPACE) && (dir == 0))))
				{
					legal_move++;
				}
			}
		}
	}
	if (legal_move == 0)
	{
		return true;
	}
	int a = 0;
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OWN)
			{
				a++;
			}
		}
	}
	if (a >= 3) return true;
	int b = 0;
	for (int i = X_WIDTH - 1; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OPPONENT)
			{
				b++;
			}
		}
	}
	if (b >= 3) return true;

	int own_captured = 0;
	int opponent_captured = 0;

	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OWN)
			{
				own_captured++;

			}
		}
	}
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == OPPONENT)
			{
				opponent_captured++;

			}
		}
	}


	if (own_captured == 2 || opponent_captured == 2)
	{
		return true;
	}
	return false;
}
#endif

double chess_state::min_value(int current_depth, chess_state input_state, int & nodes)
{
	//input_state.print_state();
	if (game_over(input_state)) return lose;
	if (current_depth == max_depth)
	{
#ifdef DUM_HN
		return defensive_heuristic(input_state);
#endif
#ifdef SMART_HN
		return offensive_heuristic(input_state);
#endif
	}
	double best_score = 99999;
	
	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}
	
	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				nodes++;
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;
				double max_score = max_value(current_depth + 1, input_state,nodes);
				//cout << "max_score:" << max_score << endl;
				if (max_score < best_score)
				{
					best_score = max_score;
				}

				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}

		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	//if (input_state.current_player == 1)
	//{
	//	for (int i = 0; i < X_WIDTH; i++)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OPPONENT)
	//			{
	//				//if (!whether_captured(i, j, OPPONENT, input_state))
	//				//{
	//					//cout << "min test:(" << i << "," << j <<")"<<endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{
	//					//cout << "ttt" << endl;
	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 1 <= X_WIDTH - 1) && (i + 1 >= 0))
	//						&& (((input_state.current_state[i + 1][j + dir] != OPPONENT) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "second min test:(" << i << "," << j << ")" << endl;
	//						int temp = input_state.current_state[i + 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i + 1][j + dir] = OPPONENT;
	//						int max_score = max_value(current_depth + 1, input_state);
	//						//cout << "max_score:" << max_score << endl;
	//						if (max_score < best_score)
	//						{
	//							best_score = max_score;
	//						}
	//						input_state.current_state[i][j] = OPPONENT;
	//						input_state.current_state[i + 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	for (int i = X_WIDTH - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OWN)
	//			{
	//				//if (!whether_captured(i, j, OPPONENT, input_state))
	//				//{
	//				//cout << "min test:(" << i << "," << j <<")"<<endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{
	//					//cout << "ttt" << endl;
	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
	//						&& (((input_state.current_state[i - 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i - 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "second min test:(" << i << "," << j << ")" << endl;
	//						int temp = input_state.current_state[i - 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i - 1][j + dir] = OWN;
	//						int max_score = max_value(current_depth + 1, input_state);
	//						//cout << "max_score:" << max_score << endl;
	//						if (max_score < best_score)
	//						{
	//							best_score = max_score;
	//						}
	//						input_state.current_state[i][j] = OWN;
	//						input_state.current_state[i - 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	return best_score;
}

double chess_state::max_value(int current_depth, chess_state input_state, int & nodes)
{
	//input_state.print_state();
	if (game_over(input_state)) return win;
	if (current_depth == max_depth)
	{
#ifdef DUM_HN
		return defensive_heuristic(input_state);
#endif
#ifdef SMART_HN
		return offensive_heuristic(input_state);
#endif
	}
	double best_score = -99999;
	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}

	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				nodes++;
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;

				double min_score = min_value(current_depth + 1, input_state, nodes);
				//cout << "max test:(" << i << "," << j << ")" << endl;
				//cout << "score:" << min_score << endl;
				if (min_score > best_score)
				{
					best_score = min_score;
				}
				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}

		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	//if (input_state.current_player == 0)
	//{
	//	for (int i = X_WIDTH - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OWN)
	//			{
	//				//if (!whether_captured(i, j, OWN, input_state))
	//				//{
	//					//cout << "not capture test: (" << i << "," << j << ")" << endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{

	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
	//						&& (((input_state.current_state[i - 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i - 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "test" << endl;
	//						int temp = input_state.current_state[i - 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i - 1][j + dir] = OWN;

	//						int min_score = min_value(current_depth + 1, input_state);
	//						//cout << "max test:(" << i << "," << j << ")" << endl;
	//						//cout << "score:" << min_score << endl;
	//						if (min_score > best_score)
	//						{
	//							best_score = min_score;
	//						}
	//						input_state.current_state[i][j] = OWN;
	//						input_state.current_state[i - 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < X_WIDTH; i++)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OPPONENT)
	//			{
	//				//if (!whether_captured(i, j, OWN, input_state))
	//				//{
	//				//cout << "not capture test: (" << i << "," << j << ")" << endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{

	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 1 <= X_WIDTH - 1) && (i + 1 >= 0))
	//						&& (((input_state.current_state[i + 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "test" << endl;
	//						int temp = input_state.current_state[i + 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i + 1][j + dir] = OWN;

	//						int min_score = min_value(current_depth + 1, input_state);
	//						//cout << "max test:(" << i << "," << j << ")" << endl;
	//						//cout << "score:" << min_score << endl;
	//						if (min_score > best_score)
	//						{
	//							best_score = min_score;
	//						}
	//						input_state.current_state[i][j] = OWN;
	//						input_state.current_state[i + 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	return best_score;
}

bool chess_state::whether_captured(int x, int y, int player_type, chess_state input_state)
{
	if (player_type == 0)
	{
		if (((input_state.current_state[x - 1][y + 1] == 1)&&(x-1<X_WIDTH)&&(x-1>=0)&&(y+1>=0)&&(y+1<Y_WIDTH))
			|| ((input_state.current_state[x - 1][y - 1] == 1)&&(x-1>=0)&&(x-1<X_WIDTH) && (y - 1 >= 0) && (y - 1<Y_WIDTH)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (((input_state.current_state[x + 1][y + 1] == 0) && (x + 1<X_WIDTH) && (x + 1 >= 0) && (y + 1 >= 0) && (y + 1<Y_WIDTH))
			|| ((input_state.current_state[x + 1][y - 1] == 0) && (x + 1 >= 0) && (x + 1<X_WIDTH) && (y - 1 >= 0) && (y - 1<Y_WIDTH)))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

chess_state::location chess_state::minimax(chess_state input_state, int & nodes)
{
	//input_state.print_state();
	location best_move;
	best_move.x = -1;
	best_move.y = -1;
	best_move.current_action = NO_MOVE;
	double best_score = -99999;
	int current_depth = 0;
	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}

	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				nodes++;
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;

				double min_score = min_value(current_depth + 1, input_state, nodes);
				//cout << "max test:(" << i << "," << j << ")" << endl;
				//cout << "score:" << min_score << endl;
				if (min_score > best_score)
				{
					best_score = min_score;
					best_move.x = x;
					best_move.y = y;
					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
					best_move.evaluation = best_score;
				}
				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}

		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	//if (input_state.current_player == 0)
	//{
	//	for (int i = X_WIDTH - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OWN)
	//			{
	//				//if (!whether_captured(i, j, OWN, input_state))
	//				//{
	//					for (int dir = -1; dir <= 1; dir++)
	//					{
	//						if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
	//							&& (((input_state.current_state[i - 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//								|| ((input_state.current_state[i - 1][j + dir] == SPACE) && (dir == 0)))) 
	//						{
	//							int temp = input_state.current_state[i - 1][j + dir];
	//							input_state.current_state[i][j] = SPACE;
	//							input_state.current_state[i - 1][j + dir] = OWN;
	//							int min_score = min_value(current_depth, input_state);
	//							if (min_score > best_score)
	//							{
	//								best_score = min_score;
	//								best_move.x = i;
	//								best_move.y = j;
	//								if (dir == -1)
	//								{
	//									best_move.current_action = LEFT_FORWARD;
	//								}
	//								else if (dir == 0)
	//								{
	//									best_move.current_action = FORWARD;
	//								}
	//								else
	//								{
	//									best_move.current_action = RIGHT_FORWARD;
	//								}
	//								best_move.evaluation = best_score;
	//							}

	//							input_state.current_state[i][j] = OWN;
	//							input_state.current_state[i - 1][j + dir] = temp;
	//						}
	//					}
	//				//}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	//best_score = 99999;
	//	for (int i = 0; i < X_WIDTH; i++)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OPPONENT)
	//			{
	//				//if (!whether_captured(i, j, OPPONENT, input_state))
	//				//{
	//					
	//					for (int dir = -1; dir <= 1; dir++)
	//					{
	//						if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 1 <= X_WIDTH - 1) && (i + 1 >= 0))
	//							&& (((input_state.current_state[i + 1][j + dir] != OPPONENT) && ((dir == -1) || (dir == 1)))
	//								|| ((input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))))
	//						{
	//							int temp = input_state.current_state[i + 1][j + dir];
	//							input_state.current_state[i][j] = SPACE;
	//							input_state.current_state[i + 1][j + dir] = OPPONENT;
	//							
	//							int max_score = min_value(current_depth, input_state);
	//							//cout << "max_score is: " << max_score + 2 << endl;
	//							if (max_score > best_score)
	//							{
	//								best_score = max_score;
	//								best_move.x = i;
	//								best_move.y = j;
	//								if (dir == -1)
	//								{
	//									best_move.current_action = LEFT_FORWARD;
	//								}
	//								else if (dir == 0)
	//								{
	//									best_move.current_action = FORWARD;
	//								}
	//								else
	//								{
	//									best_move.current_action = RIGHT_FORWARD;
	//								}
	//								best_move.evaluation = best_score;
	//								//cout << "best score is: " << best_score << "in location: (" << i << "," << j << ")" << endl;
	//							}
	//							input_state.current_state[i][j] = OPPONENT;
	//							input_state.current_state[i + 1][j + dir] = temp;
	//						}
	//					}
	//				//}
	//			}
	//		}
	//	}
	//}
	return best_move;
}

chess_state::location chess_state::max_alpha_beta(int current_depth, chess_state input_state, double alpha, double beta, int & nodes)
{
	//input_state.print_state();
	chess_state::location best_move;
	if (game_over(input_state))
	{
		best_move.evaluation = lose;
		return best_move;
	}
	if (current_depth == max_depth_ab)
	{
#ifdef DUM_HN
		best_move.evaluation = defensive_heuristic(input_state);
#endif
#ifdef SMART_HN
		best_move.evaluation = offensive_heuristic(input_state);
#endif
		return best_move;
	}
	double best_score = -99999;
	best_move.evaluation = best_score;
	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}

	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				nodes++;
				//cout << "(" << x + 2 * input_state.current_player - 1 << "," << y + dir << ")" << endl;
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;

				double min_score = min_alpha_beta(current_depth + 1, input_state, alpha, beta, nodes).evaluation;

				if (min_score > best_score)
				{
					best_move.evaluation = min_score;
					best_move.x = x;
					best_move.y = y;

					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
					best_score = min_score;
				}

				if (best_score >= beta)
				{
					best_move.evaluation = best_score;
					best_move.x = x;
					best_move.y = y;

					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
					return best_move;
				}
				//cout << "max test:(" << i << "," << j << ")" << endl;
				//cout << "score:" << min_score << endl;
				if (best_score > alpha)
				{
					alpha = best_score;
				}
				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}

		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	//if (input_state.current_player == 0)
	//{
	//	for (int i = X_WIDTH - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OWN)
	//			{
	//				//if (!whether_captured(i, j, OWN, input_state))
	//				//{
	//				//cout << "not capture test: (" << i << "," << j << ")" << endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{

	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
	//						&& (((input_state.current_state[i - 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i - 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "test" << endl;
	//						int temp = input_state.current_state[i - 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i - 1][j + dir] = OWN;

	//						int min_score = min_alpha_beta(current_depth + 1, input_state,alpha,beta).evaluation;
	//						
	//						if (min_score > best_score)
	//						{
	//							best_move.evaluation = min_score;
	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							best_score = min_score;
	//						}

	//						if (best_score >= beta)
	//						{
	//							best_move.evaluation = best_score;
	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							return best_move;
	//						}
	//						//cout << "max test:(" << i << "," << j << ")" << endl;
	//						//cout << "score:" << min_score << endl;
	//						if (best_score > alpha)
	//						{
	//							alpha = best_score;
	//						}
	//						input_state.current_state[i][j] = OWN;
	//						input_state.current_state[i - 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	for (int i = 0; i < X_WIDTH; i++)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OPPONENT)
	//			{
	//				//if (!whether_captured(i, j, OWN, input_state))
	//				//{
	//				//cout << "not capture test: (" << i << "," << j << ")" << endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{

	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 1 <= X_WIDTH - 1) && (i + 1 >= 0))
	//						&& (((input_state.current_state[i + 1][j + dir] != OPPONENT) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "test" << endl;
	//						int temp = input_state.current_state[i + 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i + 1][j + dir] = OPPONENT;

	//						int min_score = min_alpha_beta(current_depth + 1, input_state, alpha, beta).evaluation;

	//						if (min_score > best_score)
	//						{
	//							best_move.evaluation = min_score;
	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							best_score = min_score;
	//						}

	//						if (best_score >= beta)
	//						{
	//							best_move.evaluation = best_score;
	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							return best_move;
	//						}
	//						//cout << "max test:(" << i << "," << j << ")" << endl;
	//						//cout << "score:" << min_score << endl;
	//						if (best_score > alpha)
	//						{
	//							alpha = best_score;
	//						}
	//						input_state.current_state[i][j] = OPPONENT;
	//						input_state.current_state[i + 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	return best_move;
}

chess_state::location chess_state::min_alpha_beta(int current_depth, chess_state input_state, double alpha, double beta, int & nodes)
{
	//input_state.print_state();
	chess_state::location best_move;
	double best_score = 99999;
	best_move.evaluation = best_score;
	//int valid_move = 0;
	if (game_over(input_state))
	{
		best_move.evaluation = win;
		return best_move;
	}
	if (current_depth == max_depth_ab)
	{
#ifdef DUM_HN
		best_move.evaluation = defensive_heuristic(input_state);
#endif
#ifdef SMART_HN
		best_move.evaluation = offensive_heuristic(input_state);
#endif
		return best_move;
	}

	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}
	
	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				nodes++;
				//valid_move++;
				//cout << "second min test:(" << i << "," << j << ")" << endl;
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;
				double max_score = max_alpha_beta(current_depth + 1, input_state, alpha, beta, nodes).evaluation;
				//cout << "max_score:" << max_score << endl;
				if (max_score < best_score)
				{
					best_score = max_score;
					best_move.evaluation = max_score;

					best_move.x = x;
					best_move.y = y;

					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
				}
				if (best_score <= alpha)
				{
					best_move.evaluation = best_score;

					best_move.x = x;
					best_move.y = y;

					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
					return best_move;
				}
				if (best_score < beta)
				{
					beta = best_score;
				}
				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}

		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	//if (input_state.current_player == 1)
	//{

	//	for (int i = 0; i < X_WIDTH; i++)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OPPONENT)
	//			{
	//				//if (!whether_captured(i, j, OPPONENT, input_state))
	//				//{
	//				//cout << "min test:(" << i << "," << j <<")"<<endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{
	//					//cout << "ttt" << endl;
	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i + 1 <= X_WIDTH - 1) && (i + 1 >= 0))
	//						&& (((input_state.current_state[i + 1][j + dir] != OPPONENT) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "second min test:(" << i << "," << j << ")" << endl;
	//						int temp = input_state.current_state[i + 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i + 1][j + dir] = OPPONENT;
	//						int max_score = max_alpha_beta(current_depth + 1, input_state,alpha,beta).evaluation;
	//						//cout << "max_score:" << max_score << endl;
	//						if (max_score < best_score)
	//						{
	//							best_score = max_score;
	//							best_move.evaluation = max_score;

	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//						}
	//						if (best_score <= alpha)
	//						{
	//							best_move.evaluation = best_score;

	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							return best_move;
	//						}
	//						if (best_score < beta)
	//						{
	//							beta = best_score;
	//						}
	//						input_state.current_state[i][j] = OPPONENT;
	//						input_state.current_state[i + 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	for (int i = X_WIDTH - 1; i >= 0; i--)
	//	{
	//		for (int j = 0; j < Y_WIDTH; j++)
	//		{
	//			if (input_state.current_state[i][j] == OWN)
	//			{
	//				//if (!whether_captured(i, j, OPPONENT, input_state))
	//				//{
	//				//cout << "min test:(" << i << "," << j <<")"<<endl;
	//				for (int dir = -1; dir <= 1; dir++)
	//				{
	//					//cout << "ttt" << endl;
	//					if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
	//						&& (((input_state.current_state[i - 1][j + dir] != OWN) && ((dir == -1) || (dir == 1)))
	//							|| ((input_state.current_state[i - 1][j + dir] == SPACE) && (dir == 0))))
	//					{
	//						//cout << "second min test:(" << i << "," << j << ")" << endl;
	//						int temp = input_state.current_state[i - 1][j + dir];
	//						input_state.current_state[i][j] = SPACE;
	//						input_state.current_state[i - 1][j + dir] = OWN;
	//						int max_score = max_alpha_beta(current_depth + 1, input_state, alpha, beta).evaluation;
	//						//cout << "max_score:" << max_score << endl;
	//						if (max_score < best_score)
	//						{
	//							best_score = max_score;
	//							best_move.evaluation = max_score;
	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//						}
	//						if (best_score <= alpha)
	//						{
	//							best_move.evaluation = best_score;

	//							best_move.x = i;
	//							best_move.y = j;

	//							if (dir == -1)
	//							{
	//								best_move.current_action = LEFT_FORWARD;
	//							}
	//							else if (dir == 0)
	//							{
	//								best_move.current_action = FORWARD;
	//							}
	//							else
	//							{
	//								best_move.current_action = RIGHT_FORWARD;
	//							}
	//							return best_move;
	//						}
	//						if (best_score < beta)
	//						{
	//							beta = best_score;
	//						}
	//						input_state.current_state[i][j] = OWN;
	//						input_state.current_state[i - 1][j + dir] = temp;
	//					}
	//				}
	//				//}
	//			}
	//		}
	//	}
	//}
	return best_move;
}

chess_state::location chess_state::alpha_beta_search(chess_state& input_state, int & nodes)
{
	chess_state::location best_move;
	best_move = max_alpha_beta(0, input_state, -9999999, 9999999, nodes);
	if (best_move.evaluation == 99999)
	{
		for (int i = 0; i < X_WIDTH; i++)
		{
			for (int j = 0; j < Y_WIDTH; j++)
			{
				if (input_state.current_state[i][j] == (location_type)input_state.current_player)
				{
					for (int dir = -1; dir <= 1; dir++)
					{
						if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
							&& (input_state.current_state[i + 2 * input_state.current_player - 1][j + dir] != (location_type)input_state.current_player)
							&& ((dir == -1) || (dir == 1)))
						{
							best_move.x = i;
							best_move.y = j;

							if (dir == -1)
							{
								best_move.current_action = LEFT_FORWARD;
							}
							else if (dir == 0)
							{
								best_move.current_action = FORWARD;
							}
							else
							{
								best_move.current_action = RIGHT_FORWARD;
							}
							return best_move;
						}
						else if (((j + dir >= 0) && (j + dir <= Y_WIDTH - 1) && (i - 1 <= X_WIDTH - 1) && (i - 1 >= 0))
							&& (input_state.current_state[i + 1][j + dir] == SPACE) && (dir == 0))
						{

							best_move.x = i;
							best_move.y = j;

							if (dir == -1)
							{
								best_move.current_action = LEFT_FORWARD;
							}
							else if (dir == 0)
							{
								best_move.current_action = FORWARD;
							}
							else
							{
								best_move.current_action = RIGHT_FORWARD;
							}
							return best_move;
						}
					}
				}
			}
		}
	}
	if (best_move.evaluation == 99999)
	{
		input_state.game_over_flag = 1;
	}
	return best_move;
}

chess_state::location chess_state::player_action(chess_state input_state)
{
	location player_act;
	player_act.x = -1;
	player_act.y = -1;
	player_act.current_action = NO_MOVE;
	player_act.evaluation = -1;

	int valid_move = -1;
	while (input_state.current_state[player_act.x][player_act.y] != OWN || valid_move == -1)
	{
		cout << "Enter the piece you want to move. " << endl;

		cout << "Please enter the row number firstly in the range of " << 0 << " to " << X_WIDTH - 1 << ": ";
		cin >> player_act.x;
		cout << endl;
		while (player_act.x < 0 || player_act.x > X_WIDTH - 1)
		{
			cout << "The input is not in valid range, please enter again: ";
			cin >> player_act.x;
			cout << endl;
		}

		cout << "Please enter the column number firstly in the range of " << 0 << " to " << Y_WIDTH - 1 << ": ";
		cin >> player_act.y;
		cout << endl;
		while (player_act.y < 0 || player_act.y > Y_WIDTH - 1)
		{
			cout << "The input is not in valid range, please enter again: ";
			cin >> player_act.y;
			cout << endl;
		}

		valid_move = 0;
		if (input_state.current_state[player_act.x][player_act.y] != OWN)
		{
			cout << "There is no valid piece, please enter the correct information again." << endl;
			valid_move = -1;
		}
		else
		{
			if ((player_act.x - 1 >= 0) && input_state.current_state[player_act.x - 1][player_act.y] != SPACE &&
				(input_state.current_state[player_act.x - 1][player_act.y + 1] == OWN && player_act.y + 1 < Y_WIDTH) &&
				(input_state.current_state[player_act.x - 1][player_act.y - 1] == OWN && player_act.y - 1 >= 0))
			{
				valid_move = -1;
				cout << "There is no valid move for this piece, please choose another piece. " << endl;
			}
		}
	}

	int direction_check = -1;
	while (direction_check == -1)
	{
		cout << "Please choose a direction to move, just enter the number to matching direction." << endl;
		cout << "Enter 0 to move directly forward, enter 1 to move left forward, enter 2 to move right forward." << endl;
		int temp;
		cin >> temp;
		player_act.current_action = (action_set)temp;
		while (temp < 0 || temp > 2)
		{
			cout << "Invalid move, please choose one valid direction: ";
			cin >> temp;
			player_act.current_action = (action_set)temp;
		}
		direction_check = temp;
		if (temp == 0)
		{
			if (input_state.current_state[player_act.x - 1][player_act.y] != SPACE)
			{
				cout << "The piece cannot move to the position, there is already one piece." << endl;
				cout << "Please choose one valid move. " << endl;
				direction_check = -1;
			}
		}
		else if(temp == 1)
		{
			if (input_state.current_state[player_act.x - 1][player_act.y - 1] == OWN)
			{
				cout << "The piece cannot move to the position, there is already one piece." << endl;
				cout << "Please choose one valid move. " << endl;
				direction_check = -1;
			}
		}
		else
		{
			if (input_state.current_state[player_act.x - 1][player_act.y + 1] == OWN)
			{
				cout << "The piece cannot move to the position, there is already one piece." << endl;
				cout << "Please choose one valid move. " << endl;
				direction_check = -1;
			}
		}
	}
	return player_act;
}

chess_state::location chess_state::greedy_search(chess_state input_state)
{
	location best_move;
	best_move.x = -1;
	best_move.y = -1;
	best_move.evaluation = -999999;
	best_move.current_action = NO_MOVE;
	std::vector<location> all_pieces;
	for (int i = 0; i < X_WIDTH; i++)
	{
		for (int j = 0; j < Y_WIDTH; j++)
		{
			if (input_state.current_state[i][j] == (location_type)input_state.current_player)
			{
				location temp;
				temp.x = i;
				temp.y = j;
				all_pieces.push_back(temp);
			}
		}
	}
	double best_score = -999999;
	while (all_pieces.size() > 0)
	{
		int index = rand() % all_pieces.size();
		int x = all_pieces[index].x;
		int y = all_pieces[index].y;
		for (int dir = -1; dir <= 1; dir++)
		{
			if (((y + dir >= 0) && (y + dir <= Y_WIDTH - 1) && (x + 2 * input_state.current_player - 1 <= X_WIDTH - 1) && (x + 2 * input_state.current_player - 1 >= 0))
				&& (((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] != (location_type)input_state.current_player) && ((dir == -1) || (dir == 1)))
					|| ((input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] == SPACE) && (dir == 0))))
			{
				
				int temp = input_state.current_state[x + 2 * input_state.current_player - 1][y + dir];
				input_state.current_state[x][y] = SPACE;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = (location_type)input_state.current_player;

				double min_score = defensive_heuristic(input_state);

				if (min_score > best_score)
				{
					best_move.evaluation = min_score;
					best_move.x = x;
					best_move.y = y;

					if (dir == -1)
					{
						best_move.current_action = LEFT_FORWARD;
					}
					else if (dir == 0)
					{
						best_move.current_action = FORWARD;
					}
					else
					{
						best_move.current_action = RIGHT_FORWARD;
					}
					best_score = min_score;
				}

				input_state.current_state[x][y] = (location_type)input_state.current_player;
				input_state.current_state[x + 2 * input_state.current_player - 1][y + dir] = temp;
			}
		
		}
		std::vector<location> temp_vector;
		for (size_t i = 0; i < all_pieces.size(); i++)
		{
			if (all_pieces[i].x != x || all_pieces[i].y != y)
			{
				temp_vector.push_back(all_pieces[i]);
			}
		}
		all_pieces = temp_vector;
	}
	return best_move;
}
