#ifndef __GUARD_ADJACENCY_MATRIX__
#define __GUARD_ADJACENCY_MATRIX__

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <exception>
#include <string>
#include <limits>

#include "io.cpp"

namespace cnsd
{

  enum ConnectionType {
    CON_DIRECTED, CON_UNDIRECTED
  };

  std::vector<std::string> split(const std::string & s, char delimiter)
  {
     std::vector<std::string> tokens;
     std::vector<char> buffer;
     for(char c : s)
     {
        if(c != delimiter)
          buffer.push_back(c);
        else
        {
          if(buffer.size() != 0)
          {
            std::string token(buffer.begin(), buffer.end());
            tokens.push_back(token);
            buffer.clear();
          }
        }
     }
     return tokens;
  }

  template <typename T>
  class AdjacencyMatrix
  {
    /*
    * Adjacency matrix graph representation.
    * > Convention: directed connection of i to j is represented as
    * M[i,j] = weight (row i, col j).
    */
  private:
    std::vector<T> weights;
    std::size_t nb_nodes;
    ConnectionType connection_type;
    T max_weight, min_weight;
  public:

    AdjacencyMatrix(
      const std::size_t & nb_nodes = 0,
      const ConnectionType & connection_type = CON_DIRECTED
    )
    {
      this->resize(nb_nodes);
      this->setConnectionType(connection_type);

      this->max_weight = std::numeric_limits<T>::min();
      this->min_weight = std::numeric_limits<T>::max();
      return ;
    }

    // # SETTERS
    // ##########################

    void resize(const std::size_t & nb_nodes, const T & init_val = 0)
    {
      /*
      * Resizes the weight matrix.
      * > Cleanup edges.
      */
      double total_size = (nb_nodes * nb_nodes * sizeof(T)) /
          (1024 * 1024 * 1024);
      if(total_size >= 2)
        debug::log("(Warning) Memory usage over 2GB");

      this->nb_nodes = nb_nodes;
      this->weights = std::vector<T>(nb_nodes * nb_nodes, init_val);
      return ;
    };

    void setWeight(const std::size_t & index, const T & weight)
    {
      std::size_t node_from, node_to;
      indexToNodes(node_from, node_to, index);
      setWeight(node_from, node_to, weight);
      return ;
    }

    void setWeight(const std::size_t & i, const std::size_t & j, const T & weight)
    {
      std::size_t nb_nodes = this->getNodesNumber();

      if(i >=  nb_nodes || j >= nb_nodes)
        throw std::runtime_error("Index out of range");

      this->weights[(i * nb_nodes) + j] = weight;
      if(this->getConnectionType() == CON_UNDIRECTED)
        this->weights[(j * nb_nodes) + i] = weight;

      if(this->max_weight < weight)
        this->max_weight = weight;

      if(this->min_weight > weight)
        this->min_weight = weight;

      return ;
    }

    void setConnectionType( const ConnectionType & connection_type )
    {
      this->connection_type = connection_type;
      return ;
    }

    // ##########################

    // # GETTERS
    // ##########################

    T get(const std::size_t & index)  const
    {
      return this->weights[index];
    }

    T get(const std::size_t & node_from, const std::size_t & node_to) const
    {
      std::size_t index;
      this->nodesToIndex(index, node_from, node_to);
      return this->weights[index];
    }

    std::size_t getNodesNumber() const
    {
      return this->nb_nodes;
    }

    std::vector<T> getWeightsVector() const
    {
      return weights;
    }

    void indexToNodes(
      std::size_t & node_from,
      std::size_t & node_to,
      const std::size_t & index
    ) const
    {
      std::size_t nb_nodes = this->getNodesNumber();
      node_from = index / nb_nodes;
      node_to = index % nb_nodes;
      return ;
    }

    void nodesToIndex(
      std::size_t & index,
      const std::size_t & node_from,
      const std::size_t & node_to
    ) const
    {
      std::size_t nb_nodes = this->getNodesNumber();
      index = ( node_from * nb_nodes ) + node_to;
      return ;
    }

    T getMax() const { return max_weight; }

    T getMin() const { return min_weight; }

    ConnectionType getConnectionType() const
    {
      return this->connection_type;
    }

    // ##########################

    // # MODIFIERS
    // ##########################

