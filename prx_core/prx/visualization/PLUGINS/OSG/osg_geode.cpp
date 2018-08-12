/**
 * @file osg_geode.cpp
 *
 * @copyright Software License Agreement (BSD License)
 * Copyright (c) 2013, Rutgers the State University of New Jersey, New Brunswick
 * All Rights Reserved.
 * For a full description see the file named LICENSE.
 *
 * Authors: Andrew Dobson, Andrew Kimmel, Athanasios Krontiris, Zakary Littlefield, Kostas Bekris
 *
 * Email: pracsys@googlegroups.com
 */


#include "prx/visualization/PLUGINS/OSG/osg_geode.hpp"
#include "prx/visualization/PLUGINS/OSG/osg_helpers.hpp"

namespace prx
{
    using namespace util;
    namespace vis
    {


std::vector<vector_t> floatsToPoints(const std::vector<double>* params)
{
    PRX_ASSERT(params->size() % 3 == 0);
    std::vector<vector_t> points(params->size() / 3);

    std::vector<double>::const_iterator param_itr = params->begin();
    foreach (vector_t& point, points)
    {
        point.resize(3);
        point[0] = *(param_itr++);
        point[1] = *(param_itr++);
        point[2] = *(param_itr++);
    }

    return points;
}

/**
 * Creates a sphere osg::Geometry given the radius.
 *
 * @brief Creates a Sphere.
 *
 * @param radius The radius of the sphere.
 */
void setup_sphere(osg::ref_ptr<osg::Geode> geode, double radius, vector_t color)
{
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0, 0, 0), radius) );
 //   shape->setColor(osg::Vec4d(color[0],color[1],color[2],color[3]));
    if (color[0] > -1)
    {
        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
        osg::StateSet* ss = shape->getOrCreateStateSet();
        ss->setMode( GL_LIGHTING,osg::StateAttribute::ON);
    }
    geode->addDrawable( shape );
}

void setup_ellipsoid(osg::ref_ptr<osg::Geode> geode, const trimesh_t* trimesh, const vector_t& color, bool add_inverted_normal = false)
{
    // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Set Vertices
    const vector_t* vex;
    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;

    int nr_indices = trimesh->get_faces_size();
    vector_t vertex;
    face_t f;
    for( int i=0; i<nr_indices; i ++ )
    {
        trimesh->get_face_at(i,&f);
        vex = trimesh->get_vertex_at(f.get_index1());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index2());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index3());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    }

    geom->setVertexArray( vert );

    if (color[0] > -1)
    {
        osg::Vec4Array* colors = new osg::Vec4Array;
    //        colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));
        colors->push_back(osg::Vec4(color[0],color[1],color[2],color[3]));
        // PRX_PRINT ("SETTING COLOR: " << color, PRX_TEXT_BLUE);
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    }


    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Set to 3-vertex Triangle
    geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, trimesh->get_faces_size()*3) );
    // geom->setColorArray (color.get());
    // geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    osg::StateSet* state = geom->getOrCreateStateSet();
    // state->setMode( GL_LIGHTING,osg::StateAttribute::ON);

    // Add to Geode
    geode->addDrawable( geom );
}




/**
 * Creates a Box osg::Geometry given the length of x, y, and z.
 *
 * @brief Creates a Box.
 *
 * @param lx The x length of the box.
 * @param ly The y length of the box.
 * @param lz The z length of the box.
 */
