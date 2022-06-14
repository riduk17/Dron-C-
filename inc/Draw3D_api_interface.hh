/*
Copyright 2020 Arkadiusz Mielczarek

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DR3D_API_INT_HH
#define DR3D_API_INT_HH

#include <vector>
#include <string>
#include <iostream>

/*!
 * \file Draw3D_api_interface.hh
 * File contains abstract interface for drawing in 3D.
 */

/*!
 * \brief Namespace for drawing tools - abstract interface and 3D point
 * Namespace for drawing tools - abstract interface and 3D point.
 */

namespace drawNS {

  /*!
   * \brief Point in 3D
   * Class represents point in 3D. 
   */
  
  class Point3D {
    /*!
     * \brief x,y,z coord
     * array for x,y,z coordintions.
     */
    double data[3];
  public:
    /*!
     * \brief deleted non-parametric constructor to force using parametric.
     */
    Point3D() = delete;
    /*!
     * \brief Constructor
     * \param x - x coord.
     * \param y - y coord.
     * \param z - z coord.
     */
    Point3D(double x, double y, double z) : data{x,y,z} {}
    /*!
     * \brief Acess operator
     * \param ind - 0 for x coord, 1 for y, 2 for z.
     * \return value of coord.
     */
    const double & operator[](uint ind) const {
      if (ind < 3)
	return data[ind];
      std::cerr << "operator[] dla Point3D poza zakresem" << std::endl;
      exit(1);
    };
    /*!
     * \brief Acess operator
     * \param ind - 0 for x coord, 1 for y, 2 for z.
     * \return reference to  coord.
     */
    double & operator[](uint ind) {
      if (ind < 3)
	return data[ind];
      std::cerr << "operator[] dla Point3D poza zakresem" << std::endl;
      exit(1);
    };      
  };
  
  /*!
   * \brief Abstract interface for drawing class
   * Abstract class used as interface for 3D drawing class implementations.
   *
   * Colors are pre-defined or must be on form rgb:#rrggbb.\n
   * Pre defined colors: black, white, grey, blue, light-blue, red, green, yellow, orange, purple, as it was in default palette in Matlab 2014b.\n
   *
   * Class have 3 modes, described in refresh_rate_ms variable.
   */

  class Draw3DAPI {
  protected:
    /*!
     * \brief mode for ploting and time between auto-replots
     *
     * For negative vaules - auto-replots is off and replot can be done only by function redraw(),
     * for 0 value - auto-replots is off and replot is done after every drawing functions,
     * for positive values - auto-replots is on and value is time (in miliseconds) between replots, 
     * drawing functions and redraw() don't replot in this mode.
     */
  int refresh_rate_ms;    
  public:
    /*!
     * \brief Constructor
     * \param ref_time_ms - mode for plotting (negative, 0, positive, see refresh_rate_ms) and time between auto-replots (in miliseconds).
     */
    Draw3DAPI(int ref_time_ms = 0) : refresh_rate_ms(ref_time_ms) {}
    /*
     * \brief virtual destructor.
     */
    virtual ~Draw3DAPI() {};
    /*!
     * \brief draws line between 2 points
     * \param point1 - first point
     * \param point2 - second point
     * \param color - color of line (see class description)
     * \return id of shape (see erase_shape, change_shape_color)
     */
    virtual uint draw_line(const Point3D & point1, const Point3D & point2, const std::string & color = "black") = 0;
    /*!
     * \brief draws polygonal_chain between n points
     * \param points - vector of points
     * \param color - color of line (see class description)
     * \return id of shape (see erase_shape, change_shape_color)
     */
    virtual uint draw_polygonal_chain(const std::vector<Point3D> & points, const std::string & color = "black") = 0;
    /*!
     * \brief draws polyhedron described by its apexs 
     * \param points_map - vector of points vectors. Every vector contains points describes one "layer" of polyhedron
     * \param color - color of line (see class description)
     * \return id of shape (see erase_shape, change_shape_color)
     */
    virtual uint draw_polyhedron(const std::vector<std::vector<Point3D> > & points_map, const std::string & color = "black") = 0;
    /*!
     * \brief draws surface described by its points 
     * \param points_map - vector of points vectors. Every vector contains points describes one "line" in surface
     * \param color - color of line (see class description)
     * \return id of shape (see erase_shape, change_shape_color)
     */
    virtual uint draw_surface(const std::vector<std::vector<Point3D> > & points_map, const std::string & color = "black") = 0;
    /*!
     * \brief erase shape by id 
     * \param id - id of shape
     */
    virtual void erase_shape(uint id) = 0;
    /*!
     * \brief change color of shape 
     * \param id - id of shape
     * \param color - color of line (see class description)
     */
    virtual void change_shape_color(uint id, const std::string & color) = 0;
    /*!
     * \brief change mode for ploting and/or time between auto-replots
     * \param ref_time_ms - mode for plotting (negative, 0, positive, see refresh_rate_ms) and time between auto-replots (in miliseconds).
     */    
    virtual void change_ref_time_ms(int ref_time_ms) = 0;
    /*!
     * \brief replot all shapes
     * Replots all shapes, works only in negative refresh_rate_ms value mode.
     */    
    virtual void redraw() = 0;
  };

}

#endif
