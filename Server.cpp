#include "Server.h"

DWORD WINAPI roomServerThread(LPVOID arg)
{
	WAITING_ROOM wr_server = *(WAITING_ROOM*)arg;
	SOCKET client_sock;
	SOCKET server_sock = wr_server.GetMySock();
	struct sockaddr_in clientaddr;
	int addrlen = sizeof(clientaddr);
	HANDLE hnd;

	while (1) {
		client_sock = accept(server_sock, (struct sockaddr*)&clientaddr, &addrlen);
		short cl_num = wr_server.FindBlankPlayer();
		if (cl_num == -1) {
			closesocket(client_sock);
			continue;
		};
		wr_server.SetMyNum(cl_num);
		wr_server.SetMySock(client_sock);
		hnd = CreateThread(NULL, 0, roomDataProcessingThread, (LPVOID)&wr_server, 0, NULL);
		CloseHandle(hnd);
	}

	return 0;
}

DWORD WINAPI roomClientThread(LPVOID arg)
{
	WAITING_ROOM wr_server = *(WAITING_ROOM*)arg;

	SOCKET sv_sock = wr_server.GetMySock();
	HWND hDlg = wr_server.GetDlgHandle();
	char recvcode[30];

	int retval = recv(sv_sock, recvcode, NICKBUFSIZE, MSG_WAITALL);
	SetDlgItemTextA(hDlg, IDC_HOSTNAME, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME

	while (1) {
		retval = recv(sv_sock, recvcode, 3, MSG_WAITALL);

		// 경우에 따라 상호작용 종료
		if (wr_server.stringAnalysis(recvcode) == -1) {
			break;
		}
	}

	// 서버와의 연결 종료와 처리
	for (int i{}; i < 4; ++i)
		SetDlgItemTextA(hDlg, IDC_HOSTNAME + i, ""); // IDC_P1NAME + n = IDC_P(n+1)NAME
	wr_server.enableConnectGui(TRUE);
	closesocket(sv_sock);
	return 0;
}

DWORD WINAPI roomDataProcessingThread(LPVOID arg)
{
	struct WAITING_ROOM wr_server = *(struct WAITING_ROOM*)arg;

	short cl_num = wr_server.GetMyNum();
	SOCKET cl_sock = wr_server.GetMySock();
	HWND hDlg = wr_server.GetDlgHandle();
	char recvcode[30];
	int retval = 0;

	while (1) {
		int retval = recv(cl_sock, recvcode, 3, MSG_WAITALL);

		// 경우에 따라 상호작용 종료
		if (wr_server.stringAnalysis(recvcode) == -1 || retval <= 0)
			break;
	}

	// 클라이언트 cl_num과의 연결 종료와 처리
	SetDlgItemTextA(hDlg, IDC_P1NAME + cl_num, ""); // IDC_P1NAME + n = IDC_P(n+1)NAME
	closesocket(cl_sock);
	return 0;
}

DWORD WINAPI roomDataResendThread(LPVOID arg)
{
	struct WAITING_ROOM wr_server = *(struct WAITING_ROOM*)arg;

	short cl_num = wr_server.GetMyNum();
	SOCKET cl_sock = wr_server.GetMySock();
	HWND hDlg = wr_server.GetDlgHandle();
	char tmpbuf[NICKBUFSIZE];
	char tmpstr[2];
	int retval = 0;


	bool beforeReadyStatus[3]; // 이전 레디상태와 비교하여 dlg에 변화가 있으면 재전송하는 변수
	for (int i{}; i < 3; ++i) {
		GetDlgItemTextA(hDlg, IDC_P1READY + i, tmpbuf, 10);
		beforeReadyStatus[i] = (strcmp(tmpbuf, "") == 0) ? false : true;
	}
	
	// Host Nickname
	GetDlgItemTextA(hDlg, IDC_EDITNICKNAME, tmpbuf, NICKBUFSIZE);
	send(cl_sock, tmpbuf, NICKBUFSIZE, 0);

	while (1) {
		// 재분배
		for (int i{}; i < 3; ++i) {
			// i번 플레이어의 닉네임 정보가 바뀌었단 사실을 클라이언트에게 전송
			GetDlgItemTextA(hDlg, IDC_P1NAME + i, tmpbuf, NICKBUFSIZE);
			_itoa(i, tmpstr, 10);
			retval = send(cl_sock, (char*)"NN", 3, 0);
			retval = send(cl_sock, tmpstr, 2, 0);
			retval = send(cl_sock, tmpbuf, NICKBUFSIZE, 0);

			// i번 플레이어의 레디 정보가 바뀌었단 사실을 클라이언트에게 전송
			GetDlgItemTextA(hDlg, IDC_P1READY + i, tmpbuf, 10);
			bool dlgReady = (strcmp(tmpbuf, "") == 0) ? false : true;
			if (beforeReadyStatus[i] != dlgReady) {
				beforeReadyStatus[i] = !beforeReadyStatus[i];
				retval = send(cl_sock, (char*)"RD", 3, 0);
				retval = send(cl_sock, tmpstr, 2, 0);
			}
		}

		GetDlgItemTextA(hDlg, IDC_HOSTREADY, tmpbuf, NICKBUFSIZE);
		if (strcmp(tmpbuf, "Starting...") == 0) {
			retval = send(cl_sock, (char*)"ST", 3, 0);
			GetDlgItemTextA(hDlg, IDC_P1READY + cl_num, tmpbuf, NICKBUFSIZE);
			SetDlgItemTextA(hDlg, IDC_P1READY + cl_num, (strcmp(tmpbuf, "") == 0) ? "" : "Starting...");
		}

		// 내가 시작 중(Starting)인 맨 위의 플레이어일 때, 시작(Start)처리
		bool checkWrongAccess = false;
		for (int i{}; i < cl_num; ++i) {
			GetDlgItemTextA(hDlg, IDC_P1NAME + i, tmpbuf, NICKBUFSIZE);
			if (strcmp(tmpbuf, "") != 0) {
				GetDlgItemTextA(hDlg, IDC_P1READY + i, tmpbuf, NICKBUFSIZE);
				if (strcmp(tmpbuf, "Starting...") == 0) {
					checkWrongAccess = true;
					break;
				}
			}
		}
		GetDlgItemTextA(hDlg, IDC_P1READY + cl_num, tmpbuf, NICKBUFSIZE);
		if (!checkWrongAccess && strcmp(tmpbuf, "Starting...") == 0) {
			// 시작 처리
			SetDlgItemTextA(hDlg, IDC_P1READY + cl_num, "Start!");
		}

		// 모든 플레이어가 시작 상태일 때 Dlg를 끄고 인게임으로
		bool goStart = false;
		for (int i{}; i < 3; ++i) {
			GetDlgItemTextA(hDlg, IDC_P1NAME + i, tmpbuf, NICKBUFSIZE);
			if (strcmp(tmpbuf, "") != 0) {
				GetDlgItemTextA(hDlg, IDC_P1READY + i, tmpbuf, NICKBUFSIZE);
				if (strcmp(tmpbuf, "Start!") == 0) {
					goStart = true;
				}
				else {
					goStart = false;
					break;
				}
			}
		}
		if (goStart) {
			EndDialog(hDlg, 0);
			HANDLE hnd = CreateThread(NULL, 0, inGameServerThread, (LPVOID)&wr_server, 0, NULL);
			CloseHandle(hnd);
		}

		if (retval == SOCKET_ERROR) {
			break;
		}
		Sleep(333);
	}

	return 0;
}

DWORD WINAPI inGameServerThread(LPVOID arg)
{
	INGAME ig_server = *(INGAME*)arg;
	SOCKET cl_sock = ig_server.GetMySock();
	char recvcode[30];
	int retval;
	//gameFrame.m_curStage->m_player->GetPlayerPt();
	while (1) {
		retval = recv(cl_sock, recvcode, 3, MSG_WAITALL);

		// 경우에 따라 상호작용 종료
		if (ig_server.stringAnalysis(recvcode) == -1 || retval <= 0)
			break;
	}

	return 0;
}

DWORD WINAPI inGameClientThread(LPVOID arg)
{

	return 0;
}

WAITING_ROOM::WAITING_ROOM()
{
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

WAITING_ROOM::~WAITING_ROOM()
{
}

WAITING_ROOM::WAITING_ROOM(const WAITING_ROOM& wr)
{
	wsa = wr.wsa;
	my_sock = wr.my_sock;
	serveraddr = wr.serveraddr;

	is_ready = wr.is_ready;
	is_host = wr.is_host;
	my_num = wr.my_num;
	DlgHandle = wr.DlgHandle;
}

int WAITING_ROOM::MAKE_ROOM(char* serverport)
{
	my_num = -2;
	is_host = true;
	int retval = 0;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock == SOCKET_ERROR)
		return -1;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(atoi(serverport));
	retval = bind(my_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		return -1;

	listen(my_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
		return -1;

	HANDLE hnd = CreateThread(NULL, 0, roomServerThread, (LPVOID)this, 0, NULL);
	CloseHandle(hnd);
	return 0;
}

int WAITING_ROOM::CONNECT_ROOM(char* serverip, char* name, char* serverport)
{
	my_num = -2;
	is_host = false;
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (my_sock == INVALID_SOCKET)
		return -2;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, serverip, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(atoi(serverport));
	if (connect(my_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
		return -2;

	send(my_sock, (char*)"NN", 3, 0);
	send(my_sock, name, NICKBUFSIZE, 0);

	char tmpbuf[2];
	recv(my_sock, tmpbuf, 2, MSG_WAITALL);

	if (strcmp(tmpbuf, "X") == 0) {
		return -1;
	}

	HANDLE hnd = CreateThread(NULL, 0, roomClientThread, (LPVOID)this, 0, NULL);
	CloseHandle(hnd);

	return 0;
}

int WAITING_ROOM::FindBlankPlayer()
{
	char tmpname[NICKBUFSIZE];
	for (int i{}; i < 3; ++i) {
		GetDlgItemTextA(DlgHandle, IDC_P1NAME + i, tmpname, 20);
		if (strcmp(tmpname, "") == 0)
			return i;
	}
	return -1;
}

// 기존에 있는 닉네임인지 검사
bool WAITING_ROOM::checkNickReduplication(char* name)
{
	char tmpname[NICKBUFSIZE];
	for (int i{}; i < 4; ++i) {
		GetDlgItemTextA(DlgHandle, IDC_HOSTNAME + i, tmpname, NICKBUFSIZE);
		if (strcmp(name, tmpname) == 0) {
			return true;
		}
	}
	return false;
}

// 정상적인 접속 시도인지 검사
bool WAITING_ROOM::checkJoin(char* name)
{
	bool isAlreadyExist = checkNickReduplication(name);
	if (isAlreadyExist) {
		send(my_sock, "X", 2, 0); // 닉네임 중복이라는 정보 클라이언트 측으로 전송
		return false;
	}

	send(my_sock, "O", 2, 0); // 정상적으로 접속하였다는 정보 클라이언트 측으로 전송
	HANDLE hnd = CreateThread(NULL, 0, roomDataResendThread, (LPVOID)this, 0, NULL);
	CloseHandle(hnd);
	return true;
}

int WAITING_ROOM::stringAnalysis(char* recvdata)
{
	int retval = 0;
	char recvcode[30];
	char tmpstr[2];

	// Host인 경우의 수신정보 처리
	if (is_host) {
		if (strcmp(recvdata, "NN") == 0) { // 닉네임 정보 수신의 경우
			if (recv(my_sock, recvcode, NICKBUFSIZE, MSG_WAITALL) <= 0) 
				return -1; // 접속 종료
			if (checkJoin(recvcode))
				SetDlgItemTextA(DlgHandle, IDC_P1NAME + my_num, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME
			else 
				return -1; // 닉네임 중복
		}
		else if (strcmp(recvdata, "RD") == 0) { // Ready 정보 수신의 경우
			GetDlgItemTextA(DlgHandle, IDC_P1READY + my_num, recvcode, 10);
			SetDlgItemTextA(DlgHandle, IDC_P1READY + my_num, (strcmp(recvcode, "") == 0) ? "Ready!" : "");
		}
	}
	// Client인 경우의 수신정보 처리
	else {
		if (strcmp(recvdata, "NN") == 0) {
			if (recv(my_sock, recvcode, 2, MSG_WAITALL) <= 0)
				return -1;
			int editnum = atoi(recvcode);
			if (recv(my_sock, recvcode, NICKBUFSIZE, MSG_WAITALL) <= 0)
				return -1;
			SetDlgItemTextA(DlgHandle, IDC_P1NAME + editnum, recvcode); // IDC_P1NAME + n = IDC_P(n+1)NAME
		}
		else if (strcmp(recvdata, "RD") == 0) { // Ready 정보 수신의 경우
			if (recv(my_sock, recvcode, 2, MSG_WAITALL) <= 0)
				return -1;
			int editnum = atoi(recvcode);
			char tmpstr[10];
			GetDlgItemTextA(DlgHandle, IDC_P1READY + editnum, tmpstr, 10);
			SetDlgItemTextA(DlgHandle, IDC_P1READY + editnum, (strcmp(tmpstr, "") == 0) ? "Ready!" : "");
		}
		else if (strcmp(recvdata, "ST") == 0) {
			pressStart();
			EndDialog(DlgHandle, 0);
			HANDLE hnd = CreateThread(NULL, 0, inGameClientThread, (LPVOID)this, 0, NULL);
			CloseHandle(hnd);
		}
	}

	return 0;
}

void WAITING_ROOM::pressReady()
{
	if (!is_host){
		is_ready = !is_ready;
		send(my_sock, "RD", 3, 0);
	}
}

bool WAITING_ROOM::checkAllReady()
{
	char tmpstr[10];
	for (int i{}; i < 3; ++i) {
		GetDlgItemTextA(DlgHandle, IDC_P1NAME + i, tmpstr, 10);
		if (strcmp(tmpstr, "") != 0) {
			GetDlgItemTextA(DlgHandle, IDC_P1READY + i, tmpstr, 10);
			if (strcmp(tmpstr, "") == 0)
				return false;
		}
	}
	return true;
}

void WAITING_ROOM::pressStart()
{
	if (checkAllReady()){
		char tmpstr[30];

		SetDlgItemTextA(DlgHandle, IDC_HOSTREADY, "Starting...");
		if (!is_host) {
			for (int i{}; i < 3; ++i) {
				GetDlgItemTextA(DlgHandle, IDC_P1READY + i, tmpstr, 10);
				SetDlgItemTextA(DlgHandle, IDC_P1READY + i, (strcmp(tmpstr, "") == 0) ? "" : "Starting...");
			}
		}
	}
}

void WAITING_ROOM::enableConnectGui(bool isEnable)
{
	HWND HwndEditNickname = GetDlgItem(DlgHandle, IDC_EDITNICKNAME);
	HWND HwndIpaddress = GetDlgItem(DlgHandle, IDC_IPADDRESS);
	HWND HwndServerPort = GetDlgItem(DlgHandle, IDC_EDIT_SERVERPORT);
	HWND HwndMakeroom = GetDlgItem(DlgHandle, IDC_MAKEROOM);
	HWND HwndConnectroom = GetDlgItem(DlgHandle, IDC_CONNECTROOM);
	
	EnableWindow(HwndMakeroom, isEnable);
	EnableWindow(HwndIpaddress, isEnable);
	EnableWindow(HwndServerPort, isEnable);
	EnableWindow(HwndEditNickname, isEnable);
	EnableWindow(HwndConnectroom, isEnable);
}

void WAITING_ROOM::printErrorEditbox(char* errstr)
{
	char warnbuf[40];
	strcpy(warnbuf, errstr);
	SetDlgItemTextA(DlgHandle, IDC_EDITNICKNAME, warnbuf);
}

int WAITING_ROOM::GetMyNum()
{
	return my_num;
}

void WAITING_ROOM::SetMyNum(int in)
{
	my_num = in;
}

SOCKET WAITING_ROOM::GetMySock()
{
	return my_sock;
}

void WAITING_ROOM::SetMySock(SOCKET in)
{
	my_sock = in;
}

HWND WAITING_ROOM::GetDlgHandle()
{
	return DlgHandle;
}

void WAITING_ROOM::SetDlgHandle(HWND in)
{
	DlgHandle = in;
}

bool WAITING_ROOM::GetIsHost()
{
	return is_host;
	
}

INGAME::INGAME()
{
}

INGAME::~INGAME()
{
}

int INGAME::GetMyNum()
{
	return my_num;
}

SOCKET INGAME::GetMySock()
{
	return my_sock;
}

int INGAME::stringAnalysis(char* recvdata)
{
	int retval = 0;
	char recvcode[30];
	char tmpstr[2];

	// Host인 경우의 수신정보 처리
	if (is_host) {
		if (strcmp(recvdata, "CO") == 0) { // 좌표 정보 수신의 경우
			if (recv(my_sock, recvcode, 5, MSG_WAITALL) <= 0)
				return -1;
			int xcoord = atoi(recvcode);

			if (recv(my_sock, recvcode, 5, MSG_WAITALL) <= 0)
				return -1;
			int ycoord = atoi(recvcode);
		}
	}
	// Client인 경우의 수신정보 처리
	else {
		if (strcmp(recvdata, "CO") == 0) {
			if (recv(my_sock, recvcode, 2, MSG_WAITALL) <= 0)
				return -1;
			int editnum = atoi(recvcode);

			if (recv(my_sock, recvcode, 5, MSG_WAITALL) <= 0)
				return -1;
			int xcoord = atoi(recvcode);

			if (recv(my_sock, recvcode, 5, MSG_WAITALL) <= 0)
				return -1;
			int ycoord = atoi(recvcode);
		}
	}

	return 0;
}