void setup_box(osg::ref_ptr<osg::Geode> geode, double lx, double ly, double lz, vector_t color)
{
    osg::ref_ptr<osg::Box> box = new osg::Box(osg::Vec3(0, 0, 0), lx, ly, lz);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(box);
    if (color[0] > -1)
    {
//        PRX_ERROR_S (" I has colors! " << color);
        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
        osg::StateSet* ss = shape->getOrCreateStateSet();
        ss->setMode( GL_LIGHTING,osg::StateAttribute::ON);
//        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    }
    geode->addDrawable( shape );
}

/**
 * Creates a Cone osg::Geometry given the radius and the height.
 *
 * @brief Creates a Cone.
 *
 * @param radius The radius of the cone.
 * @param height The height of the cone.
 */
void setup_cone(osg::ref_ptr<osg::Geode> geode, double radius, double height, vector_t color)
{
//    osg::ref_ptr<osg::Cone> cone =  new osg::Cone(osg::Vec3(0, 0, 0), radius, height);
//    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cone);
//    if (color[0] >= 0)
//    {
//        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
//    }
//    geode->addDrawable( shape );

    trimesh_t* trimesh = new trimesh_t();
    trimesh->create_cone_trimesh(radius,height);

        // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Set Vertices
    const vector_t* vex;
    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;

    int nr_indices = trimesh->get_faces_size();
    vector_t vertex;
    face_t f;
    for( int i=0; i<nr_indices; i ++ )
    {
        trimesh->get_face_at(i,&f);
        vex = trimesh->get_vertex_at(f.get_index1());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index2());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index3());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    }
    /*foreach(face_t f, trimesh->get_faces())
    {
        vex = trimesh->get_vertex_at(f.get_index1());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index2());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index3());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    }*/
    geom->setVertexArray( vert );

    // Set Normal
    osg::ref_ptr<osg::Vec3Array> norm = new osg::Vec3Array;
    geom->setNormalArray( norm.get() );
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    norm->push_back(osg::Vec3(0.f, 0.f, 1.f));

    // Set to 3-vertex Triangle
    geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, trimesh->get_faces_size()*3) );

    if (color[0] > -1)
    {
        osg::Vec4Array* colors = new osg::Vec4Array;
    //        colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));
        colors->push_back(osg::Vec4(color[0],color[1],color[2],color[3]));
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    }

    osg::StateSet* state = geom->getOrCreateStateSet();
    state->setMode( GL_LIGHTING,osg::StateAttribute::ON);

    // Add to Geode
    geode->addDrawable( geom );
}

/**
 * Creates a Cylinder osg::Geometry given the radius and the height.
 *
 * @brief Creates a Cylinder.
 *
 * @param radius The radius of the cylinder.
 * @param height The height of the cylinder.
 */
void setup_cylinder(osg::ref_ptr<osg::Geode> geode, double radius, double height, vector_t color)
{
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, 0), radius, height);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
    if (color[0] > -1)
    {
        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
        osg::StateSet* ss = shape->getOrCreateStateSet();
        ss->setMode( GL_LIGHTING,osg::StateAttribute::ON);
    }
    geode->addDrawable( shape );
}


/**
 * Creates a Cylinder osg::Geometry given the radius and the height.
 *
 * @brief Creates a Cylinder.
 *
 * @param radius The radius of the cylinder.
 * @param height The height of the cylinder.
 */
void setup_bullet_cylinder(osg::ref_ptr<osg::Geode> geode, double radius, double height, vector_t color)
{
    osg::ref_ptr<osg::PositionAttitudeTransform> visualization_transform = new osg::PositionAttitudeTransform();
    osg::ref_ptr<osg::Cylinder> cylinder = new osg::Cylinder(osg::Vec3(0, 0, 0), radius, height);
    cylinder->setRotation(osg::Quat(PRX_PI/2, osg::Vec3(1,0,0)));
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(cylinder);
    if (color[0] > -1)
    {
        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
        osg::StateSet* ss = shape->getOrCreateStateSet();
        ss->setMode( GL_LIGHTING,osg::StateAttribute::ON);
    }
    geode->addDrawable( shape );




    // trimesh_t* trimesh = new trimesh_t();
    // trimesh->create_bullet_cylinder_trimesh(radius,height);

    //     // Create a osg::Geometry
    // osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // // Set Vertices
    // const vector_t* vex;
    // osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;

    // int nr_indices = trimesh->get_faces_size();
    // vector_t vertex;
    // face_t f;
    // for( int i=0; i<nr_indices; i ++ )
    // {
    //     trimesh->get_face_at(i,&f);
    //     vex = trimesh->get_vertex_at(f.get_index1());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    //     vex = trimesh->get_vertex_at(f.get_index2());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    //     vex = trimesh->get_vertex_at(f.get_index3());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    // }
    // foreach(face_t f, trimesh->get_faces())
    // {
    //     vex = trimesh->get_vertex_at(f.get_index1());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    //     vex = trimesh->get_vertex_at(f.get_index2());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    //     vex = trimesh->get_vertex_at(f.get_index3());
    //     vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    // }
    // geom->setVertexArray( vert );

    // // Set Normal
    // osg::ref_ptr<osg::Vec3Array> norm = new osg::Vec3Array;
    // geom->setNormalArray( norm.get() );
    // geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    // norm->push_back(osg::Vec3(0.f, 0.f, -1.f));

    // // Set to 3-vertex Triangle
    // geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, trimesh->get_faces_size()*3) );

    // // Add to Geode
    // geode->addDrawable( geom );
    // delete trimesh;
}

