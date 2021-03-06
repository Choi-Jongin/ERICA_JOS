#include "JOS.h"
#include "Box.h"
#include "Texell.h"

Box::Box(int _w, int _h, bool _sh, int _wali, int _hali)
	:Object("Box")
{
	this->width = _w;
	this->height = _h;
	showFrame = _sh;
	maxTextLen = _w;
	walign = _wali;
	halign = _hali;
	fill = false;
}
Box::~Box()
{

}
void Box::Update()
{

}
void Box::Render(bool isColored, char _textcolor, char _backcolor)
{
	if( showFrame == true )
	{
		for( int i = pos.x ; i < pos.x + width ; ++i)
		{
			Draw(i,pos.y,"─");
			Draw(i,pos.y + height - 1,"─");
		 }

		for( int i = pos.y ; i < pos.y + height -1; ++i)
		{
			Draw(pos.x,i,"│");
			Draw(pos.x + width -1,i,"│");
		}
		Draw(pos.x, pos.y,"┌");
		Draw(pos.x + width-1, pos.y,"┐");
		Draw(pos.x,pos.y + height-1,"└");
		Draw(pos.x + width-1, pos.y + height-1,"┘");
	}

	//배경 칠하기
	if(fill)
	{
		for( int i = 0; i < height-2 ; ++i )
		{
			for( int j = 0; j < width-2 ; ++j )
			{
				if( isColored )
					Draw(pos.x+1 + j, pos.y+1 + i, Texell("", _textcolor, _backcolor) );			
				else
					Draw(pos.x+1 + j, pos.y+1 + i, "" );			

			}
		}
	}

	if( text.size() > maxTextLen)
		text.erase(maxTextLen,text.size()-text.size());
	
	for( int i = 0; i < height-2 ; ++i )
	{
		int k = 0;		//가로 정렬용 변수
	      	if( walign == 0) k = 0;
		else if( walign == 1) k = (width-text.size()%width)/2;
		else if( walign == 2) k = width - text.size()%width-2;

		
		for( int j = 0; j < width && i<=line ; ++j )
		{
			if( i*width + j >= text.size())
				break;
			if( isColored == true )
				Draw(pos.x+1 + j + k*(line==i), pos.y+1 + i, Texell(text[i*width+j], _textcolor, _backcolor) );			
			else
				Draw(pos.x+1 + j + k*(line==i), pos.y+1 + i, text[i*width + j]);
		}
	}

}
