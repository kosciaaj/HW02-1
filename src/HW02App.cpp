#include "Lists.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/app/AppNative.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Font.h"
#include "Resources.h"




using namespace ci;
using namespace ci::app;
using namespace std;

class HW02App : public AppBasic {
  public:
    Lists* circList;
	gl::Texture	mTextTexture, mSimpleTexture;

	void setup();
	void mouseDown( MouseEvent event );	
	void mouseDrag(MouseEvent event);
	void update();
	void draw();
	void prepareSettings(Settings* settings);
	void HW02App::keyPress( KeyEvent event );

	
	private:
		Surface* my_Surface;
};

	static const int kAppWidth = 800;
	static const int kAppHeight = 600;
	static const int textureSize = 1024;
	static const bool PREMULT = false;
	int clicks, iterations;
	list<Vec2f>		mPoints;
	bool help=false;;

	void HW02App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);

	}

void HW02App::setup()


{
	my_Surface = new Surface(kAppWidth, kAppHeight, false);
    circList = new Lists;
    circList->circ_sentinel = new Circle;
    circList->circ_sentinel->next = circList->circ_sentinel;
    circList->circ_sentinel->previous = circList->circ_sentinel;
	circList->circ_sentinel->radius = kAppWidth/6;
	circList->circ_sentinel->color=Color(255,0,0);
	circList->circ_sentinel->pos_X=200;
	circList->circ_sentinel->pos_X=400;
    Circle* next;
	next = circList->insertAfter(circList->circ_sentinel, kAppWidth/8, kAppHeight/4);
	for(int i=0; i<6; i++) {
	 next= circList->insertAfter(next, i*kAppWidth/2, kAppHeight/2);
	}
	


}


void HW02App::mouseDown( MouseEvent event )
{
	clicks++;



}
void HW02App::mouseDrag( MouseEvent event )
{
	// add wherever the user drags to the end of our list of points
	mPoints.push_back( event.getPos() );
}

void HW02App::keyPress( KeyEvent event )
{
	char ch =( event.getChar() == 0 ) ? ' ' : event.getChar();

	if(ch=='h') {
	//help!=help;
	//if(help==true) {
		Vec2f mSize=Vec2f( 100, 100 );
		string txt = "Here is some text that is larger than can fit naturally inside of 100 pixels.\nAnd here is another line after a hard break.";
		TextBox tbox = TextBox().alignment( TextBox::CENTER ).font(Font( "Times New Roman", 32 )).size( Vec2i( mSize.x, TextBox::GROW ) ).text( txt );
		tbox.setColor( Color( 1.0f, 0.65f, 0.35f ) );
		tbox.setBackgroundColor( ColorA( 0.5, 0, 0, 1 ) );
		Vec2i sz = tbox.measure();
		console() << "Height: " << sz.y << endl;
		mTextTexture = gl::Texture( tbox.render() );
	}
}



void changeColor(int r, int g, int b, int index, uint8_t* pixelData) {
		 if(index>=0 && index <kAppHeight*kAppWidth*3){
			pixelData[index]=r;
			pixelData[index+1]=g;
			pixelData[index+2]=b;
		}
	}
int getIndex(int x, int y) {
		return 3*(y*kAppWidth+x);
	}


void drawCircle(int x, int y, int r, uint8_t* pixelData, int red, int green, int blue){
    int tempX, tempY;
    double angle = 0;
    
    int i;
    
    while(angle < 6.28){
        tempX = x+r*sin(angle+3.14/2);
        tempY = y+r*sin(angle);
        i = getIndex(tempX, tempY);    
        changeColor(red,green,blue,i, pixelData);
        angle+=.01;
    }
}

void display(Lists* circList, uint8_t* data){
    Circle* current = circList->circ_sentinel->next;
    while(current!= circList->circ_sentinel){
		gl::drawSolidCircle(Vec2f(current->pos_X, current->pos_Y), current->radius,0);
    }    
}

void HW02App::update()
{
	
	uint8_t* pixelData = (*my_Surface).getData();
	display(circList, pixelData);
	if(clicks==0) {
	Font customFont( Font( "Arial", 42 ) );
	TextLayout simple;
	simple.setFont( customFont );
	simple.setColor( Color( 1, 0, 0.1f ) );
	simple.addLine( "Connect the dots" );
	mSimpleTexture = gl::Texture( simple.render( true, PREMULT ) );
	}

	
		
	
	}

void HW02App::draw()
{

	gl::draw(*my_Surface);
	gl::draw( mSimpleTexture, Vec2f( 10, getWindowHeight() - mSimpleTexture.getHeight() - 5 ) );
	gl::setMatricesWindow( getWindowSize() );
	Circle* current = circList->circ_sentinel->next;
    while(current!= circList->circ_sentinel){
		gl::drawSolidCircle(Vec2f(current->pos_X, current->pos_Y), current->radius,0);
    }  
	gl::drawSolidCircle(Vec2f(20.0f, 300.0f), 50.0f);
	gl::drawSolidCircle(Vec2f(30.0f, 300.0f), 60.0f);
	gl::drawSolidCircle(Vec2f(400.0f, 300.0f), 70.0f);
	gl::drawSolidCircle(Vec2f(500.0f, 300.0f), 80.0f);

	if( mTextTexture )
		gl::draw( mTextTexture );
	

	// We'll set the color to an orange color
	glColor3f( .5f, 0.5f, 0.25f );
	
	// now tell OpenGL we've got a series of points it should draw lines between
	glBegin( GL_LINE_STRIP );
	// iterate across our list of points, and pass each one to OpenGL
	for( list<Vec2f>::iterator pointIter = mPoints.begin(); pointIter != mPoints.end(); ++pointIter ) {
		glVertex2f( *pointIter );
	}
	// tell OpenGL to actually draw the lines now
	glEnd();

}



CINDER_APP_BASIC( HW02App, RendererGl )


