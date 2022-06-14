/*
Copyright 2020 Arkadiusz Mielczarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Dr3D_gnuplot_api.hh"
#include <iostream>
#include <sstream>
#include <unistd.h>

using drawNS::Point3D;
using drawNS::APIGnuPlot3D;
using std::string;
using std::stringstream;
using std::endl;
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::thread;
using std::unique_ptr;
using std::mutex;

std::ostream & operator<<(std::ostream & strm, const Point3D & point) {
  strm << point[0] << " " << point[1] << " " << point[2] << std::endl;
  return strm;
}

APIGnuPlot3D::APIGnuPlot3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, int ref_time_ms): Draw3DAPI(ref_time_ms), refresh_thread_ptr(nullptr), last_shape(0) {
  pipe(pipe_fd);
  int local_pid=fork();
  if(local_pid != 0) {
    gnuplot_pid=local_pid;
    stringstream ss_tmp;

    ss_tmp << " set terminal qt" << endl
           << "set xrange [" << minX << ":" << maxX << "]" << endl
           << "set yrange [" << minY << ":" << maxY << "]" << endl
           << "set zrange [" << minZ << ":" << maxZ << "]" << endl
           << "set xlabel \"X\"" << endl
           << "set ylabel \"Y\"" << endl
           << "set zlabel \"Z\"" << endl
           << "set view equal xyz" << endl
           << "set xyplane relative 0" << endl
           << "unset key" << endl;

    intro = ss_tmp.str();
    send2gnuplot(intro);
    if (ref_time_ms > 0) {
      refresh_thread_ptr.reset(new thread(&APIGnuPlot3D::replot_loop,this));
      refresh_thread_ptr->detach();
    }
  }
  else {
    dup2(pipe_fd[0],0);
    std::cerr << "gnuplot is running" << endl;
    execlp("gnuplot","gnuplot",NULL);
  }
}

void APIGnuPlot3D::send2gnuplot(const string & command_arg){
  gnuplot_pipe_mutex.lock();
  write(pipe_fd[1],command_arg.c_str(),command_arg.length());
  gnuplot_pipe_mutex.unlock();
}

void APIGnuPlot3D::replot_loop(){
  while(refresh_rate_ms > 0) {
    draw_all_shapes();
    std::this_thread::sleep_for(std::chrono::milliseconds(refresh_rate_ms));
  }
}

uint APIGnuPlot3D::add2shape_list(const list<list<Point3D> > & _list, const string & color) {
  shapes.insert(std::pair<uint,shape_and_color>(last_shape,std::make_pair(_list,color)));
  return last_shape++;
}

void APIGnuPlot3D::draw_all_shapes() {
  stringstream ss;
  string color;
  ss << intro << "splot ";
  for (auto itel = shapes.begin();itel!=shapes.end();) {
    color = translate_color(itel->second.second);
    if (!color.compare("error")) {
      color = translate_color("black");
      std::cerr << "Shape nr " << itel->first << " has invalid color.\nUsing black color to draw it." << endl;
    } 
    ss << "\'-\' using 1:2:3 with lines lt 1 lw 3 " << color;
    if (++itel != shapes.end())
      ss << ",";
  }
  ss << endl;
  for (auto & el : shapes) {
    for (auto & inel : el.second.first) {
      for (auto & in2el : inel) {
	ss << in2el;
      }
      ss << endl;
    }
    ss<< "e" << endl;	
  }
  ss << "pause -1" << endl;
  command = ss.str();
  send2gnuplot(command);
}

string APIGnuPlot3D::translate_color(const string & color) const {
  string out = "lc rgb \'#";
  if (!color.compare("black"))
    return out+"000000\'";
  if (!color.compare("white"))
    return out+"ffffff\'";
  if (!color.compare("grey"))
    return out+"808080\'";
  if (!color.compare("blue"))
    return out+"0072bd\'";
  if (!color.compare("orange"))
    return out+"d95319\'";
  if (!color.compare("yellow"))
    return out+"edb120\'";
  if (!color.compare("purple"))
    return out+"7e2f8e\'";
  if (!color.compare("green"))
    return out+"77ac30\'";
  if (!color.compare("light-blue"))
    return out+"4dbeee\'";
  if (!color.compare("red"))
    return out+"a2142f\'";
  if (!color.compare(0,5,"rgb:#"))
    return out+color.substr(5);
  return "error";
}

uint APIGnuPlot3D::draw_line(const Point3D & point1, const Point3D & point2, const string & color ) {
  uint tmp = add2shape_list(list<list<Point3D> > {{point1,point2}},color);
  if (refresh_rate_ms == 0) {
    draw_all_shapes();
  }
  return tmp;
}

uint APIGnuPlot3D::draw_polygonal_chain(const vector<Point3D> & points, const string & color ) {
  uint tmp = add2shape_list(list<list<Point3D> > {list<Point3D>(begin(points),end(points))},color);
  if (refresh_rate_ms == 0) {
    draw_all_shapes();
  }
  return tmp;
}

uint APIGnuPlot3D::draw_polyhedron(const vector<vector<Point3D> > & points_map, const string & color ) {
  vector<vector<Point3D> > tmp;
  vector<Point3D> ltmp;
  for (auto & el : points_map) {
    ltmp = el;
    ltmp.push_back(el[0]);
    tmp.push_back(ltmp);
  }
  return draw_surface(tmp,color);  
}

uint APIGnuPlot3D::draw_surface(const vector<vector<Point3D> > & points_map, const string & color ) {
  list<list<Point3D> > ltmp;
  list<Point3D> iltmp;
  for (auto & el : points_map) {
    iltmp = list<Point3D> (begin(el),end(el));
    ltmp.push_back(iltmp);
  }
  uint tmp = add2shape_list(ltmp,color);
  if (refresh_rate_ms == 0) {
    draw_all_shapes();
  }
  return tmp;  
}

void APIGnuPlot3D::erase_shape(uint id) {
  shapes.erase(id);
  if (refresh_rate_ms == 0) {
    draw_all_shapes();
  }
}

void APIGnuPlot3D::change_shape_color(uint id, const string & color) {
  if (shapes.count(id)) {
    shapes[id].second=color;
  }
  if (refresh_rate_ms == 0) {
    draw_all_shapes();
  }  
}

void APIGnuPlot3D::stop_drawing_proces() {
  stringstream killstream;
  std::cerr << "gnuplot stops running" << std::endl;
  killstream << "kill " << gnuplot_pid;
  system(killstream.str().c_str());
}

void APIGnuPlot3D::change_ref_time_ms(int ref_time_ms) {
  if (ref_time_ms <= 0) {
    refresh_thread_ptr.reset();
  } else {
    refresh_thread_ptr.reset(new thread(&APIGnuPlot3D::replot_loop,this));
    refresh_thread_ptr->detach();
  }
  refresh_rate_ms = ref_time_ms;
}

void APIGnuPlot3D::redraw() {
    if (refresh_rate_ms < 0)
     draw_all_shapes();
}
