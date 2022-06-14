/*
Copyright 2021 Krystian Szpakowski

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "OpenGL_API.hh"
#include <iostream>
#include <string>
#include <unistd.h>

using drawNS::APIopenGL3D;

std::mutex APIopenGL3D::mt;
bool APIopenGL3D::control_bit = false;
double APIopenGL3D::limits[] = {0};
std::vector<drawNS::Point3D> APIopenGL3D::vertexes;
std::vector<drawNS::data> APIopenGL3D::shapes;
std::vector<drawNS::Point3D> APIopenGL3D::vertex_buffer;
std::vector<drawNS::data> APIopenGL3D::shape_buffer;
pid_t APIopenGL3D::thread_pid;
int APIopenGL3D::ref_rate = 0;
const float l_width = 3.5; //define line width for all shapes
int APIopenGL3D::mouseoldx = 0;
int APIopenGL3D::mouseoldy = 0;
double APIopenGL3D::rotationXangle = -60;
double APIopenGL3D::rotationZangle = -25;
int APIopenGL3D::state_of_mouse = 0;
bool APIopenGL3D::if_time_gone = true;
bool APIopenGL3D::if_draw = false;
int APIopenGL3D::vec_id_for_color_change = -1;
std::string APIopenGL3D::new_color;


APIopenGL3D::APIopenGL3D(double minX, double maxX, double minY, double maxY, double minZ, double maxZ,int ref_time_ms,int *argc,char **argv) {
    limits[0]=minX;
    limits[1]=maxX;
    limits[2]=minY;
    limits[3]=maxY;
    limits[4]=minZ;
    limits[5]=maxZ;
    ref_rate = ref_time_ms;
    std::thread to_drawing(Initialize,argc,argv);
    to_drawing.detach();
}


uint APIopenGL3D::draw_line(const Point3D &point1, const Point3D &point2, const std::string &color) {
    mt.lock();
    drawNS::data temp;
    if (shapes.empty()) {
        temp.shape_id = 0 +shape_buffer.size();
        temp.shape_type = 1; //draw_line
        temp.shape_size = 2;
        temp.color = color;
    } else {
        temp.shape_id = (shapes.end() - 1)->shape_id + 1 + shape_buffer.size();
        temp.shape_type = 1; //draw_line
        temp.shape_size = 2;
        temp.color = color;
    }
    control_bit = true;
    if (ref_rate == 0) {
        vertexes.push_back(scale(point1));
        vertexes.push_back(scale(point2));
        shapes.push_back(temp);
    } else {
        vertex_buffer.push_back(scale(point1));
        vertex_buffer.push_back(scale(point2));
        shape_buffer.push_back(temp);
    }
    mt.unlock();
    return temp.shape_id;
}


uint APIopenGL3D::draw_polygonal_chain(const std::vector<Point3D> &points, const std::string &color) {
    mt.lock();
    drawNS::data temp;
    if (shapes.empty()) {
        temp.shape_id = 0 + shape_buffer.size();
        temp.shape_type = 2; //draw_polygonal_chain
        temp.shape_size = points.size();
        temp.color = color;
    } else {
        temp.shape_id = (shapes.end()-1)->shape_id+1+shape_buffer.size();
        temp.shape_type = 2; //draw_polygonal_chain
        temp.shape_size = points.size();
        temp.color = color;
    }
    control_bit = true;
    if (ref_rate == 0) {
        for (const auto &i : points) {
            vertexes.push_back(scale(i));
        }
        shapes.push_back(temp);
    } else {
        for (const auto &i : points) {
            vertex_buffer.push_back(scale(i));
        }
        shape_buffer.push_back(temp);
    }
    mt.unlock();
    return temp.shape_id;
}


uint APIopenGL3D::draw_polyhedron(const std::vector<std::vector<Point3D>> &points_map, const std::string &color) {
    mt.lock();
    if (points_map[0].size() == points_map[1].size()) {
        drawNS::data temp;
        if (shapes.empty()) {
            temp.shape_id = 0 + shape_buffer.size();
            temp.shape_type = 3; //draw_polyhedron
            temp.shape_size = 2 * points_map[0].size();
            temp.color = color;
        } else {
            temp.shape_id = (shapes.end()-1)->shape_id+1+shape_buffer.size();
            temp.shape_type = 3; //draw_polyhedron
            temp.shape_size = 2 * points_map[0].size();
            temp.color = color;
        }
        control_bit = true;
        if (ref_rate == 0) {
            for (const auto &i : points_map[0]) {
                vertexes.push_back(scale(i));
            }
            for (const auto &i : points_map[1]) {
                vertexes.push_back(scale(i));
            }
            shapes.push_back(temp);
        } else {
            for (const auto &i : points_map[0]) {
                vertex_buffer.push_back(scale(i));
            }
            for (const auto &i : points_map[1]) {
                vertex_buffer.push_back(scale(i));
            }
            shape_buffer.push_back(temp);
        }
        mt.unlock();
        return temp.shape_id;
    } else {
        std::cerr << "It's not polyhedron!\n";
        mt.unlock();
        return 0;
    }
}


uint APIopenGL3D::draw_surface(const std::vector<std::vector<Point3D>> &points_map, const std::string &color) {
    mt.lock();
    bool if_surface = true; //only checks if number of points for each line is equal, checking whether points are on
    uint line_size = points_map[0].size(); //the same plane is left for user
    for (uint i=1; i<points_map.size(); i++) {
        if (points_map[i].size() != line_size) {
            if_surface = false;
            break;
        }
    }
    if (if_surface) {
        drawNS::data temp;
        if (shapes.empty()) {
            temp.shape_id = 0 + shape_buffer.size();
            temp.shape_type = 4; //draw_surface
            temp.shape_size = points_map.size()*line_size;
            temp.line_size = line_size;
            temp.color = color;
        } else {
            temp.shape_id = (shapes.end()-1)->shape_id+1+shape_buffer.size();
            temp.shape_type = 4; //draw_surface
            temp.shape_size = points_map.size()*line_size;
            temp.line_size = line_size;
            temp.color = color;
        }
        control_bit = true;
        if (ref_rate == 0) {
            for (const auto &row : points_map) {
                for (const auto &element : row) {
                    vertexes.push_back(scale(element));
                }
            }
            shapes.push_back(temp);
        } else {
            for (const auto &row : points_map) {
                for (const auto &element : row) {
                    vertex_buffer.push_back(scale(element));
                }
            }
            shape_buffer.push_back(temp);
        }
        mt.unlock();
        return temp.shape_id;
    } else {
        std::cerr << "It's not surface!\n";
        mt.unlock();
        return 0;
    }
}


void APIopenGL3D::erase_shape(uint id) {
    mt.lock();
    uint vec_index = 0;
    bool if_found = true;
    while (shapes[vec_index].shape_id != id){
        if (vec_index == shapes.size()) {
            std::cerr << "No shape with such id: " << id << std::endl;
            if_found = false;
            break;
        }
        vec_index++;
    }
    if (if_found) {
        vertexes.erase(vertexes.begin()+sum_points_before(vec_index),vertexes.begin()+sum_points_before(vec_index)+shapes[vec_index].shape_size);
        shapes.erase(shapes.begin()+vec_index);
        control_bit = true;
    }
    mt.unlock();
}


void APIopenGL3D::change_shape_color(uint id, const std::string &color) {
    mt.lock();
    uint vec_index = 0;
    bool if_found = true;
    while (shapes[vec_index].shape_id != id){
        if (vec_index == shapes.size()) {
            std::cerr << "No shape with such id: " << id << std::endl;
            if_found = false;
            break;
        }
        vec_index++;
    }
    if (if_found) {
        if (ref_rate != 0) {
            vec_id_for_color_change = int(vec_index);
            new_color = color;
        } else {
            shapes[vec_index].color = color;
        }
        control_bit = true;
    }
    mt.unlock();
}


void APIopenGL3D::change_ref_time_ms(int ref_time_ms) {
    ref_rate = ref_time_ms;
}



void APIopenGL3D::redraw() {
    mt.lock();
    if (vec_id_for_color_change != -1) {
        load_color();
        vec_id_for_color_change = -1;
    }
    load_from_buffer();
    glutPostRedisplay();
    mt.unlock();
}


void APIopenGL3D::draw_line_void(uint offset) {
    glLineWidth(l_width);
    glBegin(GL_LINES);
    glVertex3d(vertexes[0+offset][0],vertexes[0+offset][1],vertexes[0+offset][2]);
    glVertex3d(vertexes[1+offset][0],vertexes[1+offset][1],vertexes[1+offset][2]);
    glEnd();
}


void APIopenGL3D::draw_polygonal_chain_void(uint offset, uint size) {
    glLineWidth(l_width);
    for (uint i=0; i<size-1; i++){
        glBegin(GL_LINES);
        glVertex3d(vertexes[0+offset+i][0],vertexes[0+offset+i][1],vertexes[0+offset+i][2]);
        glVertex3d(vertexes[1+offset+i][0],vertexes[1+offset+i][1],vertexes[1+offset+i][2]);
        glEnd();
    }
}


void APIopenGL3D::draw_polyhedron_void(uint offset, uint size) {
    glLineWidth(l_width);
    for (uint i=0; i<size/2-1; i++){ //draw first base
        glBegin(GL_LINES);
        glVertex3d(vertexes[0+offset+i][0],vertexes[0+offset+i][1],vertexes[0+offset+i][2]);
        glVertex3d(vertexes[1+offset+i][0],vertexes[1+offset+i][1],vertexes[1+offset+i][2]);
        glEnd();
    }
    glBegin(GL_LINES); //drawing last line to spare user giving one more point to draw closed shape
    glVertex3d(vertexes[0+offset][0],vertexes[0+offset][1],vertexes[0+offset][2]);
    glVertex3d(vertexes[1+offset+(size/2-2)][0],vertexes[1+offset+(size/2-2)][1],vertexes[1+offset+(size/2-2)][2]);
    glEnd();
    for (uint i=size/2; i<size-1; i++){  //draw second base
        glBegin(GL_LINES);
        glVertex3d(vertexes[0+offset+i][0],vertexes[0+offset+i][1],vertexes[0+offset+i][2]);
        glVertex3d(vertexes[1+offset+i][0],vertexes[1+offset+i][1],vertexes[1+offset+i][2]);
        glEnd();
    }
    glBegin(GL_LINES); //drawing last line to spare user giving one more point to draw closed shape
    glVertex3d(vertexes[0+offset+(size/2)][0],vertexes[0+offset+(size/2)][1],vertexes[0+offset+(size/2)][2]);
    glVertex3d(vertexes[1+offset+(size-2)][0],vertexes[1+offset+(size-2)][1],vertexes[1+offset+(size-2)][2]);
    glEnd();
    for (uint i=0; i<size/2; i++){  // connect bases
        glBegin(GL_LINES);
        glVertex3d(vertexes[0+offset+i][0],vertexes[0+offset+i][1],vertexes[0+offset+i][2]);
        glVertex3d(vertexes[1+offset+i+(size/2-1)][0],vertexes[1+offset+i+(size/2-1)][1],vertexes[1+offset+i+(size/2-1)][2]);
        glEnd();
    }
}


void APIopenGL3D::draw_surface_void(uint offset, uint size, uint line_size){
    glLineWidth(l_width);
    for (uint i=0; i<size; i+=line_size) {
        for (uint j = 0; j < (line_size - 1); j++) {
            glBegin(GL_LINES);
            glVertex3d(vertexes[0+offset+i+j][0], vertexes[0+offset+i+j][1], vertexes[0+offset+i+j][2]);
            glVertex3d(vertexes[1+offset+i+j][0], vertexes[1+offset+i+j][1], vertexes[1+offset+i+j][2]);
            glEnd();
            if (i != size-line_size) {
                glBegin(GL_LINES);
                glVertex3d(vertexes[0+offset+i+j][0], vertexes[0+offset+i+j][1], vertexes[0+offset+i+j][2]);
                glVertex3d(vertexes[line_size+offset+i+j][0], vertexes[line_size+offset+i+j][1], vertexes[line_size+offset+i+j][2]);
                glEnd();
            }
        }
        if (i != size-line_size) {
            glBegin(GL_LINES);
            glVertex3d(vertexes[line_size - 1 + offset + i][0], vertexes[line_size - 1 + offset + i][1],
                       vertexes[line_size - 1 + offset + i][2]);
            glVertex3d(vertexes[2 * line_size - 1 + offset + i][0], vertexes[2 * line_size - 1 + offset + i][1],
                       vertexes[2 * line_size - 1 + offset + i][2]);
            glEnd();
        }
    }
}


void APIopenGL3D::draw_all_shapes() {
    if (!shapes.empty()) {
        for (uint i = 0; i < shapes.size(); i++) {
            uint offset = sum_points_before(i); //summing how many vertexes was before to set correct offset for deleting
            switch (shapes[i].shape_type){      //and drawing shapes
                case 1:
                    set_color(shapes[i].color);
                    draw_line_void(offset);
                    break;
                case 2:
                    set_color(shapes[i].color);
                    draw_polygonal_chain_void(offset,shapes[i].shape_size);
                    break;
                case 3:
                    set_color(shapes[i].color);
                    draw_polyhedron_void(offset,shapes[i].shape_size);
                    break;
                case 4:
                    set_color(shapes[i].color);
                    draw_surface_void(offset,shapes[i].shape_size,shapes[i].line_size);
                    break;
            }
        }
    }
}


void APIopenGL3D::Initialize(int *argc,char **argv) {
    thread_pid = getpid();
    glutInit(argc, argv);
    glutInitWindowPosition(250, 100);
    glutInitWindowSize(800, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL_API");
    glClearColor(1, 1, 1, 1);
    glColor3d(0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(Draw_scene);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouse);
    glutMotionFunc(mouse_drag);
    glutIdleFunc(Draw_scene_with_delay);
    glutMainLoop();
}


void APIopenGL3D::Draw_scene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotated(rotationXangle, 1, 0, 0);
    glRotated(rotationZangle,0,0,1);
    mt.lock();
    Draw_axes();
    draw_all_shapes();
    mt.unlock();
    glutSwapBuffers();
}


void APIopenGL3D::Draw_scene_with_delay() {
    if (ref_rate == 0) {
        if (control_bit) {
            mt.lock();
            glutPostRedisplay();
            control_bit = false;
            mt.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } else if (ref_rate > 0) {
        if (if_time_gone && control_bit) {
            mt.lock();
            control_bit = false;
            if_time_gone = false;
            mt.unlock();
            std::thread waiting(wait,ref_rate);
            waiting.detach();
        }
        if (if_draw) {
            mt.lock();
            if (vec_id_for_color_change != -1) {
                load_color();
                vec_id_for_color_change = -1;
            }
            load_from_buffer();
            glutPostRedisplay();
            if_draw = false;
            mt.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    } else if (ref_rate == -1){
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}


void APIopenGL3D::Draw_axes() {
    glColor3d(0,0,0);
    glLineWidth(GLdouble(2.5));
    glBegin(GL_LINES); // Z axis
    glVertex3d(-0.5,-0.5,-0.5);    //                +y|     /-z
    glVertex3d(-0.5,-0.5,0.5);     //                  |   /
    glEnd();                       //                  | /
    glBegin(GL_LINE_LOOP);         //      -x-----------------------x+
    glVertex3d(-0.5,-0.5,-0.5);    //                / |
    glVertex3d(0.5,-0.5,-0.5);     //              /   |
    glVertex3d(0.5,0.5,-0.5);      //          +z/   -y|
    glVertex3d(-0.5,0.5,-0.5);     //
    glEnd(); //draws floor (X and Y axes)

    double offsetX = (0.1/1)*(fabs(limits[0])+fabs(limits[1])); //for drawing scale on axis
    double offsetY = (0.1/1)*(fabs(limits[2])+fabs(limits[3]));
    double offsetZ = (0.1/1)*(fabs(limits[4])+fabs(limits[5]));
    double i; //for for loops
    double c;

    for (i=0.1,c=limits[4]+offsetZ; i<=1;){ //rendering scale on Z axis
        glBegin(GL_LINES);
        glVertex3d(-0.49,-0.5,-0.5+i);
        glVertex3d(-0.51,-0.5,-0.5+i);
        glEnd();
        renderString(-0.48,-0.45,-0.5+i,c); //rendering scale on axis
        i += 0.2;
        if (limits[4]*limits[5]>0)
            c = c+(limits[5]-limits[4]-2*offsetZ)/4;
        else
            c = c+(fabs(limits[4])+fabs(limits[5])-2*offsetZ)/4;
    }
    //draw infill of XY plane
    glPushAttrib(GL_ENABLE_BIT); //setting dashed line
    glLineStipple(2, 0x0F0F);
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(GLdouble(1));
    for (i=0.1,c=limits[0]+offsetX; i<=1;){ //rendering scale on X axis
        glBegin(GL_LINES);
        glVertex3d(-0.5+i,0.5,-0.5);
        glVertex3d(-0.5+i,-0.5,-0.5);
        glEnd();
        renderString(-0.48+i,-0.6,-0.5,c);
        i += 0.2;
        if (limits[0]*limits[1]>0)
            c = c+(limits[1]-limits[0]-2*offsetX)/4;
        else
            c = c+(fabs(limits[0])+fabs(limits[1])-2*offsetX)/4;
    }
    for (i=0.1,c=limits[2]+offsetY; i<=1;){ //rendering scale on Y axis
        glBegin(GL_LINES);
        glVertex3d(-0.5,-0.5+i,-0.5);
        glVertex3d(0.5,-0.5+i,-0.5);
        glEnd();
        renderString(0.55,-0.5+i,-0.5,c);
        i += 0.2;
        if (limits[3]*limits[2]>0)
            c = c+(limits[3]-limits[2]-2*offsetY)/4;
        else
            c = c+(fabs(limits[2])+fabs(limits[3])-2*offsetY)/4;
    }
    glDisable(GL_LINE_STIPPLE);
}


void APIopenGL3D::stop_drawing() {
    std::stringstream killcommand;
    killcommand << "kill " << thread_pid;
    system(killcommand.str().c_str());
}


void APIopenGL3D::load_from_buffer() {
    for (const auto &i : vertex_buffer){
        vertexes.push_back(i);
    }
    for (const auto &j : shape_buffer){
        shapes.push_back(j);
    }
    vertex_buffer.clear();
    shape_buffer.clear();
}


void APIopenGL3D::load_color() {
    shapes[vec_id_for_color_change].color = new_color;
}


void APIopenGL3D::wait(int time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    if_time_gone = true;
    if_draw = true;
}


uint APIopenGL3D::sum_points_before(uint shape_id) {
    uint temp = 0;
    for (uint i=0; i<shape_id; i++){
        temp += shapes[i].shape_size;
    }
    return temp;
}


void APIopenGL3D::set_color(const std::string &color) {
    if (color == "black")
        glColor3d(0, 0, 0);
    else if (color == "white")
        glColor3d(1,1,1);
    else if (color == "grey")
        glColor3d(0.6,0.6,0.6);
    else if (color == "blue")
        glColor3d(0,0,0.7);
    else if (color == "orange")
        glColor3d(1,0.75,0.1);
    else if (color == "yellow")
        glColor3d(1,1,0);
    else if (color == "purple")
        glColor3d(0.8,0,0.8);
    else if (color == "green")
        glColor3d(0,0.7,0);
    else if (color == "light-blue")
        glColor3d(0.5,1,1);
    else if (color == "red")
        glColor3d(1,0,0);
    else {
        std::cerr << "Color not recognized!\n";
        glColor3d(0,0,0);
    }
}


drawNS::Point3D APIopenGL3D::scale(const drawNS::Point3D &point) {
    double x_val,y_val,z_val; //some boring math here
    if (limits[0]*limits[1] < 0) { //begin X
        point[0]<0 ? x_val = -0.5-((limits[0]/(fabs(limits[0])+limits[1]))-((point[0]/limits[0]))*(limits[0]/(fabs(limits[0])+limits[1]))) :
        point[0]==0 ? x_val = -0.5-(limits[0]/(fabs(limits[0])+limits[1])) :
        x_val = (0.5-limits[1]/(fabs(limits[0])+limits[1]))+((point[0]/limits[1]))*(limits[1]/(fabs(limits[0])+limits[1]));
    } else {
        limits[1]>0 ? point[0]>0 ? x_val = -0.5 + point[0]/(limits[1]-limits[0]) - (limits[0]/(limits[1]-limits[0])) :
        point[0]==0 ? x_val = -0.5 - (limits[0]/(limits[1]-limits[0])): x_val = -0.5 - fabs(point[0]/(limits[1]-limits[0])) - (limits[0]/(limits[1]-limits[0])): //end of limits[1]>0
        point[0]>0 ? x_val = 0.5 + fabs(point[0]/(limits[0]-limits[1])) + (limits[1]/(limits[0]-limits[1])) :
        point[0]==0 ? x_val = 0.5 + (limits[1]/(limits[0]-limits[1])): x_val = 0.5 - point[0]/(limits[0]-limits[1]) + (limits[1]/(limits[0]-limits[1]));
    } //end X
    if (limits[2]*limits[3] < 0) { //begin Y
        point[1]<0 ? y_val = -0.5-((limits[2]/(fabs(limits[2])+limits[3]))-((point[1]/limits[2]))*(limits[2]/(fabs(limits[2])+limits[3]))) :
        point[1]==0 ? y_val = -0.5-(limits[2]/(fabs(limits[2])+limits[3])) :
        y_val = (0.5-limits[3]/(fabs(limits[2])+limits[3]))+((point[1]/limits[3]))*(limits[3]/(fabs(limits[2])+limits[3]));
    } else {
        limits[3]>0 ? point[1]>0 ? y_val = -0.5 + point[1]/(limits[3]-limits[2]) - (limits[2]/(limits[3]-limits[2])) :
        point[1]==0 ? y_val = -0.5 - (limits[2]/(limits[3]-limits[2])): y_val = -0.5 - fabs(point[1]/(limits[3]-limits[2])) - (limits[2]/(limits[3]-limits[2])): //end of limits[3]>0
        point[1]>0 ? y_val = 0.5 + fabs(point[1]/(limits[2]-limits[3])) + (limits[3]/(limits[2]-limits[3])) :
        point[1]==0 ? y_val = 0.5 + (limits[3]/(limits[2]-limits[3])): y_val = 0.5 - point[1]/(limits[2]-limits[3]) + (limits[3]/(limits[2]-limits[3]));
    } //end Y
    if (limits[4]*limits[5] < 0) { //begin Z
        point[2]<0 ? z_val= -0.5-((limits[4]/(fabs(limits[4])+limits[5]))-((point[2]/limits[4]))*(limits[4]/(fabs(limits[4])+limits[5]))) :
        point[2]==0 ? z_val = -0.5-(limits[4]/(fabs(limits[4])+limits[5])):
        z_val = (0.5-limits[5]/(fabs(limits[4])+limits[5]))+((point[2]/limits[5]))*(limits[5]/(fabs(limits[4])+limits[5]));
    } else {
        limits[5]>0 ? point[2]>0 ? z_val = -0.5 + point[2]/(limits[5]-limits[4]) - (limits[4]/(limits[5]-limits[4])) :
        point[2]==0 ? z_val = -0.5 - (limits[4]/(limits[5]-limits[4])): z_val = -0.5 - fabs(point[2]/(limits[5]-limits[4])) - (limits[4]/(limits[5]-limits[4])): //end of limits[5]>0
        point[2]>0 ? z_val = 0.5 + fabs(point[2]/(limits[4]-limits[5])) + (limits[5]/(limits[4]-limits[5])) :
        point[2]==0 ? z_val = 0.5 + (limits[5]/(limits[4]-limits[5])): z_val = 0.5 - point[2]/(limits[4]-limits[5]) + (limits[5]/(limits[4]-limits[5]));
    } //end Z
    drawNS::Point3D result(x_val,y_val,z_val);
    return result;
}


void APIopenGL3D::renderString(double x, double y, double z, double var) {
    glRasterPos3d(x,y,z);
    std::stringstream ss;
    std::string str;
    ulong counter;
    var = var-(var*10-int(var*10))/10; //rounding var to 1st place after comma
    ss << var;
    ss >> str;
    char *c = &str[0];
    for (counter = 0; counter < str.size(); counter++, c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
    }
}


void APIopenGL3D::changeSize(int w, int h) {
    if (h==0)
        h = 1;
    if (w>h) {
        glViewport((w-h)/2,0,h,h);
    } else if (h>w) {
        glViewport(0,(h-w)/2,w,w);
    } else {
        glViewport(0,0,w,h);
    }
}


void APIopenGL3D::mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            //doing nothing here
        } else if (state == GLUT_DOWN) {
            mouseoldx = x;
            mouseoldy = y;
            state_of_mouse = GLUT_LEFT_BUTTON;
        }
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        state_of_mouse = GLUT_RIGHT_BUTTON;
        rotationZangle = -25;
        rotationXangle = -60;
        glutPostRedisplay();
    }
}


void APIopenGL3D::mouse_drag(int x, int y) {
    if (state_of_mouse == GLUT_LEFT_BUTTON) {
        mt.lock();
        int xloc = x - mouseoldx;
        rotationZangle += 0.3 * xloc;
        rotationZangle>360.0 ? rotationZangle -= 360.0 : rotationZangle<-360.0 ? rotationZangle += 360.0 : rotationZangle;
        mouseoldx = x;
        int yloc = y - mouseoldy;
        rotationXangle += 0.3 * yloc;
        rotationXangle>360.0 ? rotationXangle -= 360.0 : rotationXangle<-360.0 ? rotationXangle += 360.0 : rotationXangle;
        mouseoldy = y;
        mt.unlock();
        glutPostRedisplay();
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(3)); //might saves CPU
}