/**
 * Creates a capsule osg::Geometry given the radius and the height.
 *
 * @brief Creates a capsule.
 *
 * @param radius The radius of the capsule.
 * @param height The height of the capsule.
 */
void setup_capsule(osg::ref_ptr<osg::Geode> geode, double radius, double height, vector_t color)
{
    osg::ref_ptr<osg::Capsule> capsule = new osg::Capsule(osg::Vec3(0, 0, 0), radius, height);
    osg::ref_ptr<osg::ShapeDrawable> shape = new osg::ShapeDrawable(capsule);
    if (color[0] > -1)
    {
        shape->setColor(osg::Vec4(color[0],color[1],color[2],color[3]));
        osg::StateSet* ss = shape->getOrCreateStateSet();
        ss->setMode( GL_LIGHTING,osg::StateAttribute::ON);
    }
    geode->addDrawable( shape );
}

void setup_trimesh(osg::ref_ptr<osg::Geode> geode, const trimesh_t* trimesh, const vector_t& color, bool add_inverted_normal = false)
{
    // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Set Vertices
    const vector_t* vex;
    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;

    int nr_indices = trimesh->get_faces_size();
    vector_t vertex;
    face_t f;
    for( int i=0; i<nr_indices; i ++ )
    {
        trimesh->get_face_at(i,&f);
        vex = trimesh->get_vertex_at(f.get_index1());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index2());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index3());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    }
    /*foreach(face_t f, trimesh->get_faces())
    {
        vex = trimesh->get_vertex_at(f.get_index1());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index2());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
        vex = trimesh->get_vertex_at(f.get_index3());
        vert->push_back(osg::Vec3((*vex)[0],(*vex)[1],(*vex)[2]));
    }*/
    geom->setVertexArray( vert );

    if (color[0] > -1)
    {
        osg::Vec4Array* colors = new osg::Vec4Array;
    //        colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));
        colors->push_back(osg::Vec4(color[0],color[1],color[2],color[3]));
        // PRX_PRINT ("SETTING COLOR: " << color, PRX_TEXT_BLUE);
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    }


    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // // Set Normal
    // vector_t vertex1,vertex2,vertex3,normal, inverted_normal;
    // normal.resize(3);
    // osg::ref_ptr<osg::Vec3Array> norm = new osg::Vec3Array;
    // geom->setNormalArray( norm.get() );
    // geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // for( int i=0; i<nr_indices; i ++ )
    // {
    //     trimesh->get_face_at(i,&f);
    //     vex = trimesh->get_vertex_at(f.get_index1());
    //     vertex1 = *vex;
    //     vex = trimesh->get_vertex_at(f.get_index2());
    //     vertex2 = *vex;
    //     vex = trimesh->get_vertex_at(f.get_index3());
    //     vertex3 = *vex;
    //     normal.cross_product(vertex2-vertex1,vertex3-vertex1);
    //     normal.normalize();
    //     // norm->push_back(osg::Vec3(normal[0],normal[1],normal[2]));
    //     // norm->push_back(osg::Vec3(normal[0],normal[1],normal[2]));
    //     // norm->push_back(osg::Vec3(normal[0],normal[1],normal[2]));

    //     // if (add_inverted_normal)
    //     // {
    //     //     inverted_normal = normal;
    //     //     inverted_normal *= -1;
    //     //     norm->push_back(osg::Vec3(inverted_normal[0],inverted_normal[1],inverted_normal[2]));
    //     //     norm->push_back(osg::Vec3(inverted_normal[0],inverted_normal[1],inverted_normal[2]));
    //     //     norm->push_back(osg::Vec3(inverted_normal[0],inverted_normal[1],inverted_normal[2]));
    //     // }


    // }


    // norm->push_back(osg::Vec3(0.f, 0.f, 1.f));

    // Set to 3-vertex Triangle
    geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, trimesh->get_faces_size()*3) );
    // geom->setColorArray (color.get());
    // geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    osg::StateSet* state = geom->getOrCreateStateSet();
    if(!add_inverted_normal)
        state->setMode( GL_LIGHTING,osg::StateAttribute::ON);
    else
        state->setMode( GL_LIGHTING,osg::StateAttribute::OFF);

    // Add to Geode
    geode->addDrawable( geom );
}