    void cut(
      AdjacencyMatrix<T> & in_AM,
      const T & lower_T,
      const T & higher_T,
      const T & cut_value = 0
    ) const
    {
      std::size_t nb_nodes = this->getNodesNumber();
      ConnectionType conn_type = this->getConnectionType();

      in_AM.resize(nb_nodes, cut_value);
      in_AM.setConnectionType(conn_type);

      for(std::size_t node_from = 0; node_from < nb_nodes; node_from++)
      {
        std::size_t init = 0;
        if(conn_type == CON_UNDIRECTED)
          init = node_from;
        for(std::size_t node_to = init; node_to < nb_nodes; node_to++)
        {
          std::size_t index;
          nodesToIndex(index, node_from, node_to);
          T weight = this->get(index);

          if( weight >= lower_T && weight <= higher_T  )
              in_AM.setWeight(index, weight);
        }
      }
      return ;
    }

    void binarize(
      AdjacencyMatrix<bool> & in_AM,
      const T & lower_T,
      const T & higher_T
    ) const
    {
      std::size_t nb_nodes = this->getNodesNumber();
      ConnectionType conn_type = this->getConnectionType();

      in_AM.resize(nb_nodes, false);
      in_AM.setConnectionType(conn_type);

      for(std::size_t node_from = 0; node_from < nb_nodes; node_from++)
      {
        std::size_t init = 0;
        if(conn_type == CON_UNDIRECTED)
          init = node_from;
        for(std::size_t node_to = init; node_to < nb_nodes; node_to++)
        {
          std::size_t index;
          nodesToIndex(index, node_from, node_to);
          T weight = this->get(index);
          if( weight >= lower_T && weight <= higher_T  )
              in_AM.setWeight(index, true);
        }
      }
      return ;
    }

    // ##########################

    // # IO
    // ##########################

    void toFile(const std::string & file_name) const
    {
      std::ofstream o_file;
      o_file.open(file_name);
      // > Connection Type
      switch (this->getConnectionType())
      {
        case CON_UNDIRECTED:
          o_file << "U" << std::endl;
        break;

        case CON_DIRECTED:
          o_file << "D" << std::endl;
        break;
        default:

        break;
      }

      // > Number of Nodes
      o_file << this->nb_nodes << std::endl;

      // > Values
      for(T w : this->weights)
        o_file << w << " ";
      o_file << std::endl;

      o_file.close();
      return ;
    }

    bool fromFile(const std::string & file_name)
    {
      std::ifstream i_file;
      std::string line;
      std::size_t nb_nodes;

      i_file.open(file_name);
      if(!i_file.is_open())
        return false;

      // > Connection Type
      std::getline(i_file, line);
      if(line.compare("D") == 0) // if is D
        this->setConnectionType(CON_DIRECTED);
      else if(line.compare("U") == 0) // if is U
        this->setConnectionType(CON_UNDIRECTED);

      // > Number of Nodes
      std::getline(i_file, line);
      sscanf(line.c_str(), "%zu", &(nb_nodes));
      this->resize(nb_nodes);

      // > Values
      getline(i_file, line);
      std::vector<std::string> s_weights = split(line, ' ');

      std::size_t count = 0;
      for(std::string s_w : s_weights)
      {
        double weight = stod(s_w);
        std::size_t i = count / this->nb_nodes;
        std::size_t j = count % this->nb_nodes;
        this->setWeight(i, j, (T) weight);
        count++;
      }
      i_file.close();
      return true;
    }

    void print() const
    {
      for(std::size_t i = 0; i < nb_nodes; i++)
      {
        for(std::size_t j = 0; j < nb_nodes; j++)
        {
          std::cout << this->get((i * this->nb_nodes) + j);
          if(j != nb_nodes - 1)
            std::cout << " ";
        }
        std::cout << std::endl;
      }
      return ;
    }

    // ##########################
  };

  std::vector<std::size_t> nodes_degree(const AdjacencyMatrix<bool> & mat)
  {
    std::vector<std::size_t> degrees;
    std::size_t nb_nodes;
    ConnectionType conn_type;

    conn_type = mat.getConnectionType();
    nb_nodes = mat.getNodesNumber();

    degrees.resize(nb_nodes, 0);

    for(std::size_t node_from = 0; node_from < nb_nodes; node_from++)
    {
      std::size_t init = 0;
      if(conn_type == CON_UNDIRECTED)
        init = node_from;
      for(std::size_t node_to = init; node_to < nb_nodes; node_to++)
      {
        bool connected = mat.get(node_from, node_to);
        if(connected)
        {
          degrees[node_from] += 1;
          if(conn_type == CON_UNDIRECTED && node_from != node_to)
          {
            degrees[node_to] += 1;
          }
        }
      }
    }

    return degrees;
  }
};

#endif
