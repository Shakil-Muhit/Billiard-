#ifndef USER_H
#define USER_H

struct User{
	int id;
	char name[20],password[20];

	void assign(User user);
};

void writeUserData();

void initUser();

int addUser(char username[],char pass[]);

int authenticate(char username[],char pass[],int &userId);

#endif