void setup_triangle(osg::ref_ptr<osg::Geode> geode, boost::array<osg::Vec3, 4> verts)
{
    // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Set Vertices
    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;
    foreach(osg::Vec3 v, verts)
    {
        vert->push_back(v);
    }
    geom->setVertexArray( vert );


    // Set Normal
    osg::ref_ptr<osg::Vec3Array> norm = new osg::Vec3Array;
    geom->setNormalArray( norm.get() );
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    norm->push_back(osg::Vec3(0.f, 0.f, 1.f));

    // Set to 3-vertex Triangle
    geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 3) );

    // Add to Geode
    geode->addDrawable( geom );
}

void setup_quad(osg::ref_ptr<osg::Geode> geode, boost::array<osg::Vec3, 4> verts)
{
    // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    // Set Vertices
    osg::ref_ptr<osg::Vec3Array> vert = new osg::Vec3Array;
    foreach(osg::Vec3 v, verts)
    {
        vert->push_back(v);
    }
    geom->setVertexArray( vert.get() );

    // Set Normal
    osg::ref_ptr<osg::Vec3Array> norm = new osg::Vec3Array;
    geom->setNormalArray( norm.get() );
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    norm->push_back(osg::Vec3(0.f, 0.f, 1.f));

    // Set to 4-vertex Quad
    geom->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4) );

    // Add to Geode
    geode->addDrawable( geom );
}

void setup_lines(osg::ref_ptr<osg::Geode> geode, bool strip, const std::vector<vector_t>* points, vector_t color, double line_thickness)
{
//    PRX_LOG_WARNING("Creating lines");
    // Create a osg::Geometry
    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;

    geom->setUseDisplayList(false);
    // Add points to the line-strip.
    osg::Vec3Array* vertices = new osg::Vec3Array;
    foreach (vector_t point, *points)
    {
//        PRX_WARN_S(point);
        vertices->push_back(toVec3(point));
//        PRX_INFO_S("vertices : " << vertices->back()[0] << "," << vertices->back()[1] << "," << vertices->back()[2]);
    }


    geom->setVertexArray(vertices);
    osg::PrimitiveSet::Mode primitive;
//    PRX_ERROR_S(strip);
    if (strip)
        primitive = osg::PrimitiveSet::LINE_STRIP;
    else
        primitive = osg::PrimitiveSet::LINES;
    geom->addPrimitiveSet(new osg::DrawArrays(primitive, 0, points->size()));

    if (color[0] > -1)
    {
        osg::Vec4Array* colors = new osg::Vec4Array;
    //        colors->push_back(osg::Vec4(1.0,0.0,0.0,1.0));
        colors->push_back(osg::Vec4(color[0],color[1],color[2],color[3]));
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    }


    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    osg::StateSet* stateset = geom->getOrCreateStateSet();
    stateset->setAttributeAndModes(new osg::LineWidth(line_thickness), osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);

    geode->addDrawable( geom );
}

