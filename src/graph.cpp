#include "graph.h"

Graph::Graph(string graphname, int window_width, int window_height,
   double left, double right, double bottom, double top) :
sf::RenderWindow(sf::VideoMode(window_width, window_height), graphname)
{
   this->setActive(true);   

   red = 255;
   blue = 0;
   green = 0;

   points = 200;

   xZero = (2./(right-left))*(-left) - 1.;
   yZero = (2./(top-bottom))*(-bottom) -1.;

   xScale = 2./(right-left);
   yScale = 2./(top-bottom);

   rr = right;
   ll = left;
   tt = top;
   bb = bottom;

   // create axes
   xAxis.resize(6);
   yAxis.resize(6);

   xAxis[0] = float(convertX(ll - 1.)); xAxis[1] = float(convertY(0.)); xAxis[2] = AXIS_DEPTH;
   xAxis[3] = float(convertX(rr + 1.)); xAxis[4] = float(convertY(0.)); xAxis[5] = AXIS_DEPTH;

   yAxis[0] = float(convertX(0.f)); yAxis[1] = float(convertY(bb-1.)); yAxis[2] = AXIS_DEPTH;
   yAxis[3] = float(convertX(0.f)); yAxis[4] = float(convertY(tt+1.)); yAxis[5] = AXIS_DEPTH;

   // init GL
   glShadeModel(GL_SMOOTH);
   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_LIGHTING);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_CULL_FACE);

   this->setActive(false);
}

/*
void Graph::operator= (const Graph & arg)
{
   xRange = arg.xRange;
   yRange = arg.yRange;
   xScale = arg.xScale;
   yScale = arg.yScale;
   xZero = arg.xZero;
   yZero = arg.yZero;
   rr = arg.rr;
   ll = arg.ll;
   tt = arg.tt;
   bb = arg.bb;
   points = arg.points;
   verts = arg.verts;
   xAxis = arg.xAxis;
   yAxis = arg.yAxis;
   colors = arg.colors;
   red = arg.red;
   green = arg.green;
   blue = arg.blue;
} */

Graph::~Graph()
{

}

void Graph::update()
{
   this->setActive(true);

   // handle events
   sf::Event event;

   while(this->pollEvent(event))
   {
      if(event.type == sf::Event::Closed){
         this->close();
      }
   }

   // draw
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

   // draw axes
   glColor3f(255.f, 255.f, 255.f);
   // x
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &xAxis[0]);

   glPushMatrix();

   glDrawArrays(GL_LINES, 0, GLsizei(xAxis.size()/3));
   
   glPopMatrix();

   glDisableClientState(GL_VERTEX_ARRAY);
   // y
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, GL_FLOAT, 0, &yAxis[0]);

   glPushMatrix();

   glDrawArrays(GL_LINES, 0, GLsizei(yAxis.size()/3));

   glPopMatrix();

   glDisableClientState(GL_VERTEX_ARRAY);

   // draw graphs
   for(unsigned int i=0;i<verts.size();++i)
   {  
      //glColor3fv( &colors[i][0] );
      glColor3f(colors[i][0], colors[i][1], colors[i][2]);

      glEnableClientState(GL_VERTEX_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, &verts[i][0]);

      glPushMatrix();

      glDrawArrays(GL_LINE_STRIP, 0, GLsizei(verts[i].size()/3)); 

      glPopMatrix();

      glDisableClientState(GL_VERTEX_ARRAY);
   }

   this->display();

   this->setActive(false);
}

void Graph::save(string fname)
{
	sf::Image screenshot = this->capture();

	screenshot.saveToFile(fname.c_str());
}

void Graph::setColor(unsigned short r, unsigned short g, unsigned short b)
{
   red = r;
   green = g;
   blue = b;
}

unsigned int Graph::addplot(double (*func)(double) )
{	
   verts.resize(verts.size()+1);
   verts[verts.size()-1].resize(points*3);

   colors.resize(colors.size()+1);
   colors[colors.size()-1].resize(3);
   colors[colors.size()-1][0] = red;
   colors[colors.size()-1][1] = green;
   colors[colors.size()-1][2] = blue;
   

	double gap = (rr-ll)/double(points-1);

	for(unsigned int i=0;i<points;++i)
	{
      verts[verts.size()-1][i*3] = float(convertX(double(i)*gap+ll));
      verts[verts.size()-1][i*3+1] = float(convertY(func(double(i)*gap+ll)));
      verts[verts.size()-1][i*3+2] = 0.f;
   }

   return unsigned(verts.size()-1);
}

double Graph::convertX(double x)
{
	return x*xScale+xZero;
}

double Graph::convertY(double y)
{
	return y*yScale+yZero;
}