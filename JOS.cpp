#include "JOS.h"
#include "WindowManager.h"
#include "Desktop.h"

Texell **g_backBuffer = NULL;
WindowManager windowManager;		// singleton 패턴 적용 요망
JOS_SYSTEM JOS;				// singleton 패턴 적용 요망, 운영체제 관리자 역활을 함

int main( int argc, char * argv[] )
{
	JOS.SetResolution( SIZEX, SIZEY );


	if( argc > 2 )
	{
		int size1 = strlen( argv[1] );
		int size2 = strlen( argv[2] );
		//해상도를 설정한 채 실행
		if( size1 == 3 && size2 == 2 )
		{
			int h = (argv[1][0]-'0')*100 +  (argv[1][1]-'0')*10 + (argv[1][2]-'0');
			int w = 			(argv[2][0]-'0')*10 + (argv[2][1]-'0');
			JOS.SetResolution( h, w );
		}
	}
	else if( argc > 1 )
	{
		//테스트 모드로 실행
		if( strcmp(argv[1],"test") == 0 )
			JOS.mode = 1;	
	}
	
	bool _mainLoop = true;
	
	Box tempBox(12,3,true,1,1);
	tempBox.SetText("");

	//System Init
	JOS.Init();
	windowManager.Init();
	windowManager.AddWindow( new Desktop());
	
	system("clear");

	//Main Loop
	while( _mainLoop )
	{	
		//flush backbuffer
		//memset(g_backBuffer,0,sizeof(g_backBuffer));
		JOS.SetTextBgColor(30,47);
		++JOS.frame;

		//do something in this frame
		windowManager.Update();
		while(Kbhit() == true)
		{
			char in = getchar();
			if( in  == 'q' )
				_mainLoop = false;
		}
		//render
		windowManager.Render();

		for( int i = 0 ; i < JOS.H ; ++i)
		{
			JOS.SetDefaultColor();
			for(int j = 0 ; j < JOS.W ; ++j)
				g_backBuffer[i][j].Print();
		
			printf("\033[%d;%dm\n", 0, 0);
		}
		
		if( JOS.mode == 1 )
		{
			for( int i = 0 ; i < JOS.W ; ++i)
				std::cout<<'-';
			cout<<endl<<"frame:"<<JOS.frame<<"  "<<"RunTime: 00:00:00"<<endl;
			cout<<"ErrorMessage:"<<endl;
			for( auto it = JOS.errorlist.begin() ; it != JOS.errorlist.end() ; ++it )
			{
				string s = *it;
				cout<<s.c_str()<<", ";
			}
			JOS.errorlist.clear();
		}

		if( _mainLoop == false )
			break;		
		//waiting
		sleep(1);
		system("clear");
		
	}
	printf("\033[%d;%dm", 0, 0);
	
	cout<<"JOS IS END"<<endl;
	return 0;
}
// 화면에 특정 문자를 특정 위치에 출력, 조만간 텍셀이라는 구조체를 만들 예정
void Draw(int _x, int _y, const char * _c)
{
	if( !SetValidPos(&_x,&_y) )
		return ;
	g_backBuffer[_y][_x].SetText(_c);	
//        strcpy(g_backBuffer[_y][_x],_c);
//       g_backBuffer[_y][_x][3] = '\0';
}
void Draw(int _x, int _y, char * _c)
{
	if( !SetValidPos(&_x,&_y) )
		return ;
	g_backBuffer[_y][_x].SetText(_c);	
//      strcpy(g_backBuffer[_y][_x],_c);
//      g_backBuffer[_y][_x][3] = '\0';
}
void Draw(int _x, int _y, char _c)
{
	if( !SetValidPos(&_x,&_y) )
		return ;
	g_backBuffer[_y][_x].SetText(_c);	
//        char temp[4];
//        temp[0] = _c;
//        strcpy(g_backBuffer[_y][_x],temp);
//        g_backBuffer[_y][_x][1] = '\0';
}
// 키보드 버퍼가 비어있지 않으면 1을 반환
int Kbhit()
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
//화면 밖을 벗어나는 좌표가 들어오면 값을 화면 안으로 수정하고 거짓을 반환
bool SetValidPos(int * _x, int * _y)
{
	int x = *_x;
	int y = *_y;
	if( *_x < 0 )
		*_x = 0;
	if( *_x > JOS.W -1 )
		*_x = JOS.W -1;
	if( *_y < 0 )
		*_y = 0;
	if( *_y > JOS.H -1 )
		*_y = JOS.H -1;
	
	return (x==*_x)&&(y==*_y);
}
