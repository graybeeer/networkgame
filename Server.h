#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���
#include <stdio.h> // printf(), ...
#include "resource.h"
#include "GameFrame.h"

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define NICKBUFSIZE 21

DWORD WINAPI roomServerThread(LPVOID arg);
DWORD WINAPI roomClientThread(LPVOID arg);
DWORD WINAPI roomDataProcessingThread(LPVOID arg);
DWORD WINAPI roomDataResendThread(LPVOID arg);

DWORD WINAPI inGameServerThread(LPVOID arg);

typedef struct PLAYERINFO {
	char nickname[NICKBUFSIZE] = "";
	bool isReady = false;
	SOCKET sock = INVALID_SOCKET;
}PlayerInfo;

class WAITING_ROOM
{
public:
	WAITING_ROOM();
	~WAITING_ROOM();
	WAITING_ROOM(const WAITING_ROOM& wr);
	int MAKE_ROOM(char* serverport);
	int CONNECT_ROOM(char* serverip, char* name, char* serverport);

	int		FindBlankPlayer();
	bool	checkNickReduplication(char* name);
	bool	checkJoin(char* name);
	int		stringAnalysis(char* recvdata);

	void	pressReady();
	bool	checkAllReady();
	void	pressStart();

	void	enableConnectGui(bool isEnable);
	void	printErrorEditbox(char* errstr);


	int			GetMyNum();
	void		SetMyNum(int in);
	SOCKET		GetMySock();
	void		SetMySock(SOCKET in);
	HWND		GetDlgHandle();
	void		SetDlgHandle(HWND in);
	bool		GetIsHost();


private:
	WSADATA				wsa;
	SOCKET				my_sock;
	struct sockaddr_in	serveraddr;

	bool				is_ready{ false };
	bool				is_host{ false };
	int					my_num{ -1 };
	HWND				DlgHandle;
};

struct CLIENTINFO {
	SOCKET sock;
	HWND dlg;
	short num;
};
class INGAME
{
public:
	INGAME();
	~INGAME();

	int			GetMyNum();
	SOCKET		GetMySock();
	int			stringAnalysis(char* recvdata);

private:
	SOCKET				my_sock;

	int					my_num{ -1 };
	bool				is_host{ false };
};