/**
 * Creates a set of points from point cloud
 *
 * @brief Creates a set of points
 *
 * @param point cloud
 */
void setup_pointcloud(osg::ref_ptr<osg::Geode> geode, const std::vector<double> points)
{
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());
    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());

    for (int i=0; i<points.size(); i=i+6) {
        vertices->push_back (osg::Vec3 (points[i], points[i+1], points[i+2]));

        uint32_t red,green,blue;
        red = points[i+3];
        green = points[i+4];
        blue = points[i+5];

       colors->push_back (osg::Vec4f ((float)red/255.0f, (float)green/255.0f, (float)blue/255.0f,1.0f));
     }

     geometry->setVertexArray (vertices.get());
     geometry->setColorArray (colors.get());
     geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
  
     geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,vertices->size()));
  
     geode->addDrawable (geometry.get());
     osg::StateSet* state = geometry->getOrCreateStateSet();
     state->setMode( GL_LIGHTING,osg::StateAttribute::ON);
}

osg::ref_ptr<osg::Node> osg_geode_t::setup_mesh(const std::string& filename, hash_t< std::string, osg::ref_ptr<osg::Node>, string_hash>* mesh_lookup)
{
    PRX_INFO_S("trying to load model: " << filename);

    // Check if model has already been loaded
    if (mesh_lookup->find(filename) == mesh_lookup->end())
    {
        osg::ref_ptr<osg::Node> mesh = osgDB::readNodeFile(filename);

        if (!mesh.valid())
        {
            PRX_WARN_S("Failed to load model: " << filename);
        }

        osg::ref_ptr<osg::MatrixTransform> flip_x = new osg::MatrixTransform(osg::Matrixd(-1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1));
        flip_x->addChild(mesh);

        return flip_x;
    }
    else
    {
        return (*mesh_lookup)[filename];
    }
}

osg::ref_ptr<osg::Node> osg_geode_t::setup_mesh(const std::string& filename)
{
    PRX_INFO_S("trying to load model: " << filename);

    // Check if model has already been loaded

        osg::ref_ptr<osg::Node> mesh = osgDB::readNodeFile(filename);

        if (!mesh.valid())
        {
            PRX_WARN_S("Failed to load model: %s" << filename);
        }

        return mesh;
}

