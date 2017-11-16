#include "state.h"

int main()
{
	int smart_wins = 0;
	int dumb_wins = 0;
	for (int i = 0; i < 50; i++)
	{
		int expanded_nodes_1 = 0;
		int expanded_nodes_0 = 0;
		int player_1_move = 0;
		int player_0_move = 0;
		clock_t start, end;
		start = clock();
		
		int random = rand() % 2;
		chess_state a(random);
		cout << "The starting player is: " << random << endl;
		while (!a.game_over(a))
		{
			chess_state::location next_action;

			if (a.current_player == 1)
			{
				//next_action = a.minimax(a,expanded_nodes_1);
				next_action = a.alpha_beta_search(a,expanded_nodes_1);
				player_1_move++;
				//std::cout << "current value is: " << next_action.evaluation << endl;
			}
			else
			{
				next_action = a.minimax(a, expanded_nodes_0);
				player_0_move++;
				//std::cout << "current value is: " << next_action.evaluation << endl;
				//next_action = a.alpha_beta_search(a,expanded_nodes
				//next_action = a.player_action(a,expanded_nodes_0);
				//next_action = a.greedy_search(a,expanded_nodes_0);
			}
			if (next_action.evaluation == 99999)
			{
				std::cout << "next action evaluation is 99999." << endl;
				std::cout << "(" << next_action.x << "," << next_action.y << ")" << endl;
				//a.print_state();
			}
			if (next_action.x == -1)
			{
				std::cout << "current action is move ";
				if (next_action.current_action == 0)
				{
					std::cout << "forward. " << endl;
				}
				else if (next_action.current_action == 1)
				{
					std::cout << "left forward. " << endl;
				}
				else
				{
					std::cout << "right forward. " << endl;
				}
				std::cout << "in coordinate (" << next_action.x << "," << next_action.y << ")" << endl;
				a.print_state(a);
			}


			a.take_action(next_action);
			a.print_state(a);
/*
			if (a.game_over(a))
			{
				cout << "End: " << endl;
			}*/
		}
		a.print_state(a);
		end = clock();
		float total_time = (float(end) - (float)start)/1000;
		float average_player_1_time_per_move = total_time / player_1_move;
		float average_player_0_time_per_move = total_time / player_0_move;
		int average_nodes_1 = expanded_nodes_1 / player_1_move;
		int average_nodes_0 = expanded_nodes_0 / player_0_move;
		std::cout << "Total time is: " << total_time << endl;
		std::cout << "Total expanded node for player 1 is: " << expanded_nodes_1 << endl;
		std::cout << "Average time per move for player 1 is: " << average_player_1_time_per_move << endl;
		std::cout << "Average expanded nodes per move for player 1 is: " << average_nodes_1 << endl;
		std::cout << "Total expanded node for player 0 is: " << expanded_nodes_0 << endl;
		std::cout << "Average time per move for player 0 is: " << average_player_0_time_per_move << endl;
		std::cout << "Average expanded nodes per move for player 0 is: " << average_nodes_0 << endl;

		int winner = (a.current_player + 1) % 2;
		std::cout << "The winning is: ";
		if (winner == 1)
		{
			std::cout << "smart one." << endl;
			smart_wins++;
		}
		else
		{
			std::cout << "dumb one." << endl;
			dumb_wins++;
		}
		system("pause");
	}

	std::cout << "The final result is: SMART WINS: " << smart_wins << " DUMB WINS: " << dumb_wins << endl;
	
	system("pause");
	return 0;
}