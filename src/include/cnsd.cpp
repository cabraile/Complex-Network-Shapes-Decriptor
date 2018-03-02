#ifndef __GUARD_COMPLEX_NETWORKS__
#define __GUARD_COMPLEX_NETWORKS__

#include <numeric>
#include "adjacency_matrix.cpp"
#include "io.cpp"

namespace cnsd {

  typedef std::vector<double> Descriptor;

  template<typename T>
  T max(const std::vector<T> & v)
  {
    T max = std::numeric_limits<T>::min();
    for (T x : v)
      max = (max < x) ? x : max;
    return max;
  }

  template<typename T>
  Descriptor CNSD (
    const AdjacencyMatrix<T> & mat,
    const std::size_t & nb_steps,
    const T & threshold_min,
    const T & threshold_max
  )
  {
    Descriptor desc;
    double step_size;

    step_size = (threshold_max - threshold_min) / ((double) nb_steps + 1.0);

    for(std::size_t step = 0; step < nb_steps; step++)
    {
      AdjacencyMatrix<bool> connections;
      std::vector<std::size_t> degrees_list;
      double feature;
      debug::log("=> Step ");
      debug::log("");

      // > Threshold
      T thresh_value = ( threshold_max - threshold_min) -
          step_size * (step + 1);
      debug::log("> Threshold: " + std::to_string(thresh_value));
      debug::log("");

      mat.binarize(connections, threshold_min, thresh_value);
      if(debug::log_enabled())
        connections.print();

      // > Degrees
      debug::log("> Degrees List: ");
      degrees_list = nodes_degree(connections);
      for(auto degree : degrees_list)
        debug::log( std::to_string(degree) + " ", true);
      debug::log("");

      // > Feature - Max Degree
      debug::log("> Features: ");
      feature = max(degrees_list);
      desc.push_back(feature);
      debug::log("Max: " +  std::to_string(feature));

      // > Feature - Average of Degrees
      feature = (double) std::accumulate(degrees_list.begin(),
          degrees_list.end(), 0.0) / ((double) mat.getNodesNumber());
      desc.push_back(feature);
      debug::log("Avg: " +  std::to_string(feature));
    }

    return desc;
  };

};

#endif
