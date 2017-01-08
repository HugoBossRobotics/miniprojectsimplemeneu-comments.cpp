#include <ros/ros.h>
#include <iostream>
#include <string>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <std_msgs/String.h>
#include <time.h>
#include <stdlib.h>

//typedef is used to declare a new type of information, based on an old one.
//In our case it is used to make it more convenient - using a custom name, instead of the whole actionlib name.
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> GoalAndAvoid;

//integer argc represents the argument count. Character argv is an argument vector.
//We specify those when this program is using or is used by another program.
int main(int argc, char** argv){

//ros::init initializes the node and gives it a name "coor".
//This has to be done prior to every function which communicates with the node.
  ros::init(argc, argv, "coor");

/*
Constructs an action client, which allows the action move_base to run in background and allows the program to
proceed with next function (spins a thread). In the script of the action "move_base" an action server is created.
Therefore, by constructing this action client we create a server as well.
The action client itself establishes connection with the move_base action.
This starts the move_base node and connects it to the kobuki.
*/
  GoalAndAvoid ac("move_base", true);

//while loop, which on each 5 seconds tells the user that it waits for the server to connect.
  int a=0;
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for server to connect...");
    a=a+1;

/*
since we set a counter "a", which will increase by one on each 5 seconds,with the following if statement
we order the program to kill the process after the 6-th message (after 6*5=30 seconds).
*/
    if (a>5)
	{
	ROS_INFO("Failed to connect to server. Check connection
          between the robot and the computer and try again.");
	ros::shutdown();
	return 0;
	}

  }

  ROS_INFO("Connected.");

// Creates a goal for the move_base node and specifies the messages which will be used to communicate with the server.
// However, the goal has no coordinates yet.
  move_base_msgs::MoveBaseGoal goal;

 //the frame is defining which coordinate system to use in order to process the coordinates
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

/*
All examples from class have "using namespace std;" right under the libraries. This tells the program where to
look in order to find the basic functions like cout and cin. However, when we haven't specified that in the beginning,
we need to tell the program manually to look in the std namespace before each function. So the following three lines
basically define two strings and one integer, which we will use later.
*/
  int inputInt=0;
  std::string menu;
  std::string tryagain;

/*
To understand that, please read what is a do-while loop first. Then try the program to see what it can do.
If you have done that - proceed with this. Basically we need two do-while loops. We need them so that the
user interface will always have the possibility to run the "Menu" again without having to restart the program.
The first do-while loop (or the outer one) is used when the program reaches its end and the user requests to see
the menu again. That will be executed in most cases, because even if the user decides to use manual control, after
he closes the teleop, the program will proceed and will reach the end of the script, where we state or while loop.
So while the user keeps saying yes to the question "do you want to go back to the menu", it will execute the "do"
part of the outer do-while loop.
*/
do{

/*
The second do-while loop is used only if the user enters an invalid number from the menu. Then a message will pop up,
saying that this option is not available and will ask the user whether he wants to go back to menu. Here comes the while part.
So while the user keeps saying yes to that question, it will execute the "do" part.
*/
  do{

/*
Ros info is similar to "cout" with a small difference. If you have tried the program, you have noticed the numbers
on the left from the menu. If you use "cout" instead, they will be on the right side of the menu.
*/
  std::cout << "<<<<<<<<<<<<<<<<<<<<<<MENU>>>>>>>>>>>>>>>>>>>>>>>"<<std::endl<<std::endl;
  std::cout << "Press 1 and ENTER to send the robot to room 1."<<std::endl<<std::endl;
  std::cout << "Press 2 and ENTER to send the robot to room 2."<<std::endl<<std::endl;
  std::cout << "Press 3 and ENTER to send the robot to room 3."<<std::endl<<std::endl;
  std::cout << "Press 4 and ENTER to go to to the charging point."<<std::endl<<std::endl;
  std::cout << "Press 5 and ENTER for manual control of the robot."<<std::endl<<std::endl;
  std::cout << "Press 6 and ENTER to shut down this program."<<std::endl<<std::endl;




/*
After we have given directions to the user, he will have to input a number and store it in the integer inputInt.
We have defined it before the do-while loops.
*/
  std::cin>> inputInt;

 /*
 Please read about switch-case before this part. This statement allows the program to check the input of the user (inputInt)
 with the different cases we can have. So in case he has written "1" then the first case will be executed.
 Note that we can have "case Room1:", its no problem, but we need to store that into a string, not an integer.
 */
  switch(inputInt){
  case 1:
  goal.target_pose.pose.position.x = 2.6; //example coordinate for x
  goal.target_pose.pose.position.y = -15.2; //example coordinate for y
  goal.target_pose.pose.orientation.w = 1.0; //example orientation for w
  std::cout<< "Taking you to room 1";
  break;
  /*
  if we don't put the break, then it will automatically execute everything until it reaches a "break"
    or the end of the switch-case statement.
    */

  case 2:
  goal.target_pose.pose.position.x = 0.342; //example coordinate for x
  goal.target_pose.pose.position.y = -15.7; //example coordinate for y
  goal.target_pose.pose.orientation.w = 1.0; //example orientation for w
  std::cout<< "Taking you to room 2";
  break;

  case 3:
  goal.target_pose.pose.position.x = 5.342; //example coordinate for x
  goal.target_pose.pose.position.y = -11.7; //example coordinate for y
  goal.target_pose.pose.orientation.w = 1.0; //ecample orientation for w
  std::cout<< "Taking you to room 3";
  break;

  case 4:
  goal.target_pose.pose.position.x = 0.342; //example coordinate for x
  goal.target_pose.pose.position.y = -1.7; //example coordinate for y
  goal.target_pose.pose.orientation.w = 1.0; //ecample orientation for w
  std::cout<< "Taking you to the charging point";
  break;

  case 5://Case 5 allows the user to control the bot manually. The function "system" will run the command in the brackets.
system("roslaunch turtlebot_teleop keyboard_teleop.launch");
  break;

  case 6:
   ros::shutdown();
   return 0;
   break;

  default:
      /*
      the default case means that if none of the cases matches the user input, it will execute the
      actions in the default section.
      */

  ROS_INFO("Option not found. Do you want to try again? (Y or N)");
  //here we ask the user to answer if he wants to enter the menu again.

 std::cin >> tryagain;
  if(tryagain=="N" || tryagain=="n"){ //if he says no, the process will be killed.
	  ros::shutdown();
	  return 0;
  }
  }
 }while(tryagain=="Y" || tryagain=="y"); //while he keeps saying yes to the previous question, the menu will be executed again.

  if (inputInt!= 5){
        /*if the user did not chose the teleop function, the action client will send the coordinates to the server.
        When the coordinates are sent to the server, the program will tell the user that the robot
        has received the coordinates and now will navigate to the room
        */

ac.sendGoal(goal); //The action client is sending the goal to the server.
 ROS_INFO("Goal set.");

//The action client is waiting for feedback from the server.
  ac.waitForResult();

//The following if-esle statement will compare the coordinates of the goal with the current location of the robot.
//The user will be notified on whether the goal was reached ot not.
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("The target has been reached.");
		if(inputInt == 4){
			std::cout << "Move the robot exactly towards the charging point. Activating manual control."<<std::endl;
			std::cout << "Click Ctr and C to close manual control and activate auto-docking"<<std::endl;
			std::cout << "Type <ok> to continue"<<std::endl;
			std::string enter;
			std::cin>> enter;

			system("roslaunch turtlebot_teleop keyboard_teleop.launch");
			system("roslaunch kobuki_auto_docking activate.launch --screen");
		}
  else
    ROS_INFO("The target has not been reached.");
}
	std::cout<< "Go back to menu? (Y or N)";
	//after the robot managed or failed to reach the goal, the user will be asked if he wants to enter the menu again.

	std::cin >> menu;
  }while(menu=="Y" || menu=="y"); //while he says yes, he will be sent to the menu. Otherwise the program will be turned off.
  return 0;
}
