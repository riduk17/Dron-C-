/*
Copyright 2020 Arkadiusz Mielczarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DR3D_GPLT_API_HH
#define DR3D_GPLT_API_HH

#include "Draw3D_api_interface.hh"
#include <list>
#include <map>
#include <utility>
#include <memory>
#include <mutex>
#include <thread>

using shape_and_color = std::pair<std::list<std::list<drawNS::Point3D> >,std::string>;

namespace drawNS {

   /*!
   * \brief Implementation drawing 3D class using gnuplot
   * Class implements proxy between gnuplot and C++ code.
   *
   * Colors are pre-defined or must be on form rgb:#rrggbb.\n
   * Pre defined colors: black, white, grey, blue, light-blue, red, green, yellow, orange, purple, as it was in default palette in Matlab 2014b.\n
   *
   * Class have 3 modes, described in refresh_rate_ms variable.
   */ 
  
class APIGnuPlot3D : public drawNS::Draw3DAPI {
  /*!
   * \brief file descriptors for unnamed pipe between app and gnuplot
   */
  int pipe_fd[2];
    /*!
   * \brief pointer to thread responible for refreshing
   */
  std::unique_ptr<std::thread> refresh_thread_ptr;
  /*!
   * \brief mutex responsible for correct usage of pipe
   */
  std::mutex gnuplot_pipe_mutex;
    /*!
   * \brief proces id of gnuplot
   */
  int gnuplot_pid;
  /*!
   * \brief string contains proper initialization of gnuplot.
   */
  std::string intro;
  /*!
   * \brief string send to gnuplot
   * Contains initialization and data
   */
  std::string command;
  /*!
   * \brief variable used to give indiviual id to each shape
   */
  uint last_shape;
  /*!
   * \brief container for shapes to draw
   */
  std::map<uint,shape_and_color> shapes;
  void send2gnuplot(const std::string & command_arg);
  void replot_loop();
  uint add2shape_list(const std::list<std::list<Point3D> > & _list, const std::string & color);
  void draw_all_shapes();
  std::string translate_color(const std::string & color) const;
  void stop_drawing_proces();
public:
  /*!
   * \brief deleted non-parametric constructor to force using parametric.
   */ 
  APIGnuPlot3D() = delete;
  /*!
   * \brief Constructor
   * \param minX - lower bounry in X axis,
   * \param maxX - upper bounry in X axis,
   * \param minY - lower bounry in Y axis,
   * \param maxY - upper bounry in Y axis,
   * \param minZ - lower bounry in Z axis,
   * \param maxZ - upper bounry in Z axis,
   * \param ref_time_ms - mode for plotting (negative, 0, positive, see refresh_rate_ms) and time between auto-replots (in miliseconds).
   */
  APIGnuPlot3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ, int ref_time_ms = 0);
    /*!
   * \brief destructor, also stops gnuplot proces.
   */ 
  ~APIGnuPlot3D() {stop_drawing_proces();}
  uint draw_line(const Point3D & point1, const Point3D & point2, const std::string & color = "black") override;
  uint draw_polygonal_chain(const std::vector<Point3D> & points, const std::string & color = "black") override;
  uint draw_polyhedron(const std::vector<std::vector<Point3D> > & points_map, const std::string & color = "black") override;
  uint draw_surface(const std::vector<std::vector<Point3D> > & points_map, const std::string & color = "black") override;
  void erase_shape(uint id) override;
  void change_shape_color(uint id, const std::string & color) override;
  void change_ref_time_ms(int ref_time_ms) override;
  void redraw() override;
};

}
  
#endif
