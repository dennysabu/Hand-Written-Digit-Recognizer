/**
 * @file abstract_node.hpp
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

#pragma once
#ifndef PRX_ABSTRACT_NODE_HPP
#define	PRX_ABSTRACT_NODE_HPP

#include "prx/utilities/definitions/defs.hpp"
#include "prx/utilities/spaces/space.hpp"
#include "prx/utilities/distance_metrics/graph_metric/graph_proximity.hpp"
#include <fstream>

#include <boost/graph/adjacency_list.hpp>

namespace prx
{
    namespace util
    {

        typedef boost::adjacency_list_traits<boost::listS, boost::listS, boost::directedS>::vertex_descriptor directed_vertex_index_t;
        typedef boost::adjacency_list_traits<boost::listS, boost::listS, boost::undirectedS>::vertex_descriptor undirected_vertex_index_t;

        enum graph_direction_t
        {
            DIRECTED_NODE, UNDIRECTED_NODE
        };

        class proximity_node_t;

        /**
         * An abstract node class for graph operations. Used to abstract out the particular 
         * requirements of motion planners so the same graph can be used for all of them.
         * 
         * @brief <b> An abstract node class for graph operations. </b>
         */
        class abstract_node_t
        {

          public:

            abstract_node_t()
            {
                point = NULL;
                search_id = 0;
                cached_heuristic.first = 0;
                cached_heuristic.second = 0;
                prox_node=nullptr;
            }

            virtual ~abstract_node_t()
            { 
            }

            /**
             * Serializes the node information to an output stream.
             * @brief Serializes the node information to an output stream.
             * @param output_stream The stream to output information to.
             * @param point_space The space that \c point belongs to.
             */
            virtual void serialize(std::ofstream& output_stream, const space_t* point_space);

            /**
             * Deserializes the node information from an input stream.
             * @brief Deserializes the node information from an input stream.
             * @param input_stream The stream to output information to.
             * @param point_space The space that \c point belongs to.
             */
            virtual void deserialize(std::ifstream& input_stream, const space_t* point_space);

            /**
             * Get the type of graph this node is being used in.
             * @brief Get the type of graph this node is being used in.
             * @return The graph type.
             */
            graph_direction_t get_graph_direction() const
            {
                return graph_direction;
            }

            /**
             * Helper function for casting to different node types.
             * @brief Helper function for casting to different node types.
             * @return The casted pointer.
             */
            template<class T>
            const T* as() const
            {
                return static_cast<const T*>(this);
            }

            template<class T>
            T* get()
            {
                return static_cast<T*>(this);
            }


            /**
             * @brief The point being encapsulated by this node.
             */
            space_point_t* point;

            /**
             * @brief For quick removal in graph_distance_metric
             */
            proximity_node_t* prox_node;


            /**
             * @brief It is used in order to avoid initializing all the node of the graph, every time we start a new A* search.
             */
            unsigned long search_id;

            /**
             *  @brief Integer identifier of this node.
             */
            unsigned node_id;

            /**
             *  @brief Cached heuristic value of the node.
             */
            std::pair<unsigned long, double> cached_heuristic;

          protected:
            /**
             * @brief The type of graph this node belongs to.
             */
            graph_direction_t graph_direction;

        };

    }
}

#endif


