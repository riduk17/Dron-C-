/*
Copyright 2021 Krystian Szpakowski

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef OPENGL_API_OPENGL_API_HH
#define OPENGL_API_OPENGL_API_HH
#include <GL/glut.h>
#include <GL/glu.h>
#include "Draw3D_api_interface.hh"
#include <cmath>
#include <array>
#include <vector>
#include <sstream>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <mutex>


namespace drawNS {


    struct data { //used to store shapes data
        uint shape_id;
        uint shape_type; //1-draw_line, 2-polygonal_chain, 3-draw_polyhedron, 4-draw_surface
        uint shape_size; //how many points does one shape contain
        uint line_size {}; //used for drawing surface
        std::string color = "black";
    };


    class APIopenGL3D : public drawNS::Draw3DAPI {
        /*
         * responsible for correct data transfer
         */
        static std::mutex mt;
        /*
         * \brief used to redraw once with ref_rate == 0
         * Method Draw_scene_with_delay() would redraw once then reset control bit
         */
        static bool control_bit;
        /*
         *  \brief limits of X, Y, Z axis
         */
        static double limits[6];
        /*
         *  \brief stores vertexes
         */
        static std::vector<Point3D> vertexes;
        /*
         *  \brief stores data about each shape
         */
        static std::vector<data> shapes;
        /*
         * \brief used when ref_rate != 0 to load image in correct moment
         */
        static std::vector<Point3D> vertex_buffer;
        /*
         * \brief used when ref_rate != 0 to load image in correct moment
         */
        static std::vector<data> shape_buffer;
        /*
         * \brief stores thread PID
         * Used in class destructor to close process in class destructor, therefore destructor must be called in main
         */
        static pid_t thread_pid;
        /*
         * \brief holds value in ms
         */
        static int ref_rate;
        /*
         * \brief holds old X coordinate of mouse position
         */
        static int mouseoldx;
        /*
         * \brief holds old Y coordinate of mouse position
         */
        static int mouseoldy;
        /*
         * \brief holds angle of rotation by X axis
         */
        static double rotationXangle;
        /*
         * \brief holds angle of rotation by Z axis
         */
        static double rotationZangle;
        /*
         * \brief holds value to allow only left-click rotation
         */
        static int state_of_mouse;
        /*
         * \brief checks is ref_rate is gone, when yes it draws
         * Used to allow smooth rotation even if ref_rate is high
         */
        static bool if_time_gone;
        /*
         * \brief is time is gone, sets it for true to draw
         */
        static bool if_draw;
        /*
         * \brief stores id for load_color method
         */
        static int vec_id_for_color_change;
        /*
         * \brief stores color for load_color method
         */
        static std::string new_color;
    public:
        /*
         *  \brief holds max values of axes
         *  needed for static func
         */
        APIopenGL3D() = delete;
        /*
         * \brief creates scene
         */
        APIopenGL3D(double minX,double maxX,double minY,double maxY,double minZ,double maxZ,int ref_time_ms,int *argc,char **argv);
        ~APIopenGL3D() override {stop_drawing();};

        /*
         *  TO ALL DRAWING METHODS RETURNING VALUES
         *  They are interface: store vertexes in std::vector, add new shapes to shapes vector
         *  They return value of shape id
         */
        uint draw_line(const Point3D & point1, const Point3D & point2, const std::string & color) override;
        uint draw_polygonal_chain(const std::vector<Point3D> & points, const std::string & color) override;
        uint draw_polyhedron(const std::vector<std::vector<Point3D> > & points_map, const std::string & color) override;
        uint draw_surface(const std::vector<std::vector<Point3D> > & points_map, const std::string & color) override;
        void erase_shape(uint id) override;
        void change_shape_color(uint id, const std::string & color) override;
        void change_ref_time_ms(int ref_time_ms) override;
        void redraw() override;
        /*
         * TO ALL STATIC VOID DRAWING METHODS
         *  They are responsible for drawing, data for them are prepared by their equivalent uint methods
         *  They are used in draw_all_shapes() method
         */
    private:
        static void draw_line_void(uint offset);
        static void draw_polygonal_chain_void(uint offset, uint size);
        static void draw_polyhedron_void(uint offset, uint size);
        static void draw_surface_void(uint offset, uint size, uint line_size);
        /*
         * \brief draws all shapes
         * This method is included in Draw_scene()
         */
        static void draw_all_shapes();
        /*
         * \brief set window size, starting position, bg color
         */
        static void Initialize(int *argc,char **argv);
        /*
         *  \brief draws scene
         */
        static void Draw_scene();
        /*
         * \brief draws with delay given during initialization
         */
        static void Draw_scene_with_delay();
        /*
         * \brief draws axes
         */
        static void Draw_axes();
        /*
         *  \brief kill drawing process
         */
        static void stop_drawing();
        /*
         * \brief loads vertexes and shapes from buffer
         * Used when ref_rate != 0
         */
        static void load_from_buffer();
        /*
         * \brief loads color, when needed
         * Used when ref_rate != 0 -> when dragging mouse color won't be overwritten
         */
        static void load_color();
        /*
         * \brief after time is gone sets bool variables
         * Called by thread
         */
        static void wait(int time);
        /*
         * \brief summing how many points where before shape with id = shape_id
         * used for:
         * 1. setting offset which is used to delete vertexes while deleting shapes
         * 2. to let program know from which vertexes program shall draw, when there is more than one shape
         */
        static uint sum_points_before(uint shape_id);
        /*
         * \brief sets color for drawing next shape
         */
        static void set_color(const std::string &color);
        /*
         * \brief scale coordinates to values on which program operates (1 unit for each direction: i. e. on X axis
         * values are from -1 to +1)
         */
        static drawNS::Point3D scale (const drawNS::Point3D &point);
        /*
         * \brief takes double and displays it at given 3d vector, displays to 1st place after comma
         */
        static void renderString(double x, double y, double z, double var);
        /*
         * \brief responsible for scaling scene when windows is resized
         * It makes coordinate system staying always in center of window and prevent deforming scales by
         * changing ratio of windows' height and width for other than 1
         */
        static void changeSize(int w, int h);
        /*
         * \brief reacts when mouse is clicked
         */
        static void mouse(int button, int state, int x, int y);
        /*
         * \brief rotate coordinate system by mouse dragging
         * Uses "mouse" method, left-click + drag allows to rotate coordinate system by X and Z axes
         * right-click reset the rotation, especially handy when ref_rate is high value
         */
        static void mouse_drag(int x, int y);
    }; //end of class


} //end of namespace

#endif //OPENGL_API_OPENGL_API_HH