osg::ref_ptr<osg::Geode> osg_geode_t::setup_geometry(const geometry_t* geometry, bool transparent, double line_thickness)
{
    osg::ref_ptr<osg::Geode> geode = new osg::Geode();

    vector_t color = geometry->get_color();

    if (color.get_dim() <=0)
        color.resize(4);

    if (transparent)
    {
        color[0] = color[1] = color[2] = 1.0;
        color[3] = 0.15;
    }
    // PRX_PRINT("GEOMETRY TYPE: " << geometry->get_type(), PRX_TEXT_CYAN);
    switch (geometry->get_type())
    {
        case PRX_NONE:
            PRX_FATAL_S("Can't create geode with geometry type NONE.");
            break;
        case PRX_SPHERE:
        {
            double radius;
            geometry->get_sphere(radius);
            setup_sphere(geode, radius, color);
            break;
        }
        case PRX_ELLIPSOID:
        {
            setup_ellipsoid(geode, geometry->get_trimesh(), color);
            break;
        }
        case PRX_BOX:
        {
            double dx, dy, dz;
            geometry->get_box(dx, dy, dz);
            setup_box(geode, dx, dy, dz, color);
            break;
        }
        case PRX_CONE:
        {
            double radius, height;
            geometry->get_cone(radius, height);
            setup_cone(geode, radius, height, color);
            break;
        }
        case PRX_CYLINDER:
        {
            double radius, height;
            geometry->get_cylinder(radius, height);
            setup_cylinder(geode, radius, height, color);
            break;
        }
        case PRX_OPEN_CYLINDER:
        {
            double radius, height;
            geometry->get_open_cylinder(radius, height);
            setup_trimesh(geode, geometry->get_trimesh(), color);
            break;
        }
        case PRX_CAPSULE:
        {
            double radius, height;
            geometry->get_capsule(radius, height);
            setup_capsule(geode, radius, height, color);
            break;
        }

        case PRX_LINES:
        {
            const std::vector<vector_t> points = floatsToPoints(geometry->get_info());
            setup_lines(geode, false, &points, color, line_thickness);
            break;
        }
        case PRX_LINESTRIP:
        {
            //double line_width = geometry->get_info().pop_back();
            const std::vector<vector_t> points = floatsToPoints(geometry->get_info());
            setup_lines(geode, true, &points, color, line_thickness);
            break;
        }
        case PRX_MESH:
        {
            //PRX_WARN_S("MESH LOADING NOT SUPPORTED YET!!!!!!!!");
            setup_trimesh(geode,geometry->get_trimesh(), color);
            //NO support yet
            break;
        }
        case PRX_CLOUD:
        {
            setup_pointcloud(geode, *(geometry->get_info()));
            break;
        }
        case PRX_HEIGHTMAP:
        {
            //PRX_WARN_S("MESH LOADING NOT SUPPORTED YET!!!!!!!!");
            setup_trimesh(geode,geometry->get_trimesh(), color);
            //NO support yet
            break;
        }
        case PRX_POLYGON:
        {
            setup_trimesh(geode,geometry->get_trimesh(), color, true);
            break;
        }
        case PRX_BULLET_CYLINDER:
        {
            // setup_trimesh(geode,geometry->get_trimesh());
            double radius, height;
            geometry->get_cylinder(radius, height);
            setup_bullet_cylinder(geode, radius, height, color);
            break;
        }
        default:
            PRX_FATAL_S("Geometry type not supported by OSG plugin: " << (int)geometry->get_type());
            break;
    }

    // PRX_PRINT ("CREATING GEODE WITH COLOR: " << color, PRX_TEXT_BLUE);

    // osg::StateSet* stateSet = new osg::StateSet;

    // osg::Material* material = new osg::Material;

    // // material->setAmbient
    // //     ( osg::Material::FRONT_AND_BACK, osg::Vec4( color[1], color[2], color[3], 1.0 ) );

    // material->setAmbient
    //     ( osg::Material::FRONT_AND_BACK, osg::Vec4( color[1], color[2], color[3], 1.0 ) );

    // material->setDiffuse
    //     ( osg::Material::FRONT_AND_BACK, osg::Vec4( color[1], color[2], color[3], 1.0 ) );

    // // material->setSpecular
    // //     ( osg::Material::FRONT_AND_BACK, osg::Vec4( color[1], color[2], color[3], 1.0 ) );


    // stateSet->setAttributeAndModes( material );

    // geode->setStateSet( stateSet );

   //  const std::string TheShaderSource =
   //     "uniform vec3 rgb;\n"
   //     "void main()\n"
   //     "{\n"
   //     "   gl_FragColor = vec4(rgb, 1.0);\n"
   //     "}\n";
   //  // Do the shader stuff
   // osg::ref_ptr<osg::Shader> shader(new osg::Shader(osg::Shader::FRAGMENT));
   // shader->setShaderSource(TheShaderSource);
 
   // osg::ref_ptr<osg::Program> program(new osg::Program());
   // program->addShader(shader);
 
   // osg::ref_ptr<osg::StateSet> ss = geode->getOrCreateStateSet();
   // ss->setAttribute(program);
 
   // osg::ref_ptr<osg::Uniform> rgbUniform(
   //    new osg::Uniform("rgb", osg::Vec3(color[1], color[2], color[3])));
   // ss->addUniform(rgbUniform);


    return geode;
}


    }
 }
